/**
 * @file Tobj.cpp
 * @author Lucas Pinto
 * @brief A rough implementation of "container" object that uses a tree-like recursive structure to 
 *        to store and organize components. Generally usable for UI or even Physics. 
 * @version 0.1
 * @date 2025-02-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "tobj.hpp"

void Tobj::Update(float deltaTime){
    for(auto * child : children){
        child->Update(deltaTime);
    }
}

void Tobj::Draw(){
    for(auto* child : children){
        child->Draw();
    }
}

void Tobj::addChild(Tobj* child){
    child->parent = this;
    children.push_back(child);
}

