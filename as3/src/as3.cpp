/**
 * @file as3.cpp
 * @author Lucas Pinto
 * @brief 
 * @version 0.1
 * @date 2025-02-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #include <iostream>

 #include "raylib-cpp.hpp"
 #include "skybox.hpp"
 
 //#define WIRE_FRAME_COLOR WHITE //was for an old defunct implementation
 #define DEFAULT_SCALE 30
 
 //#define LOGIC_SPEED 1
 #define DRAW_FPS 60
 #define DEFAULT_VOLUME 0.5
 
 #define INITIAL_FONT_SIZE 50
 #define INITIAL_FONT_SPACING 2

 #define MAX_SPEED 100
 #define Y_HEADING_LERP (0.1)
 #define Y_HEADING_ANGLE_INCREMENT (5.0)

 #define X_HEADING_LERP (0.02)
 #define X_HEAD_LIMIT (45.0*DEG2RAD)
 #define DELTA_COMPENSATOR 100.0
 
 
 template<typename T> //Cool type validation!
 concept Transformer = requires(T t, raylib::Matrix m) {
     { t(m) } -> std::convertible_to<raylib::Matrix>;
 };
 
 /**
  * @brief Draws a model with a transformation applied to it.
  * 
  * @param model The model to be drawn.
  * @param transformer A function or lambda that takes a raylib::Matrix and returns a transformed raylib::Matrix.
  */
 void DrawBoundedModel(raylib::Model& model, Transformer auto transformer)
 {   
     raylib::Matrix backup = model.transform;    // LINE EXPLANATION:: Creates a backup of the model's original transform matrix. 
                                                 // We don't want to permanent changes to the model's transform, so we make a copy of the original matrix.
                                                 // Presumably, this is because we'd otherwise have to track the transforms ourselves, which is a pain.
                                                 // Additionally, this way we force transforms to aggregate externally.
                                                 // If we do want to keep track, this doesn't obfuscate tracing transforms.
     model.transform = transformer(backup);  // LINE EXPLANATION:: Applies the transformation function to the model's transform matrix. 
                                             // Whether translation, scaling, or rotation, so long as "transformer" is valid, it should work!
                                             // Initially thought this was ridiculous, since why not just slap a matrix argument?
                                             // But this lets us do some really fancy stuff with chaining transformations.
                                             // Especially if the transformations don't all work with the same operations or "orders".
     model.Draw({}); // LINE EXPLANATION:: Draws the model with the now-transformed matrix. 
                     // Has a "{}" to explicitly state no additional transformations are applied.
     model.GetTransformedBoundingBox().Draw(); 
     // LINE EXPLANATION:: This essentially draws the trasnformed bounding box of the model after it has been transformed.
     // The bounding box itself is a raylib::BoundingBox object, which has a Draw() method that draws the box.
     // It's a very convenient way to visualize the bounds of the model after transformation especially for collisions. 
     // NOTE:: Thank the Discord. I was scratching my head over this one until I looked at the function
     // implementation and decided to check the Discord. ヾ(≧▽≦*)o
     model.transform = backup;   // LINE EXPLANATION:: Resets the model's transform matrix to its original state. 
                                 // Also just realized, a lot of properties of objects we tend to write with respect to a frame of reference.
                                 // Resetting the matrix lends to that reference paradigm rather than sequential local transforms relative to itself. 
                                 // That, and its how most people think ¯\_(ツ)_/¯
 }
 // ===========================================================
 // Transformation Lambdas
 // ===========================================================
 /**
  * @brief Arbitrary translation function that translates on each axis accordingly using a vector.
  * 
  */
 auto translate = [](raylib::Vector3 translation) { 
     // Lambda Inception (￣▽￣)"
     return [=](raylib::Matrix& transform) -> raylib::Matrix {
         return transform.Translate(translation);
     };
 };
 /**
  * @brief Arbitrary scaling function that scales each axis accordingly using a vector.
  * 
  */
 auto scale = [](raylib::Vector3 scaling) { 
     // Lambda Inception (￣▽￣)"
     return [=](raylib::Matrix& transform) -> raylib::Matrix {
         return MatrixMultiply(transform, MatrixScale(scaling.x, scaling.y, scaling.z));
     };
 };
 /**
  * @brief Arbitrary rotation function that rotates (assumedly right hand rule) around the given axis vector.
  * 
  */
 auto rotate = [](raylib::Vector3 axis, float angle) {
     // Lambda Inception (￣▽￣)"
     return [=](raylib::Matrix& transform) -> raylib::Matrix {
         return MatrixMultiply(transform, MatrixRotate(axis, angle)); 
     };
 };
 /**
  * @brief Combines arbitrary number of transformation lambdas into one.
  */
 auto combine = [](auto... transformers) {
     // Lambda uh... ㄟ( ▔, ▔ )ㄏ
     return [=](raylib::Matrix& transform) -> raylib::Matrix {
         raylib::Matrix median_transform; //Will in fact flip the hell out if I don't do this (╯°□°）╯︵ ┻━┻
         median_transform = transform; //Not exactly sure why??? My guess is its some goofy addressing hallucinations
         ((median_transform = transformers(median_transform)), ...); //Also this is super cool(￣▽￣)b
         return median_transform;
     };
 };
 
 // ===========================================================
 // Useful Structs
 // ===========================================================

