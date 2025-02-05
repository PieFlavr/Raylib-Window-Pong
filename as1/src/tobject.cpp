/**
 * @file tobject.cpp
 * @author Lucas Pinto
 * @brief A rough implementation of "container" object that uses a tree-like recursive structure to 
 *        to store and organize components. Generally usable for UI or even Physics. 
 * @version 0.1
 * @date 2025-02-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "tobject.hpp"

class Tobject{
    std::string name;
    Vector2 position; 

    Tobject* parent;
    std::vector<Tobject*> children;
    
    private:

    public:
        virtual void Update(float deltaTime);
        virtual void Draw();
};
