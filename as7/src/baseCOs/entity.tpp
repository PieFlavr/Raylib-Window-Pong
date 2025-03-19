#include "entity.hpp"
#include "component.hpp"
#include "behavior.hpp"

#include <memory>
#include <optional>
#include <vector>

namespace CO {

    template<std::derived_from<Component> T, typename... Ts>
    T& Entity::addComponent(Ts... args) {
        std::unique_ptr<Component> component = std::make_unique<T>(std::forward<Ts>(args)...);
        components.push_back(std::move(component));
        return *dynamic_cast<T*>(components.back().get());
    }

    template<std::derived_from<Component> T>
    void Entity::pushBackComponent(std::unique_ptr<T> component) {
        components.push_back(std::move(component)); 
        if(auto ptr = dynamic_cast<CO::Behavior*>(components.back().get())){ // Check if the component is a behavio
            ptr->setTarget(this); // Set the target of the behavior to this entity
            ptr->checkRequirements(); // Check if the behavior has the required components
        }
    }    
    
    template<std::derived_from<Component> T>
    std::optional<std::reference_wrapper<T>> Entity::getComponent() {
        for (const auto& component : components) {
            if (auto ptr = dynamic_cast<T*>(component.get())) {
                return std::ref(*ptr);
            }
        }
        return std::nullopt;
    }
    
    template<std::derived_from<Component> T>
    bool Entity::hasComponent() const {
        for (const auto& component : components) {
            if (dynamic_cast<T*>(component.get())) {
                return true;
            }
        }
        return false;
    }
    
    template<std::derived_from<Component> T>
    std::optional<std::unique_ptr<T>> Entity::removeComponent() {
        for (auto component = components.begin(); component != components.end(); ++component) {
            if (auto ptr = dynamic_cast<T*>(component->get())) { // vvv Derived -> Base is fine w/ std::move but Derived -> Base is not i guess, so we need to do this
                std::unique_ptr<T> removedComponent = std::unique_ptr<T>(static_cast<T*>(component->release())); // Release the ownership of the component, then downcast it to T
                components.erase(component);
                return removedComponent;
            }
        }
        return std::nullopt;
    }
    
} // namespace CO