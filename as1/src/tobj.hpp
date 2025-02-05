/**
 * @file tobject.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-02-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef TOBJ_H
#define TOBJ_H

#include "raylib-cpp.hpp"

#include <iostream>
#include <vector>

class Tobj{
    std::string name;
    Vector2 position; 
    float rotation;

    Tobj* parent;
    std::vector<Tobj*> children;

    public:
        virtual void Update(float deltaTime);
        virtual void Draw();
        void addChild(Tobj* child);
};

#endif