/**
 * @file as3.cpp
 * @author Lucas Pinto
 * @brief Program that creates a 3D scene with a car, skybox, and ground plane. The car can be controlled to move and rotate in the scene.
 *         Most movements are smoothed, and even camera movement and FOV changes in response to car speed is smoothed. 
 *         Extra Credit and Ordinary Features were implemented in tandem since it was just more convenient that way :p
 *         Also freebird plays if you fly the car for long enough
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

 #define MAX_SPEED 300
 #define CAMERA_LERP (0.03)
 #define CAR_LERP (0.05)

 #define MAX_SPEED 300
 #define Y_HEADING_LERP (0.1)
 #define Y_HEADING_ANGLE_INCREMENT (7.0)

 #define CAMERA_MINIMUM_FOV 45.0
 #define CAMERA_MAXIMUM_FOV 60.0
 #define CAMERA_FOV_LERP (0.005)

 #define CAR_ACCELERATION_INCREMENT (30)

 #define X_HEADING_LERP (0.02)
 #define X_HEAD_LIMIT (45.0*DEG2RAD)
 #define DELTA_COMPENSATOR 100.0

 #define PLANE_WIDTH 10000
 #define PLANE_LENGTH 10000
 
 
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
} KinematicsData;

// ===========================================================
// Main Function
// ===========================================================
int main()
{   
    float timer = 0;
    double freebird_volume = 0;
    //bool enabledExtraCredit = false; 

    bool keyLeftPressed = false;
    bool keyRightPressed = false;
    bool keyQPressed = false;
    bool keyEPressed = false;
    bool keyWPressed = false;
    bool keySPressed = false;
    bool keyAPressed = false;
    bool keyDPressed = false;
    bool keyTPressed = false;
    bool keySpacePressed = false;

    bool freebird_controls = false;

    bool freebird_playing = false;

    Vector3 cameraCarOffset = {0, 120, 500};

    raylib::Window window(800, 600, "CS381 - Assignment 3"); //Feature #1 - Window Title (??? pts)
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
        {0, 90*DEG2RAD},
        0
    };

    raylib::Model grass = raylib::Mesh::Plane(PLANE_WIDTH, PLANE_LENGTH, 100, 100).LoadModelFrom();
    raylib::Texture grassTexture = raylib::Texture("../../assets/textures/grass.png");
    grass.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grassTexture;

    raylib::Model car = raylib::Model("../../assets/Kenny Car Kit/sedan.glb");
    car.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    raylib::Model wheel = raylib::Model("../../assets/Kenny Car Kit/wheel-default.glb");
    wheel.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    raylib::Model cow = raylib::Model("../../custom_assets/as2/Cow.glb");   
    cow.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);    // From... https://www.fab.com/listings/fbd8c8b0-fc59-460e-adb8-c7ab8f19a6f9

    raylib::Model rocket = raylib::Model("../../assets/Kenny Space Kit/rocketA.glb"); 
    rocket.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    raylib::AudioDevice defaultDevice;
    raylib::Music freebird;
    freebird.Load("../../custom_assets/as3/Lynyrd Skynyrd - Free Bird (Official Audio).mp3");
    freebird.Pause();

    cs381::SkyBox sky("textures/skybox.png");

    while (!window.ShouldClose())
    {
        
        // ===========================================================
        // Logic Block
        // ===========================================================

            double logicDelta = GetFrameTime();
            double freebird_factor = freebird_volume/DEFAULT_VOLUME;

            // I think this counts as unbuffered input??? 
            // Feature #5 - Use Unbuffered Input (10 points total)
            //EC Feature #1 - Don't Use IsKeyPressed(), instead use a Bool/Timer (2 points)
            keyLeftPressed = keyLeftPressed ? keyLeftPressed : raylib::Keyboard::IsKeyDown(KEY_LEFT);
            keyRightPressed = keyRightPressed ? keyRightPressed : raylib::Keyboard::IsKeyDown(KEY_RIGHT);
            keyQPressed = keyQPressed ? keyQPressed : raylib::Keyboard::IsKeyDown(KEY_Q);
            keyEPressed = keyEPressed ? keyEPressed : raylib::Keyboard::IsKeyDown(KEY_E);
            keyWPressed = keyWPressed ? keyWPressed : raylib::Keyboard::IsKeyDown(KEY_W);
            keySPressed = keySPressed ? keySPressed : raylib::Keyboard::IsKeyDown(KEY_S);
            keyAPressed = keyAPressed ? keyAPressed : raylib::Keyboard::IsKeyDown(KEY_A);
            keyDPressed = keyDPressed ? keyDPressed : raylib::Keyboard::IsKeyDown(KEY_D);
            keyTPressed = keyTPressed ? keyTPressed : raylib::Keyboard::IsKeyDown(KEY_T);
            keySpacePressed = keySpacePressed ? keySpacePressed : raylib::Keyboard::IsKeyDown(KEY_SPACE);

            if(freebird_factor >= 0.90){ //There's no going back
                keyWPressed = true;
                keySpacePressed = false;
            }

            if(timer <= 0){ //Feature #5A - Incremenenting, not Continuous Values (5pts)
                double camera_lerp_ratio = std::min( pow(freebird_factor,2) * CAMERA_FOV_LERP * DELTA_COMPENSATOR * logicDelta, 1.0);
                double car_vel_lerp_ratio = std::min(CAR_LERP*DELTA_COMPENSATOR * logicDelta, 1.0);
                if(keyDPressed){ //Feature #5F - Pressing D Increases Car's Heading (2 points)
                    if(freebird_controls){
                        car_kinematics.rot.y -= Y_HEADING_ANGLE_INCREMENT*DEG2RAD*logicDelta*DELTA_COMPENSATOR;
                    } else if (raylib::Keyboard::IsKeyUp(KEY_D)){
                        car_kinematics.rot.y -= Y_HEADING_ANGLE_INCREMENT*DEG2RAD;
                        //std::cout << "REAL" << std::endl;
                    }
                    if(freebird_controls || raylib::Keyboard::IsKeyUp(KEY_D)){
                        car_kinematics.rot.y = fmod(car_kinematics.rot.y, 360*DEG2RAD);
                        keyDPressed = false;
                    }
                } else if(keyAPressed){ //Feature #5E - Pressing A Increases Car's Heading (2 points)
                    if(freebird_controls){
                        car_kinematics.rot.y += Y_HEADING_ANGLE_INCREMENT*DEG2RAD*logicDelta*DELTA_COMPENSATOR;
                    } else if (raylib::Keyboard::IsKeyUp(KEY_A)) {
                        car_kinematics.rot.y += Y_HEADING_ANGLE_INCREMENT*DEG2RAD;
                    }
                    if(freebird_controls || raylib::Keyboard::IsKeyUp(KEY_A)){
                        car_kinematics.rot.y = fmod(car_kinematics.rot.y, 360*DEG2RAD);
                        keyAPressed = false;
                    }
                } 
    
                if(keyWPressed){ //Feature #5C - Pressin W Increases Car Velcoity (1 point)
                    if(freebird_controls){
                        car_kinematics.vel = lerp(car_kinematics.vel, MAX_SPEED+(std::pow(3,10*freebird_factor)-1), car_vel_lerp_ratio);
                    } else if (raylib::Keyboard::IsKeyUp(KEY_W)) {
                        car_kinematics.vel += CAR_ACCELERATION_INCREMENT;
                    }
                    if(freebird_controls || raylib::Keyboard::IsKeyUp(KEY_W)){
                        camera.SetFovy(std::max(lerp(camera.GetFovy(), CAMERA_MAXIMUM_FOV, camera_lerp_ratio),CAMERA_MINIMUM_FOV));
                        keyWPressed = false;
                    }
                } else if(keySPressed){ //Feature #5D - Pressing S Decreases Car Velcoity (1 point)
                    if(freebird_controls){
                        car_kinematics.vel = lerp(car_kinematics.vel, -MAX_SPEED, car_vel_lerp_ratio);
                    } else if (raylib::Keyboard::IsKeyUp(KEY_S)) {
                        car_kinematics.vel -= CAR_ACCELERATION_INCREMENT;
                    }
                    if(freebird_controls || raylib::Keyboard::IsKeyUp(KEY_S)){
                        keySPressed = false;
                    }
                
                } else {
                    if(freebird_controls){
                        car_kinematics.vel = lerp(car_kinematics.vel, 0, 0.05*car_vel_lerp_ratio);
                        camera.SetFovy(lerp(camera.GetFovy(), CAMERA_MINIMUM_FOV, camera_lerp_ratio));
                        //Feature #5B - Car Continues to Move even w/o active control (30 points)
                    }
                }

                double X_heading_lerp_ratio = std::min(X_HEADING_LERP * DELTA_COMPENSATOR * logicDelta, 1.0);
                
                if(keyQPressed){ //EC Feature #4 - The Ability to Fly (10 points)
                    if(freebird_controls){
                        car_kinematics.rot.x = lerp(car_kinematics.rot.x, X_HEAD_LIMIT, X_heading_lerp_ratio);
                    } else if (raylib::Keyboard::IsKeyUp(KEY_Q)){
                        car_kinematics.rot.x += Y_HEADING_ANGLE_INCREMENT*DEG2RAD;
                    }
                    car_kinematics.rot.x = fmod(car_kinematics.rot.x, 360*DEG2RAD);
                    if(freebird_controls || raylib::Keyboard::IsKeyUp(KEY_Q)){
                        keyQPressed = false;
                    }
                } else if(keyEPressed){
                    if(freebird_controls){
                        car_kinematics.rot.x = lerp(car_kinematics.rot.x, -X_HEAD_LIMIT, X_heading_lerp_ratio);
                    } else if (raylib::Keyboard::IsKeyUp(KEY_E)){
                        car_kinematics.rot.x -= Y_HEADING_ANGLE_INCREMENT*DEG2RAD;
                    }
                    car_kinematics.rot.x = fmod(car_kinematics.rot.x, 360*DEG2RAD);
                    if(freebird_controls || raylib::Keyboard::IsKeyUp(KEY_E)){
                        keyEPressed = false;
                    }
                    
                } else {
                    if(freebird_controls){
                        car_kinematics.rot.x = lerp(car_kinematics.rot.x, 0, X_heading_lerp_ratio);
                    }
                }

                if(keyTPressed && raylib::Keyboard::IsKeyUp(KEY_T)){
                    freebird_controls = !freebird_controls;
                    keyTPressed = false;
                    std::cout << "Freebird Controls: " << (freebird_controls ? "Enabled" : "Disabled") << std::endl;
                }

                if(keySpacePressed){ //Feature #5G - Pressing Space Resets Car's Velocity (4 points)
                    if(freebird_controls){  //Multiples speed in freebird mode lol
                        car_kinematics.vel *= std::max((1.50)*freebird_factor, 1.05)*((logicDelta)/(1.0/60.0)); //FINALLY FIGURED OUT HOW TO PROPERLY SCALE LOGIC DELTA
                    } else if(raylib::Keyboard::IsKeyUp(KEY_SPACE)) {
                        car_kinematics.vel = 0;
                        std::cout << "stopping car" << std::endl;
                    }
                        
                    if(freebird_controls || raylib::Keyboard::IsKeyUp(KEY_SPACE)){
                        keySpacePressed = false;
                    } 
                }

                //std::cout << car_kinematics.pos.x << " " << car_kinematics.pos.z << std::endl;
                //std::cout << car_kinematics.rot.x << " " << car_kinematics.rot.y << std::endl;

                timer = 2.5; //Arbitrary timer value to prevent infinite procession :p
            }
            
            //Feature #3 - Kinematics Physics (10 points), 3D too!
            //to be honest I'm not if I'm doing it right, but it seems to be working so :p
            //too lazy to work out the algebra so I just swapped sin/cos from standard spherical coordinates to whatever the heck raylib lieks
            car_kinematics.pos.x += car_kinematics.vel * sin(car_kinematics.rot.y) * cos(car_kinematics.rot.x) * logicDelta;
            car_kinematics.pos.y += (-1.0) * car_kinematics.vel * sin(car_kinematics.rot.x) * logicDelta;
            car_kinematics.pos.z += car_kinematics.vel * cos(car_kinematics.rot.y) * cos(car_kinematics.rot.x) * logicDelta;

            //Update Camera
            double camera_lerp_ratio = std::min(CAMERA_LERP * DELTA_COMPENSATOR * logicDelta, 1.0);

            camera.position.x = lerp(camera.position.x, car_kinematics.pos.x + cameraCarOffset.x, camera_lerp_ratio);
            camera.position.y = lerp(camera.position.y, car_kinematics.pos.y + cameraCarOffset.y, camera_lerp_ratio);
            camera.position.z = lerp(camera.position.z, car_kinematics.pos.z + cameraCarOffset.z, camera_lerp_ratio);

            camera.target.x = lerp(camera.target.x, car_kinematics.pos.x, camera_lerp_ratio);
            camera.target.y = lerp(camera.target.y, car_kinematics.pos.y, camera_lerp_ratio);
            camera.target.z = lerp(camera.target.z, car_kinematics.pos.z, camera_lerp_ratio);


            auto car_transform = combine( 
                rotate({1, 0, 0}, car_kinematics.rot.x),
                rotate({0, 1, 0}, car_kinematics.rot.y),
                //rotate({0, 0, 1}, car_kinematics.rot.z), //Spherical coordinate system so no need for z ╰(￣ω￣ｏ)
                translate(car_kinematics.pos),
                scale({1, 1, 1})
            ); 
            
            if(freebird_controls){
                timer -= window.GetFrameTime()*1000;
            } else {
                timer -= window.GetFrameTime()*500;
            }
            
            
            float neg_velocity_factor = MAX_SPEED/std::max(car_kinematics.vel,(double)(MAX_SPEED*0.90));
            float pos_velocity_factor = std::max(car_kinematics.vel,(double)(MAX_SPEED*0.90))/MAX_SPEED;
            float heading_factor = std::min(std::max(std::abs(car_kinematics.rot.x)/(X_HEAD_LIMIT),0.1),0.5);

            if(car_kinematics.vel > (MAX_SPEED/2) && car_kinematics.pos.y != 0){
                freebird_volume = lerp(freebird_volume, DEFAULT_VOLUME, 0.0003*heading_factor*pos_velocity_factor*DELTA_COMPENSATOR*logicDelta);
                freebird.SetVolume(freebird_volume);
            } else {
                freebird_volume = lerp(freebird_volume, 0, neg_velocity_factor*0.005*DELTA_COMPENSATOR*logicDelta);
                freebird.SetVolume(freebird_volume);
            }

            if(freebird_volume < 0.001 && freebird_playing){
                freebird.Pause();
                freebird_playing = false;
                std::cout << "Freebird is paused" << std::endl;
            } else if (freebird_volume >= 0.001 && !freebird_playing){
                freebird.Play();
                freebird_playing = true;
                std::cout << "Freebird is playing" << std::endl;
            }

            if(freebird_factor >= 0.9999){
                std::cout << "FREEBIRD IS AT MAX CAPACITY, CRASHING PROGRAM!!! (╯°□°）╯︵ ┻━┻" << std::endl;
                camera.EndMode();
                window.EndDrawing();
                return 0;
            } else if (freebird_factor >= 0.05){
                //std::cout << "Position: " << car_kinematics.pos.x << " " << car_kinematics.pos.z << std::endl;
                //std::cout << "Speed: " << car_kinematics.vel << std::endl
                car_kinematics.pos.x = fmod(car_kinematics.pos.x, PLANE_WIDTH/2);
                car_kinematics.pos.z = fmod(car_kinematics.pos.z, PLANE_LENGTH/2);
                car_kinematics.pos.y = std::max(car_kinematics.pos.y, (float)0.0);
            }

            if(freebird_factor >= 0.90){ //There's no going back
                keyWPressed = true;
            }

            freebird.Update();

        // ===========================================================
        // Draw Block
        // ===========================================================
        
        window.BeginDrawing();

        window.ClearBackground(BLACK);
        camera.BeginMode();
            sky.Draw();       //Feature #2 - Create Scene w/ a Car, Skybox, and Ground Plane (5 points)
            grass.Draw({});
            
            if(freebird_playing){
                DrawBoundedModel(cow, combine(rotate({0,1,0},180*DEG2RAD),translate({0,10,0}),car_transform));

                if(freebird_factor > 0.5){
                    DrawBoundedModel(rocket, combine(rotate({1,0,0},90*DEG2RAD),translate({0,10,-100}),car_transform));
                }
            }
            //EC Feature #2 - Add Four Wheels to the Car that FOllow (5 points)
            DrawBoundedModel(wheel, combine(translate({15, 0, 20}), car_transform));
            DrawBoundedModel(wheel, combine(rotate({0,1,0},180*DEG2RAD), translate({-15, 0, 20}), car_transform));
            DrawBoundedModel(wheel, combine(translate({15, 0, -20}), car_transform));
            DrawBoundedModel(wheel, combine(rotate({0,1,0},180*DEG2RAD), translate({-15, 0, -20}),  car_transform));

            DrawBoundedModel(car, car_transform); 
                
            camera.EndMode();
            
            std::string controls = "[A/D] to rotate car\n[W/S] to move car\n[Q/E] to rotate car vertically\n[Space] to stop car";
            controls += "\n[T] to toggle control scheme [" + std::string(freebird_controls ? "Freebird" : "Car") + "]";
            const char* controls_cstr = controls.c_str();
            DrawTextEx(GetFontDefault(), 
            controls_cstr, 
            Vector2(10,10), 
            INITIAL_FONT_SIZE/2, 
            INITIAL_FONT_SPACING, 
            Color{0,0,0,(int)(255*std::max(1-(freebird_factor*50),0.0))});

            if(freebird_playing){
                std::string freebird_text = TextFormat("freebird is playing... [%.2f%]", (freebird_volume/DEFAULT_VOLUME)*100.0);
                const char* freebird_text_cstr = freebird_text.c_str();
                DrawTextEx(GetFontDefault(), 
                freebird_text_cstr, 
                Vector2(10,window.GetHeight()-INITIAL_FONT_SIZE), 
                INITIAL_FONT_SIZE/2, 
                INITIAL_FONT_SPACING, 
                Color{0,0,0,(int)(255*std::min((freebird_volume*10.0/DEFAULT_VOLUME),1.0))});

                if(freebird_factor > 0.01){
                    std::string freebird1_text = TextFormat("CONTAINING FREEBIRD TO BOUNDARY CONDITIONS... [%.2f%]", std::min((freebird_factor/0.05)*100.0,100.0));
                    freebird1_text += "\n[FLYING UP/DOWN ACCELERATES FREEBIRD FACTOR]";
                    const char* freebird1_text_cstr = freebird1_text.c_str();
                    DrawTextEx(GetFontDefault(), 
                    freebird1_text_cstr, 
                    Vector2(10,0), 
                    INITIAL_FONT_SIZE/2, 
                    INITIAL_FONT_SPACING, 
                    Color{0,0,0,(int)(255*std::min((freebird_volume*10.0/DEFAULT_VOLUME),1.0))});
                } 

                if(freebird_factor > 0.1){
                    std::string freebird2_text;
                    if(freebird_factor > 0.4){
                        freebird2_text = "FREEBIRD IS ATTEMPTING TO ESCAPE";
                    } else if (freebird_factor > 0.3){
                        freebird2_text = "FREEBIRD IS CONTAINED..?";
                    } else if (freebird_factor > 0.2){
                        freebird2_text = "FREEBIRD IS CONTAINED...";
                    } else {
                        freebird2_text = "FREEBIRD IS CONTAINED";
                    }
                    const char* freebird2_text_cstr = freebird2_text.c_str();
                    DrawTextEx(GetFontDefault(), 
                    freebird2_text_cstr, 
                    Vector2(10,INITIAL_FONT_SIZE), 
                    INITIAL_FONT_SIZE/2, 
                    INITIAL_FONT_SPACING, 
                    Color{0,0,0,(int)(255*std::min((freebird_volume*10.0/DEFAULT_VOLUME),1.0))});
                } 

                if(freebird_factor > 0.5){
                    std::string freebird3_text = TextFormat("FREEBIRD TO CRITICAL HORIZON BOUNDARY... [%.2f%]", ((freebird_factor/0.9)*100.0));
                    if (freebird_factor > 0.6){
                        freebird3_text += "\nFREEBIRD IS APPROACHING CRITICAL HORIZON BOUNDARY";
                        if(freebird_factor > 0.8){
                            freebird3_text += "\nFREEBIRD IS TOO FAST!!!";
                        } else {
                            freebird3_text += TextFormat("\nFREEBIRD IS AT VELOCITY: %.2f", car_kinematics.vel);
                        }
                        if(freebird_factor > 0.85){
                            freebird3_text += "\n!!!BOUNDARY COLLAPSE IMMINENT!!!";
                        }
                        if(freebird_factor > 0.9){
                            freebird3_text += "\n!!FREEBIRD ABOUT TO BREACH CONTAINMENT!!!";
                        }
                        if(freebird_factor > 0.99){
                            freebird3_text += "\n!!!BOUDARY COLLAPSING!!!";
                            for(int i = (int)((1-freebird_factor)*1000); i < 10; i++){
                                freebird3_text += "\n!!!FREEBIRD IS ESCAPING!!!";
                            }
                        }
                    }
                    const char* freebird3_text_cstr = freebird3_text.c_str();
                    DrawTextEx(GetFontDefault(), 
                    freebird3_text_cstr, 
                    Vector2(10,INITIAL_FONT_SIZE*2), 
                    INITIAL_FONT_SIZE/2, 
                    INITIAL_FONT_SPACING, 
                    Color{0,0,0,(255*std::min((freebird_volume*10.0/DEFAULT_VOLUME),1.0))});
                } 
            } 

        window.EndDrawing(); 
        
        
    }

    return 0;
}