typedef struct{
    Vector3 pos;
    Vector2 rot;
    double vel;
    double accel;
} KinematicsData;

// ===========================================================
// Main Function
// ===========================================================
int main()
{   
    float timer = 0;
    bool enabledExtraCredit = false; 

    bool keyLeftPressed = false;
    bool keyRightPressed = false;
    bool keyUpPressed = false;
    bool keyDownPressed = false;
    bool keyWPressed = false;
    bool keySPressed = false;

    Vector3 cameraCarOffset = {0, 120, 500};

    raylib::Window window(800, 600, "CS381 - Assignment 3");
    window.SetState(FLAG_WINDOW_RESIZABLE);

    SetTargetFPS(DRAW_FPS); //Turns out -- trying to do your own FPS limiter is a bad idea -_-
    //Driver timeouts are crazy things (╯°□°）╯︵ ┻━┻
    
    // ===========================================================
    // Model Loading + Default Transforms
    // ===========================================================
    raylib::Model cube = raylib::Mesh::Cube(30, 30, 30).LoadModelFrom();
    raylib::Camera camera = raylib::Camera(
        cameraCarOffset, 
        {0, 0, 0}, 
        {0, 1, 0}, 
        45
    ); 

    KinematicsData car_kinematics = {
        {0, 0, 0},
        {0, 0},
        0,
        0
    };

    raylib::Model grass = raylib::Mesh::Plane(1000, 1000, 10, 10).LoadModelFrom();
    raylib::Texture grassTexture = raylib::Texture("../../assets/textures/grass.png");
    grass.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grassTexture;

    raylib::Model car = raylib::Model("../../assets/Kenny Car Kit/sedan.glb");
    car.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    raylib::Model wheel = raylib::Model("../../assets/Kenny Car Kit/wheel-default.glb");
    wheel.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    cs381::SkyBox sky("textures/skybox.png");

    while (!window.ShouldClose())
    {
        
        // ===========================================================
        // Logic Block
        // ===========================================================

            double logicDelta = GetFrameTime();
            
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                enabledExtraCredit = !enabledExtraCredit;
                if(!enabledExtraCredit){

                    } else {
                }
            }

            keyLeftPressed = keyLeftPressed ? keyLeftPressed : raylib::Keyboard::IsKeyDown(KEY_LEFT);
            keyRightPressed = keyRightPressed ? keyRightPressed : raylib::Keyboard::IsKeyDown(KEY_RIGHT);
            keyUpPressed = keyUpPressed ? keyUpPressed : raylib::Keyboard::IsKeyDown(KEY_UP);
            keyDownPressed = keyDownPressed ? keyDownPressed : raylib::Keyboard::IsKeyDown(KEY_DOWN);
            keyWPressed = keyWPressed ? keyWPressed : raylib::Keyboard::IsKeyDown(KEY_W);
            keySPressed = keySPressed ? keySPressed : raylib::Keyboard::IsKeyDown(KEY_S);

            if(timer <= 0){
                double Y_heading_lerp_ratio = std::min(Y_HEADING_LERP * DELTA_COMPENSATOR * logicDelta, 1.0);
                
                if(keyLeftPressed){ 
                    car_kinematics.rot.y -= Y_HEADING_ANGLE_INCREMENT*DEG2RAD;
                    car_kinematics.rot.y = fmod(car_kinematics.rot.y, 360*DEG2RAD);
                    keyLeftPressed = false;
                } else if(keyRightPressed){
                    car_kinematics.rot.y += Y_HEADING_ANGLE_INCREMENT*DEG2RAD;
                    car_kinematics.rot.y = fmod(car_kinematics.rot.y, 360*DEG2RAD);
                    keyRightPressed = false;
                } 
    
                if(keyUpPressed){
                    car_kinematics.vel = lerp(car_kinematics.vel, MAX_SPEED, 0.1);
                    keyUpPressed = false;
                } else if(keyDownPressed){
                    car_kinematics.vel = lerp(car_kinematics.vel, -MAX_SPEED, 0.1);
                    keyDownPressed= false;
                } else {
                    car_kinematics.vel = lerp(car_kinematics.vel, 0, 0.1);
                }

                double X_heading_lerp_ratio = std::min(X_HEADING_LERP * DELTA_COMPENSATOR * logicDelta, 1.0);
                
                if(keyWPressed){
                    car_kinematics.rot.x = lerp(car_kinematics.rot.x, X_HEAD_LIMIT, X_heading_lerp_ratio);
                    car_kinematics.rot.x = fmod(car_kinematics.rot.x, 360*DEG2RAD);
                    keyWPressed = false;
                } else if(keySPressed){
                    car_kinematics.rot.x = lerp(car_kinematics.rot.x, -X_HEAD_LIMIT, X_heading_lerp_ratio);
                    car_kinematics.rot.x = fmod(car_kinematics.rot.x, 360*DEG2RAD);
                    keySPressed = false;
                } else {
                    car_kinematics.rot.x = lerp(car_kinematics.rot.x, 0, X_heading_lerp_ratio);
                }

                //std::cout << car_kinematics.pos.x << " " << car_kinematics.pos.z << std::endl;
                std::cout << car_kinematics.rot.x << " " << car_kinematics.rot.y << std::endl;
                timer = 2.5;
            }

            
            //Update Kinematics
            //to be honest I'm not if I'm doing it right, but it seems to be working so :p
            //too lazy to work out the algebra so I just swapped sin/cos from standard spherical coordinates to whatever the heck raylib lieks
            car_kinematics.pos.x += car_kinematics.vel * sin(car_kinematics.rot.y) * cos(car_kinematics.rot.x) * logicDelta;
            car_kinematics.pos.y += (-1.0) * car_kinematics.vel * sin(car_kinematics.rot.x) * logicDelta;
            car_kinematics.pos.z += car_kinematics.vel * cos(car_kinematics.rot.y) * cos(car_kinematics.rot.x) * logicDelta;

            //Update Camera
            camera.position.x = lerp(camera.position.x, car_kinematics.pos.x + cameraCarOffset.x, 0.1);
            camera.position.y = lerp(camera.position.y, car_kinematics.pos.y + cameraCarOffset.y, 0.1);
            camera.position.z = lerp(camera.position.z, car_kinematics.pos.z + cameraCarOffset.z, 0.1);

            camera.target.x = lerp(camera.target.x, car_kinematics.pos.x, 0.1);
            camera.target.y = lerp(camera.target.y, car_kinematics.pos.y, 0.1);
            camera.target.z = lerp(camera.target.z, car_kinematics.pos.z, 0.1);


            auto car_transform = combine( 
                rotate({1, 0, 0}, car_kinematics.rot.x),
                rotate({0, 1, 0}, car_kinematics.rot.y),
                //rotate({0, 0, 1}, car_kinematics.rot.z), //Spherical coordinate system so no need for z ╰(￣ω￣ｏ)
                translate(car_kinematics.pos),
                scale({1, 1, 1})
            ); 

            timer -= window.GetFrameTime()*1000;

        // ===========================================================
        // Draw Block
        // ===========================================================
        
        window.BeginDrawing();

        window.ClearBackground(BLACK);
        camera.BeginMode();
            sky.Draw();
            grass.Draw({});
            
        
            if(enabledExtraCredit){
                DrawBoundedModel(wheel, combine( translate({15, 0, 20}), car_transform));
                DrawBoundedModel(wheel, combine(rotate({0,1,0},180*DEG2RAD), translate({-15, 0, 20}), car_transform));
                DrawBoundedModel(wheel, combine( translate({15, 0, -20}), car_transform));
                DrawBoundedModel(wheel, combine(rotate({0,1,0},180*DEG2RAD), translate({-15, 0, -20}),  car_transform));
            } else {
                
            }
            DrawBoundedModel(car, car_transform); 
                
            camera.EndMode();
        

        DrawTextEx(GetFontDefault(), "[Left Click] to Toggle Extra Credit Features", Vector2(10,10), INITIAL_FONT_SIZE/2, INITIAL_FONT_SPACING, WHITE);

        window.EndDrawing(); 
        
        
    }

    return 0;
}

