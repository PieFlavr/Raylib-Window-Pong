#include "entity.hpp"

using namespace CO;

//=================================================================================================================
// CONSTRUCTOR/COPY LOGIC
//=================================================================================================================

/**
 * @brief Construct a new CO::Entity::Entity object with a deep copy of the other entity's components.
 * 
 * @param other The entity to copy from.
 */
Entity::Entity(const Entity& other) {
    for (const auto& component : other.components) {
        components.push_back(component->clone()); //clone() to avoid bad downcasts
    }
} 

/**
 * @brief Move construct a new Entity:: Entity object
 * 
 * @param other 
 */
Entity::Entity(Entity&& other) : components(std::move(other.components)) {
    components = std::move(other.components); //Move the components from the other entity to this one
}

/**
 * @brief Assignment operator for the Entity class. Clears the current components and deep copies the components from the other entity.
 * 
 * @param other The entity to copy from.
 * @return Entity& The current entity.
 */
Entity& Entity::operator=(const Entity& other) {
    if (this != &other) { 
        components.clear(); 
        for (const auto& component : other.components) {
            components.push_back(component->clone()); 
        }
    }
    return *this;
}

/**
 * @brief Move assignment operator for the Entity class. Moves the components from the other entity to this one.
 * 
 * @param other The entity to move from.
 * @return Entity& The current entity.
 */
Entity& Entity::operator=(Entity&& other) {
    if (this != &other) {
        components = std::move(other.components); //Move the components from the other entity to this one
    }
    return *this;
}

//=================================================================================================================
// ITERATION LOGIC
//=================================================================================================================

/**
 * @brief 
 * 
 * @param deltaTime 
 */
void Entity::Tick(const float& deltaTime) {
    for (auto& component : components) {
        if (component->isEnabled()) {
            component->Tick(deltaTime);
        }
    }
}

/**
 * @brief 
 * 
 */
void Entity::Update(){
    for (auto& component : components) {
        if (component->isEnabled()) {
            component->Update();
        }
    }
}

// ====================================================================================================
// CLEAN UP
// ====================================================================================================
Entity::~Entity(){
    components.clear(); // Clear the components vector
}
