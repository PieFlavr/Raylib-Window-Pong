/**
 * @file as2.cpp
 * @author Lucas Pinto
 * @brief Demonstration of loading and manipulating 3D models using raylib-cpp an the raylib library.
 *        Assignment includes a car and rocket model, both of which can be manipulated using
 *        matrix transfromations. These transformations include translation, scaling, and rotation.
 *        As well as a combination of all three. 
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
                                // Also just realized, a lot of properties of objects we tend to write with respect to a "global" frame of reference.
                                // Resetting the matrix lends to that global paradigm rather than sequential local transforms. 
                                // That and its how most people think ¯\_(ツ)_/¯
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
    
    raylib::Model rocket_F6 = raylib::Model("meshes/rocketA.glb");
    rocket_F6.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    raylib::Model rocket_F7 = raylib::Model("meshes/rocketA.glb");
    rocket_F7.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    raylib::Model car_F8 = raylib::Model("../../assets/Kenny Car Kit/sedan.glb"); //Feature #2 - Load Car Model (10 points)
    car_F8.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    raylib::Model car_F9 = raylib::Model("../../assets/Kenny Car Kit/sedan.glb"); 
    car_F9.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    raylib::Model car_F10 = raylib::Model("../../assets/Kenny Car Kit/sedan.glb");
    car_F10.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    cs381::SkyBox sky("textures/skybox.png");

    while (!window.ShouldClose())
    {
        double currentTime = GetTime();
        
        // ===========================================================
        // Logic Block
        // ===========================================================
        double logicDelta = lastLogicTime - currentTime;

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

                auto rocket_F7_transform = combine( //Feature #7 - Rocket Located at (-100, 100, 0), scaled (1,-1,1), and yawed 180 degrees (10 points)
                    rotate({0, 1, 0}, 180*DEG2RAD), // i guess yaw is y-axis here??? 
                    scale({1, -1, 1}),
                    translate({-100, 100, 0})
                );

                auto car_F9_transform = combine( //Feature #9 - Car Located at (200,0,0), and yawed 90 degrees (10 points)
                    rotate({0, 1, 0}, 90*DEG2RAD),
                    translate({200, 0, 0})
                );

                auto car_F10_transform = combine( //Feature #10 - Car Located at (100,100,0), scaled by (1,2,1), and yawed 90 degrees (10 points)
                    rotate({0, 1, 0}, 90*DEG2RAD),
                    scale({1, 2, 1}),
                    translate({100, 100, 0})
                );


                DrawBoundedModel(rocket_F6, translate({0,0,0})); //Feature #6 - Rocket Located at (0,0,0) (10 points)
                DrawBoundedModel(rocket_F7, rocket_F7_transform);
                DrawBoundedModel(car_F8, translate({-200,0,0})); //Feature #8 - Car Located at (-200,0,0) (10 points)
                DrawBoundedModel(car_F9, car_F9_transform);
                DrawBoundedModel(car_F10, car_F10_transform);
                
            camera.EndMode();

            lastDrawTime = currentTime;
        }

        window.EndDrawing(); 
         
        
    }

    return 0;
}

