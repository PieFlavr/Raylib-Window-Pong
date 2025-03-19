#pragma once

#include "raylib-cpp.hpp"

template<typename T> //Cool type validation!
concept Transformer = requires(T t, raylib::Matrix m) {
    { t(m) } -> std::convertible_to<raylib::Matrix>;
};

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
// Draw Model Functions
// ===========================================================

/**
 * @brief Draws a model with a transformation applied to it.
 * 
 * @param model The model to be drawn.
 * @param transformer A function or lambda that takes a raylib::Matrix and returns a transformed raylib::Matrix.
 */
void DrawBoundedModel(raylib::Model& model, Transformer auto transformer)
{   
    raylib::Matrix backup = model.transform;   
    model.transform = transformer(backup);  
    model.Draw({});
    model.GetTransformedBoundingBox().Draw(); 
    model.transform = backup;   
}

/**
 * @brief Draws a model with a transformation applied to it.
 * 
 * @param model The model to be drawn.
 * @param transformer A function or lambda that takes a raylib::Matrix and returns a transformed raylib::Matrix.
 */
void DrawUnboundedModel(raylib::Model& model, Transformer auto transformer)
{   
    raylib::Matrix backup = model.transform;   
    model.transform = transformer(backup);  
    model.Draw({});
    model.transform = backup;   
}