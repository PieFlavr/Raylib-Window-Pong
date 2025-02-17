/**
 * @file as2.cpp
 * @author Lucas Pinto
 * @brief Demonstration of loading and manipulating 3D models using raylib-cpp an the raylib library.
 *        Assignment includes a car and rocket model, both of which can be manipulated using
 *        matrix transfromations. These transformations include translation, scaling, and rotation.
 *        As well as a combination of all three. 
 * 
 *         (2/16) UPDATE: All Extra Credit (EC) features have effectively been implemented, 
 *                     you can easily search for each via [CTRL+F] and looking for "EC Feature"
 *                     Also for some reason experiencing driver timeouts occasionally? Not sure as of the cause.
 *                     Likely because I'm using WSL2 as opposed to full Linux distro, prolly gonna get a virtual machine next time :p.
 * @version 0.1
 * @date 2025-02-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>

#include "raylib-cpp.hpp"
#include "skybox.hpp"

//#define WIRE_FRAME_COLOR WHITE //was for an old defunct implementation
#define DEFAULT_SCALE 30

#define LOGIC_SPEED 1
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
{   //Feature #3 - Function that Translates Models via Lambda (10 points)
    //Featre #13 - Thoroughly Comment the DrawBoundedModel() function (5 points)
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
    model.GetTransformedBoundingBox().Draw(); //Feature #4 - Draw Bounding Box (10 points)
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
// Main Function
// ===========================================================
int main()
{   
    double lastDrawTime = 0; //Deltas for controlling draw+logic rates
    double lastLogicTime = 0;
    double drawDelta = 1.0 / DRAW_FPS; 

    double spinAngle = 0; //Extra Credit Animation
    double spin_scale = 2.0;

    bool enabledExtraCredit = false; 

    raylib::Window window(800, 600, "CS381 - Assignment 2"); //Feature #1 - Create Titled Window (1 point)
    window.SetState(FLAG_WINDOW_RESIZABLE);
    
    // ===========================================================
    // Model Loading + Default Transforms
    // ===========================================================
    raylib::Model cube = raylib::Mesh::Cube(30, 30, 30).LoadModelFrom();
    raylib::Camera camera = raylib::Camera(
        {0, 120, 500}, 
        {0, 0, 0}, 
        {0, 1, 0}, 
        45
    ); //Feature #5 - Draw Camera at (0,120,500) (5 points)
    
    raylib::Model rocket = raylib::Model("meshes/rocketA.glb"); //Feature #2 - Load Rocket Model Once (10 points)
    rocket.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    raylib::Model car = raylib::Model("../../assets/Kenny Car Kit/sedan.glb"); //Feature #2 - Load Car Model Once (10 points)
    car.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    raylib::Model cow = raylib::Model("../../custom_assets/as2/Cow.glb");   // EC Feature #1 - Custom Internet Meshes (5 points)
    cow.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);    // From... https://www.fab.com/listings/fbd8c8b0-fc59-460e-adb8-c7ab8f19a6f9

    raylib::Model wheel = raylib::Model("../../assets/Kenny Car Kit/wheel-default.glb");
    wheel.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    cs381::SkyBox sky("textures/skybox.png");

    // EC Feature #4 - Appropriately Ambient Audio (5 points)
    raylib::AudioDevice defaultDevice; 

    raylib::Music ambience, music;
    music.Load("../../custom_assets/as2/Audio/Interstellar - Coward  Imperfect Lock  No Time for Caution.mp3");  
    // From... https://www.youtube.com/watch?v=7hkI43oPIxk&pp=ygUQZ29qbyBob25vcmVkIG9uZQ%3D%3D
    ambience.Load("../../custom_assets/as2/Audio/Brown Noise  Relax For 5 Minutes.mp3");  
    // And also... https://www.youtube.com/watch?v=4MiNGLBMaVI 
    
    music.SetVolume(DEFAULT_VOLUME);
    ambience.SetVolume(DEFAULT_VOLUME);

    music.Pause();
    ambience.Pause();

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
                    music.Pause();
                    ambience.Pause();
                } else {
                    music.Play();
                    ambience.Play();  
                }
            }

            if(enabledExtraCredit){
                spinAngle = fmod(spinAngle + (logicDelta*(log(currentTime)/log(spin_scale))), 360); //EC Feature #2  - Animate Models (5 points)
                // "MURRRRPHHHHHHHHHH!!!!!!!"
                music.Update();
                ambience.Update();
            } 

            auto rocket_F6_transform = combine(
                rotate({0, 1, 0}, (spinAngle)*DEG2RAD),
                translate({0, 0, 0})
            ); //Feature #6 - Rocket Located at (0,0,0) (10 points)

            auto rocket_F7_transform = combine( //Feature #7 - Rocket Located at (-100, 100, 0), scaled (1,-1,1), and yawed 180 degrees (10 points)
                rotate({0, 1, 0}, (180+spinAngle)*DEG2RAD), // i guess yaw is y-axis here??? 
                scale({1, -1, 1}),
                translate({-100, 100, 0})
            );

            auto car_F8_transform = combine(
                rotate({0, 1, 0}, (spinAngle)*DEG2RAD),
                translate({-200, 0, 0})
            ); //Feature #8 - Car Located at (-200,0,0) (10 points)

            auto car_F9_transform = combine( //Feature #9 - Car Located at (200,0,0), and yawed 90 degrees (10 points)
                rotate({0, 1, 0}, (90+spinAngle)*DEG2RAD),
                translate({200, 0, 0})
            );

            auto car_F10_transform = combine( //Feature #10 - Car Located at (100,100,0), scaled by (1,2,1), and yawed 90 degrees (10 points)
                rotate({0, 1, 0}, (270+spinAngle)*DEG2RAD),
                scale({1, 2, 1}),
                translate({100, 100, 0})
            );

            auto cow_transform = combine( 
                rotate({0, 1, 0}, (-spinAngle)*DEG2RAD),
                translate({0, -100, -100})
            );

        lastLogicTime = currentTime;


        // ===========================================================
        // Draw Block
        // ===========================================================
        
        window.BeginDrawing();

        if(currentTime - lastDrawTime >= drawDelta)
        {
            window.ClearBackground(BLACK);
            camera.BeginMode();
                sky.Draw();

                if(enabledExtraCredit){
                    DrawBoundedModel(cow, cow_transform); 
                    
                    // EC Feature #3 - Four Wheels that Move Along (5 points)
                    DrawBoundedModel(wheel, combine( translate({15, 0, 20}), car_F8_transform));
                    DrawBoundedModel(wheel, combine(rotate({0,1,0},180*DEG2RAD), translate({-15, 0, 20}), car_F8_transform));
                    DrawBoundedModel(wheel, combine( translate({15, 0, -20}), car_F8_transform));
                    DrawBoundedModel(wheel, combine(rotate({0,1,0},180*DEG2RAD), translate({-15, 0, -20}),  car_F8_transform));

                    DrawBoundedModel(wheel, combine( translate({15, 0, 20}), car_F9_transform));
                    DrawBoundedModel(wheel, combine(rotate({0,1,0},180*DEG2RAD), translate({-15, 0, 20}), car_F9_transform));
                    DrawBoundedModel(wheel, combine( translate({15, 0, -20}), car_F9_transform));
                    DrawBoundedModel(wheel, combine(rotate({0,1,0},180*DEG2RAD), translate({-15, 0, -20}),  car_F9_transform));
                    
                    DrawBoundedModel(wheel, combine( translate({15, 0, 20}), car_F10_transform));
                    DrawBoundedModel(wheel, combine(rotate({0,1,0},180*DEG2RAD), translate({-15, 0, 20}), car_F10_transform));
                    DrawBoundedModel(wheel, combine( translate({15, 0, -20}), car_F10_transform));
                    DrawBoundedModel(wheel, combine(rotate({0,1,0},180*DEG2RAD), translate({-15, 0, -20}),  car_F10_transform));
                } else {
                    
                }

                DrawBoundedModel(rocket, rocket_F6_transform); 
                DrawBoundedModel(rocket, rocket_F7_transform);
                DrawBoundedModel(car, car_F8_transform); 
                DrawBoundedModel(car, car_F9_transform);
                DrawBoundedModel(car, car_F10_transform);
                
            camera.EndMode();
            
            lastDrawTime = currentTime;
        }

        DrawTextEx(GetFontDefault(), "[Left Click] to Toggle Extra Credit Features", Vector2(10,10), INITIAL_FONT_SIZE/2, INITIAL_FONT_SPACING, WHITE);

        window.EndDrawing(); 
         
        
    }

    return 0;
}

