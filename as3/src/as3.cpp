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
    Vector3 vel;
    Vector3 accel;
} KinematicsData;

// ===========================================================
// Main Function
// ===========================================================
int main()
{   
    double lastLogicTime = 0; //Deltas for controlling logic rates

    double spinAngle = 0; //Extra Credit Animation
    double spin_scale = 6.0;

    bool enabledExtraCredit = false; 

    raylib::Window window(800, 600, "CS381 - Assignment 3");
    window.SetState(FLAG_WINDOW_RESIZABLE);

    SetTargetFPS(DRAW_FPS); //Turns out -- trying to do your own FPS limiter is a bad idea -_-
    //Driver timeouts are crazy things (╯°□°）╯︵ ┻━┻
    
    // ===========================================================
    // Model Loading + Default Transforms
    // ===========================================================
    raylib::Model cube = raylib::Mesh::Cube(30, 30, 30).LoadModelFrom();
    raylib::Camera camera = raylib::Camera(
        {0, 120, 500}, 
        {0, 0, 0}, 
        {0, 1, 0}, 
        45
    ); 

    KinematicsData car_kinematics = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    raylib::Model grass = raylib::Mesh::Plane(100, 100, 10, 10).LoadModelFrom();
    raylib::Texture grassTexture = raylib::Texture("assets/textures/grass.jpg");
    grass.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grassTexture;

    raylib::Model car = raylib::Model("../../assets/Kenny Car Kit/sedan.glb");
    car.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    raylib::Model wheel = raylib::Model("../../assets/Kenny Car Kit/wheel-default.glb");
    wheel.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    cs381::SkyBox sky("textures/skybox.png");

    while (!window.ShouldClose())
    {
        double currentTime = GetTime();
        
        // ===========================================================
        // Logic Block
        // ===========================================================

            double logicDelta = lastLogicTime - currentTime;
            
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                enabledExtraCredit = !enabledExtraCredit;
                if(!enabledExtraCredit){
                    spinAngle = 0; //reset spin
                } else {
                }
            }

            if(enabledExtraCredit){

            } 

            auto car_transform = combine( 
                rotate({0, 1, 0}, (spinAngle)*DEG2RAD),
                translate(car_kinematics.pos),
                scale({1, 1, 1})
            ); 


        lastLogicTime = currentTime;


        // ===========================================================
        // Draw Block
        // ===========================================================
        
        window.BeginDrawing();

        window.ClearBackground(BLACK);
        camera.BeginMode();
            sky.Draw();

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

