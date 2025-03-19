#pragma once //~This is cool and easier than #ifndef #define #endif

#define RAYLIB_SUPPORT_OIS
#include <raylib-cpp.hpp> //~Mostly to let it access fancy transform/matrix math + vector3 and degree types.

//~Pretty self-explanatory includes
#include <memory> //~For unique_ptr
#include <vector> //~For vector
#include <optional> //~For optional

namespace cs381 {

	/**
	 * @brief Base class for various components that can be attached to an entity.
	 */
	class Component { //~Basically an object wrapper for the Entity struct which let's programmer make derivative classes
		struct Entity* object; 	//~Spooky forward declaration of Entity struct? Pointer to the entity that this component is associated with
		friend struct Entity; 	//~Entity can (relatively) freely access the object pointer

	public:
		struct Entity& Object() { return *object; } 	//~Returns the entity struct that this component is associated with
		struct TransformComponent& Transform();			//~Spooky forward declaration of Transform Component? Defines Transform() to return a TransformComponent itself.
		bool enabled = true;							//~Whether or not the component is enabled, mainly for skipping logic/processing entirely

		Component(struct Entity& e) : object(&e) {}		//Constructor taking in an entity struct and setting the object pointer to it
		Component(struct Entity& e, bool enabled): object(&e), enabled(enabled) {} // Start //~Above but with the additional enabled flag
		virtual void Tick(float dt) {}					//~Virtual function for ticking the component, should be override by derivative classes for custom logic
		virtual ~Component() {} // Cleanup				//~Virtual destructor for cleanup, similar to the above, should prolly be careful to maintain recursive structure (if exists)

		template<std::derived_from<Component> T>		//~Template function for casting the component to classes derived from Component
		T& as() { return *dynamic_cast<T*>(this); }		//~Spooky dynamic cast to the derived class (didn't even know this was a thing wtf)
	};

	/**
	 * @brief Component storing the positional data of a Entity
	 */
	struct TransformComponent: public Component { //~Derivative of Component? Stores transform data for an entity.
		using Component::Component;				 //~Inherits the constructor from Component
		raylib::Vector3 position = {0, 0, 0};	 //~3D Entity position
		raylib::Degree heading = 0;				//~Entity heading (rotation), not sure exactly which axis this is around, will probably modify later
	};


	/**
	 * @brief Represents an object in the game world.
	 */
	struct Entity {
		std::vector<std::unique_ptr<Component>> components = {};	//~Vector of unique pointers to components, allows for dynamic component addition/removal ,
																	//~and also recusrive component structure (components containing components)
		Entity() { AddComponent<TransformComponent>(); } // Entities have a transform by default!
		Entity(const Entity&) = delete;								//~Delete implicit copy constructor so we don't copy/duplicate entities
		//~Presumably, this "moves" the components from one entity to another, but I'm not entirely sure how this works
		Entity(Entity&& other) : components(std::move(other.components)) {	//~Turns components in component list to workable references?
			for(auto& component: components)				//~For each component in the entity's component list
				component->object = this; 					// When we are moved make sure the components still point to us!
		}

		Entity& operator=(const Entity&) = delete;	 	//~Delete implicit copy assignment operator so we don't copy/duplicate entities
		//~Presumably, this "moves" the components from one entity to another, but I'm not entirely sure how this works
		Entity& operator=(Entity&& other) {				//~Redefine the assignment operator to move components from one entity to another
			components = std::move(other.components);	//~Turns components in component list to workable references?
			for(auto& component: components)			//~For each component in the entity's component list
				component->object = this; // When we are moved make sure the components still point to us!
			return *this;				//~Return the entity with the moved components
		}

		/**
		 * @brief Performs actions during each game update for the entity and its components.
		 * @param dt The time elapsed since the last update.
		 */
		void Tick(float dt) { 					//~Delta time -- we can mindlessly call this for any delta-related logic
			for(auto& componentPtr: components) //~For each component, individually tick each one
				if(componentPtr->enabled)     	//~If the component is enabled, tick it. Can be used arbitrarily for any purpose
					componentPtr->Tick(dt);		//~Actually tick the component
		}

		/**
		 * @brief Adds a new component of type T to the entity.
		 * @tparam T The type of component to add.
		 * @tparam Ts The types of arguments to pass to the component's constructor.
		 * @param args The arguments to pass to the component's constructor.
		 * @return The index of the added component.
		 */
		template<std::derived_from<Component> T, typename... Ts>	//~Template accepting arbitary number of derived components from Component 
		size_t AddComponent(Ts... args) {							 //~Add component returning component index
																	//~Accepts any number of components to pass to the component's constructor
			//~Applies to each added component in order to the entity
			std::unique_ptr<Component> component = std::make_unique<T>(*this, std::forward<Ts>(args) ...); //~Unique pointer to the component, created from the passed components
			components.push_back(std::move(component));	//~Push the component to the entity's component list
			return components.size() - 1; 				//~Return the index of the added component
		}

		// How could we remove a component?

		/**
		 * @brief Retrieves a component of type T from the entity.
		 * @tparam T The type of component to retrieve.
		 * @return An optional reference to the component if found, or std::nullopt if not found.
		 */
		template<std::derived_from<Component> T>					//~Template accepting derived component class
		std::optional<std::reference_wrapper<T>> GetComponent() {	//~Gets the component of the desired type
			if constexpr(std::is_same_v<T, TransformComponent>){ // This extra step here allows us to skip itterator overhead since we know the transform component should be in slot 0
				T* cast = dynamic_cast<T*>(components[0].get()); //~Another spooky dynamic cast (╯°□°）╯︵ ┻━┻ This time for templated unique component type handling.
				if(cast) return *cast;							//~If the cast is successful, i.e. component compatibly casts, return the casted component? (I think?)
			}												   //~Seems like its just an iteration skip if asking for transform component

			for(auto& componentPtr: components){				//~For each component in the entity's component list
				T* cast = dynamic_cast<T*>(componentPtr.get()); //~Check if the component is castable of the desired type
				if(cast) return *cast;							//~If the cast is successful, return the casted component
			}

			return {};		//~Otheriwse above, nothing-burger return.
		}

		/**
		 * @brief Retrieves this object's transform...
		 * @return The object's transform component.
		 * @note this function has undefined behavior (probably a crash...) if the transform component is ever removed!
		 */
		TransformComponent& Transform() { return *GetComponent<TransformComponent>(); } 	//~Returns the transform component of the entity. 
};																							//~Likely defined here as opposed to Entity() because no spooky forward declaration of TransformComponent???

	/**
	 * @brief Syntax sugar function that provides access to the object's transform component
	 * @return The object's transform component.
	 * @note this function has undefined behavior (probably a crash...) if the transform component is ever removed!
	 */
	TransformComponent& Component::Transform() { return Object().Transform(); }	//~Literally just a wrapper for the above function, but for the Component class
}