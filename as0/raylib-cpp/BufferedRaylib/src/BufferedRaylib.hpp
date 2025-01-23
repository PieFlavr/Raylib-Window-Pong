/**
 * @file BufferedInput.hpp
 * @brief Contains definitions for buffered input handling and actions based off raylib
 */

#include "raylib.h"
#include "../FastSignals/libfastsignals/include/signal.h"

#include <set>
#include <string>
#include <string_view>
#include <map>
#include <array>
#include <concepts>

namespace raylib {

	template<typename F>
	struct Delegate {};

	template<typename Ret, typename... Args>
	struct Delegate<Ret(Args...)>: public is::signals::signal<Ret(Args...)> {
		using callback_type = typename is::signals::signal<Ret(Args...)>::slot_type;

		Delegate& operator+=(callback_type callback) { this->connect(callback); return *this; }

		void set(callback_type callback) {
			this->disconnect_all_slots();
			this->connect(callback);
		}
		Delegate& operator=(callback_type callback) { set(callback); return *this; }
	};

	/**
	 * @brief Represents various input button types, including keyboard keys, mouse buttons, and gamepad buttons.
	 */
	struct Button {
		// Enumerates different button types.
		enum class Type {
			Invalid = 0,
			Keyboard,
			Mouse,
			Gamepad
		} type;

		// Union to store different types of button configurations (raylib enums).
		// NOTE: it is recommended to not directly manipulate buttons but to instead use one of the following factory functions
		union {
			KeyboardKey keyboard;
			MouseButton mouse;
			struct {
				int id;
				GamepadButton button;
			} gamepad;
		};

		/**
		 * @brief Overloaded less-than operator for comparing buttons.
		 * @param o The button to compare against.
		 * @return True if this button is less than the given button, false otherwise.
		 */
		bool operator<(const Button& o) const;

		/**
		 * @brief Checks if the specified button is currently pressed.
		 * @param button The button to check.
		 * @return True if the button is pressed, false otherwise.
		 */
		static bool IsPressed(const Button& button);

		/**
		 * @brief Checks if a set of buttons is pressed.
		 * @param buttons The set of buttons to check.
		 * @return The state of the buttons (number of buttons pressed) in the set.
		 */
		static uint8_t IsSetPressed(const std::set<Button>& buttons);

		// Static helper functions to create Button objects for different input types.
		/**
		 * @brief Creates a button associated with a keyboard key
		 * 
		 * @param key the key
		 * @return Button 
		 */
		static Button key(KeyboardKey key) { return { Type::Keyboard, key}; }

		/**
		 * @brief Creates a button associated with a mouse buttion
		 * 
		 * @param button the button
		 * @return Button 
		 */
		static Button btn(MouseButton button) { return { Type::Mouse, {.mouse = button}}; }
		/**
		 * @brief Creates a button associated with a mouse buttion
		 * 
		 * @param button the button
		 * @return Button 
		 * @note The same as btn
		 */
		static Button mouse_button(MouseButton button) { return btn(button); }

		/**
		 * @brief Creates a button associated with a gamepad buttion
		 * 
		 * @param button the button
		 * @param gamrpad the gamepad the button is associated with (default 0)
		 * @return Button 
		 */
		static Button pad(GamepadButton button, int gamepad = 0) { return { Type::Keyboard, {.gamepad = {gamepad, button}}}; }
		/**
		 * @brief Creates a button associated with a gamepad buttion
		 * 
		 * @param button the button
		 * @param gamrpad the gamepad the button is associated with (default 0)
		 * @return Button 
		 * @note same as joy
		 */
		static Button joy(GamepadButton button, int gamepad = 0) { return pad(button, gamepad); }
		/**
		 * @brief Creates a button associated with a gamepad buttion
		 * 
		 * @param button the button
		 * @param gamrpad the gamepad the button is associated with (default 0)
		 * @return Button 
		 * @note same as joy
		 */
		static Button gamepad_button(GamepadButton button, int gamepad = 0) { return pad(button, gamepad); }
	};

	// Typedef for a set of buttons.
	using ButtonSet = std::set<Button>;

	/**
	 * @brief Represents various input action types, such as buttons, axes (controller trigger, mouse wheel), vectors (mouse, thumb stick), and multi-button combinations.
	 */
	struct Action {
		// Enumerates different action types.
		enum class Type {
			Invalid = 0,
			Button,
			Axis,
			Vector,
			MultiButton
		} type;

		/**
		 * @brief struct combining a gamepad and axis enum and id together
		 */
		struct Gamepad {
			int id;
			GamepadAxis axis;
		};

		/**
		 * @brief Struct holding configuration data for multibutton actions
		 *
		 * @tparam N the number of directions this state supports (default 4)
		 */
		template<size_t N>
		struct MultiButtonData {
			enum Direction {
				Up, Down, Left, Right,
				UpLeft, UpRight, DownLeft, DownRight
			};
			std::array<ButtonSet, N> directions;
			std::array<uint8_t, N> lasts;
			bool normalize = true; // When true the maximum value returned for a given axis is 1, if false the value of each direction will be the sum of the buttons pressed pointing one direction minus the sum of the pressed buttons pointing the other direction
		};

		// Union to store different types of action data.
		// NOTE: It is recommended that you don't try to mess with these values yourself, instead use one of the factory functions below
		// NOTE: Every inner type has a variable called last_state which holds the state as of the last time this action was pumped! 
		//	Can be quired externally if nessicary... but ensure that you are accessing data of the correct type!
		union Data {
			struct Button {
				ButtonSet* buttons;
				bool combo = false; // When true all buttons in the set must be pressed for the action to trigger
				uint8_t last_state = 0; // TODO: can we add a function to access the last state?
			} button;

			struct Axis {
				enum class Type {
					Invalid = 0,
					Gamepad,
					MouseWheel
				} type;

				Gamepad gamepad;
				float last_state = 0;
			} axis;

			struct Vector {
				enum class Type {
					Invalid = 0,
					MouseWheel,
					MousePosition,
					// MouseDelta,
					GamepadAxes,
				} type;

				struct GamepadAxes{
					Gamepad horizontal;
					Gamepad vertical;
				} gamepad;
				Vector2 last_state = { 0, 0 };
			} vector;

			struct MultiButton {
				enum class Type {
					Invalid = 0,
					ButtonPair,
					QuadButtons,
				} type;

				MultiButtonData<4>* quadButtons;
				Vector2 last_state;
			} multi;
		} data;

		// Callback invoked when the action is triggered
		Delegate<void(const std::string_view name, Vector2 state, Vector2 delta)> callback;

		// Constructors and destructor for the Action class.
		Action() : type(Type::Invalid), data({.button = {}}) {}
		~Action() {
			if(type == Type::Button && data.button.buttons) delete data.button.buttons;
			else if(type == Type::MultiButton && data.multi.quadButtons) delete data.multi.quadButtons;
		}
		Action(Type t, Data d = {.button = {nullptr, false, 0}}) : type(t), data(d) {}
		Action(const Action&) = delete;
		Action(Action&& o) : Action() { *this = std::move(o); }
		Action& operator=(const Action&) = delete;
		Action& operator=(Action&& o);

		// Member functions to add and set callback functions for the action (vector overloads).
		Action& AddCallbackNamed(is::signals::signal<void(const std::string_view name, Vector2 state, Vector2 delta)>::slot_type callback) {
			this->callback.connect(callback);
			return *this;
		}
		Action& SetCallbackNamed(is::signals::signal<void(const std::string_view name, Vector2 state, Vector2 delta)>::slot_type callback) {
			this->callback.disconnect_all_slots();
			return AddCallbackNamed(callback);
		}
		Action& AddCallback(is::signals::signal<void(Vector2 state, Vector2 delta)>::slot_type callback) {
			return AddCallbackNamed((is::signals::signal<void(const std::string_view name, Vector2 state, Vector2 delta)>::slot_type)
				[callback = std::move(callback)](const std::string_view name, Vector2 state, Vector2 delta){
					callback(state, delta);
				});
		}
		Action& SetCallback(is::signals::signal<void(Vector2 state, Vector2 delta)>::slot_type callback) {
			return SetCallbackNamed((is::signals::signal<void(const std::string_view name, Vector2 state, Vector2 delta)>::slot_type)
				[callback = std::move(callback)](const std::string_view name, Vector2 state, Vector2 delta){
					callback(state, delta);
				});
		}

		// Member functions to add and set callback functions for the action (float overloads).
		Action& AddCallbackNamed(is::signals::signal<void(const std::string_view name, float state, float delta)>::slot_type callback) {
			return AddCallbackNamed(
				(is::signals::signal<void(const std::string_view name, Vector2 state, Vector2 delta)>::slot_type)
				[callback](const std::string_view name, Vector2 state, Vector2 delta) {
					callback(name, state.x, delta.x);
				});
		}
		Action& SetCallbackNamed(is::signals::signal<void(const std::string_view name, float state, float delta)>::slot_type callback) {
			this->callback.disconnect_all_slots();
			return AddCallbackNamed(callback);
		}
		Action& AddCallback(is::signals::signal<void(float state, float delta)>::slot_type callback) {
			return AddCallbackNamed([callback = std::move(callback)](const std::string_view name, float state, float delta){
				callback(state, delta);
			});
		}
		Action& SetCallback(is::signals::signal<void(float state, float delta)>::slot_type callback) {
			return SetCallbackNamed([callback = std::move(callback)](const std::string_view name, float state, float delta){
				callback(state, delta);
			});
		}

		// Member functions to add and set callback functions for the action (button pressed overloads).
		Action& AddPressedCallbackNamed(is::signals::signal<void(const std::string_view name)>::slot_type callback) {
			return AddCallbackNamed(
				(is::signals::signal<void(const std::string_view name, Vector2 state, Vector2 delta)>::slot_type)
				[callback](const std::string_view name, Vector2 state, Vector2 delta) {
					if(state.x) callback(name);
				});
		}
		Action& SetPressedCallbackNamed(is::signals::signal<void(const std::string_view name)>::slot_type callback) {
			this->callback.disconnect_all_slots();
			return AddPressedCallbackNamed(callback);
		}
		Action& AddPressedCallback(is::signals::signal<void()>::slot_type callback) {
			return AddCallbackNamed(
				(is::signals::signal<void(const std::string_view name, Vector2 state, Vector2 delta)>::slot_type)
				[callback](const std::string_view name, Vector2 state, Vector2 delta) {
					if(state.x) callback();
				});
		}
		Action& SetPressedCallback(is::signals::signal<void()>::slot_type callback) {
			this->callback.disconnect_all_slots();
			return AddPressedCallback(callback);
		}
		// Member functions to add and set callback functions for the action (button released overloads).
		Action& AddReleasedCallbackNamed(is::signals::signal<void(const std::string_view name)>::slot_type callback) {
			return AddCallbackNamed(
				(is::signals::signal<void(const std::string_view name, Vector2 state, Vector2 delta)>::slot_type)
				[callback](const std::string_view name, Vector2 state, Vector2 delta) {
					if(!state.x) callback(name);
				});
		}
		Action& SetReleasedCallbackNamed(is::signals::signal<void(const std::string_view name)>::slot_type callback) {
			this->callback.disconnect_all_slots();
			return AddReleasedCallbackNamed(callback);
		}
		Action& AddReleasedCallback(is::signals::signal<void()>::slot_type callback) {
			return AddCallbackNamed(
				(is::signals::signal<void(const std::string_view name, Vector2 state, Vector2 delta)>::slot_type)
				[callback](const std::string_view name, Vector2 state, Vector2 delta) {
					if(!state.x) callback();
				});
		}
		Action& SetReleasedCallback(is::signals::signal<void()>::slot_type callback) {
			this->callback.disconnect_all_slots();
			return AddReleasedCallback(callback);
		}

		/**
		 * @brief Action that is invoked whenever the abstract button is pressed.
		 * Callback signature: [](const std::string_view name, uint8_t pressed, bool wasPressed) -> void;
		 * The value in pressed represents how many buttons in the set are currently pressed
		 *
		 * @param button the button to listen for
		 * @param combo wether all of the buttons in the set need to be pressed for a trigger (defaults to only a single button needing to be pressed)
		 * @return Action
		 */
		static Action button(Button button, bool combo = false) { return Action{Action::Type::Button, Action::Data{ .button = {new ButtonSet{button}, combo}}}; }

		/**
		 * @brief Action that is invoked whenever the keyboard key is pressed.
		 * Callback signature: [](const std::string_view name, uint8_t pressed, bool wasPressed) -> void;
		 * The value in pressed represents how many buttons in the set are currently pressed
		 *
		 * @param key the key to listen for
		 * @param combo wether all of the buttons in the set need to be pressed for a trigger (defaults to only a single button needing to be pressed)
		 * @return Action
		 */
		static Action key(KeyboardKey key, bool combo = false) { return button({ Button::Type::Keyboard, key}, combo); }

		/**
		 * @brief Action that is invoked whenever the mouse button is pressed.
		 * Callback signature: [](const std::string_view name, uint8_t pressed, bool wasPressed) -> void;
		 * The value in pressed represents how many buttons in the set are currently pressed
		 *
		 * @param b the button to listen for
		 * @param combo wether all of the buttons in the set need to be pressed for a trigger (defaults to only a single button needing to be pressed)
		 * @return Action
		 */
		static Action mouse_button(MouseButton b, bool combo = false) { return button({ Button::Type::Mouse, {.mouse = b}}, combo); }

		/**
		 * @brief Action that is invoked whenever the gamepad button is pressed.
		 * Callback signature: [](const std::string_view name, uint8_t pressed, bool wasPressed) -> void;
		 * The value in pressed represents how many buttons in the set are currently pressed
		 *
		 * @param b the button to listen for
		 * @param gamepad id of the gamepad to listen for (default 0)
		 * @param combo wether all of the buttons in the set need to be pressed for a trigger (defaults to only a single button needing to be pressed)
		 * @return Action
		 */
		static Action pad(GamepadButton b, int gamepad = 0, bool combo = false) { return button({ Button::Type::Keyboard, {.gamepad = {gamepad, b}}}, combo); }

		/**
		 * @brief Action that is invoked whenever the gamepad button is pressed.
		 * Callback signature: [](const std::string_view name, uint8_t pressed, bool wasPressed) -> void;
		 * The value in pressed represents how many buttons in the set are currently pressed
		 *
		 * @param b the button to listen for
		 * @param gamepad id of the gamepad to listen for (default 0)
		 * @param combo wether all of the buttons in the set need to be pressed for a trigger (defaults to only a single button needing to be pressed)
		 * @return Action
		 * @note same as pad
		 */
		static Action joy(GamepadButton b, int gamepad = 0, bool combo = false) { return pad(b, gamepad, combo); }

		/**
		 * @brief Action that is invoked whenever the gamepad button is pressed.
		 * Callback signature: [](const std::string_view name, uint8_t pressed, bool wasPressed) -> void;
		 * The value in pressed represents how many buttons in the set are currently pressed
		 *
		 * @param b the button to listen for
		 * @param gamepad id of the gamepad to listen for (default 0)
		 * @param combo wether all of the buttons in the set need to be pressed for a trigger (defaults to only a single button needing to be pressed)
		 * @return Action
		 * @note same as pad
		 */
		static Action gamepad_button(GamepadButton b, int gamepad = 0, bool combo = false) { return pad(b, gamepad, combo); }

		/**
		 * @brief Action that is invoked whenever the one of the collection of buttons are pressed.
		 * Callback signature: [](const std::string_view name, uint8_t pressed, bool wasPressed) -> void;
		 * The value in pressed represents how many buttons in the set are currently pressed
		 *
		 * @param buttons the set of buttons to listen for
		 * @param combo wether all of the buttons in the set need to be pressed for a trigger (defaults to only a single button needing to be pressed)
		 * @return Action
		 * @note same as pad
		 */
		static Action button_set(ButtonSet buttons = {}, bool combo = false) { return Action{Action::Type::Button, Action::Data{ .button = { new ButtonSet(buttons), combo }}}; }

		/**
		 * @brief Action that is invoked whenever the gamepad axis (usually triggers) is de/pressed.
		 * Callback signature: [](const std::string_view name, float value, float delta) -> void
		 *
		 * @param axis the id of axis to listen for
		 * @param gamepad id of the gamepad to listen for (default 0)
		 * @return Action
		 */
		static Action gamepad_axis(GamepadAxis axis = GAMEPAD_AXIS_LEFT_X, int gamepad = 0) {
			return {Action::Type::Axis, {.axis = {Data::Axis::Type::Gamepad, {gamepad, axis}}}};
		}

		/**
		 * @brief Action that is invoked whenever the mouse wheel is adjusted.
		 * Callback signature: [](const std::string_view name, float value, float delta) -> void
		 *
		 * @return Action
		 */
		static Action mouse_wheel() {
			return {Action::Type::Axis, {.axis = { Data::Axis::Type::MouseWheel }}};
		}

		/**
		 * @brief Action that combines button sets pointing in 2 opposing directions into a signed value which represents the direction of the currently pressed buttons.
		 * Callback signature: [](const std::string_view name, float dir, float delta) -> void
		 *
		 * @param positive set of keys to represent the positive direction
		 * @param negative set of keys to represent the negative direction
		 * @param normalized normally if there is more than one button in a set, the value will grow to reflect how many buttons are pushed. 
		 * 	While true the absolute value will never excede 1.
		 * @return Action
		 */
		static Action button_axis(ButtonSet positive, ButtonSet negative, bool normalize = true) {
			Action out{Action::Type::MultiButton};
			out.data.multi.type = Data::MultiButton::Type::ButtonPair;
			out.data.multi.quadButtons = new MultiButtonData<4>{{ positive, negative }, {}, normalize};
			return out;
		}

		/**
		 * @brief Action that combines button sets pointing in 2 opposing directions into a signed value which represents the direction of the currently pressed buttons.
		 * Callback signature: [](const std::string_view name, float dir, float delta) -> void
		 *
		 * @param left set of keys to represent left (negative) axis
		 * @param right set of keys to represent right (positive) axis
		 * @param normalized normally if there is more than one button in a set, the value will grow to reflect how many buttons are pushed. 
		 * 	While true the absolute value will never excede 1.
		 * @return Action
		 */
		static Action button_pair(ButtonSet left, ButtonSet right, bool normalize = true) {
			return button_axis(left, right, normalize);
		}

		/**
		 * @brief Action that is invoked whenever the mouse wheel is adjusted (also supports wheels that can tilt side to side).
		 * Callback signature: [](const std::string_view name, Vector2 dir, Vector2 delta) -> void
		 *
		 * @return Action
		 */
		static Action mouse_wheel_vector() {
			return {Action::Type::Vector, {.vector = { Data::Vector::Type::MouseWheel }}};
		}

		/**
		 * @brief Action that is invoked whenever the mouse moves.
		 * Callback signature: [](const std::string_view name, Vector2 dir, Vector2 delta) -> void
		 *
		 * @return Action
		 */
		static Action mouse_position() {
			return {Action::Type::Vector, {.vector = { Data::Vector::Type::MousePosition }}};
		}

		/**
		 * @brief Action that merges two seperate gamepad axis into a single vector.
		 * Callback signature: [](const std::string_view name, Vector2 dir, Vector2 delta) -> void
		 *
		 * @param horizontal the id of axis to listen for horizontal (x) data
		 * @param vertical the id of axis to listen for vertical (y) data
		 * @param gamepadHorizontal id of the gamepad to listen for in the horizontal axis (default 0)
		 * @param gamepadVertical id of the gamepad to listen for in the vertical axis. If its value is less than 0, it will copy the horizontal axis (default -1)
		 * @return Action
		 */
		static Action gamepad_axes(GamepadAxis horizontal = GAMEPAD_AXIS_LEFT_X, GamepadAxis vertical = GAMEPAD_AXIS_LEFT_Y, int gamepadHorizontal = 0, int gamepadVertical = -1);

		/**
		 * @brief Action that combines button sets pointing in 4 cardinal directions into a vector which represents the direction of the currently pressed buttons.
		 * 	ex. If the up and left buttons are pressed, the resulting vector will be pointing upward and leftward.
		 * Callback signature: [](const std::string_view name, Vector2 dir, Vector2 delta) -> void
		 *
		 * @param up set of keys to represent up (+y) axis
		 * @param down set of keys to represent down (-y) axis
		 * @param left set of keys to represent left (-x) axis
		 * @param right set of keys to represent right (+x) axis
		 * @param normalized normally if there is more than one button in a set, the vector's length will grow to reflect how many buttons are pushed. 
		 * 	While true none of the vector's axis will ever excede 1.
		 * @note The resulting vector itself will not be normalized! If you need it to have a length of 1 you will be on your own...
		 * @return Action
		 */
		static Action quad(ButtonSet up, ButtonSet down, ButtonSet left, ButtonSet right, bool normalized = true) {
			Action out{Action::Type::MultiButton};
			out.data.multi.type = Data::MultiButton::Type::QuadButtons;
			out.data.multi.quadButtons = new MultiButtonData<4>{{ up, down, left, right }, {}, normalized};
			return out;
		}

		/**
		 * @brief Action that combines button sets pointing in 4 cardinal directions into a vector which represents the direction of the currently pressed buttons.
		 * 	ex. If the up and left buttons are pressed, the resulting vector will be pointing upward and leftward.
		 * Callback signature: [](const std::string_view name, Vector2 dir, Vector2 delta) -> void
		 *
		 * @note This function is the same as quad, but the parameter order is changed to represent the common wasd convetion (and defaults are set to support it)
		 *
		 * @param up set of keys to represent up (+y) axis (default up arrow and w)
		 * @param left set of keys to represent left (-x) axis (default left arrow and a)
		 * @param down set of keys to represent down (-y) axis (default down arrow and s)
		 * @param right set of keys to represent right (+x) axis (default right arrow and d)
		 * @param normalized when true the resulting vector is normalized so that it always has a length of one (default true)
		 * @return Action
		 */
		static Action wasd(
			ButtonSet up = {Button::key(KEY_W), Button::key(KEY_UP)},
			ButtonSet left = {Button::key(KEY_A), Button::key(KEY_LEFT)},
			ButtonSet down = {Button::key(KEY_S), Button::key(KEY_DOWN)},
			ButtonSet right = {Button::key(KEY_D), Button::key(KEY_RIGHT)},
			bool normalized = true
		) { return quad(up, down, left, right, normalized); }


		/**
		 * @brief Helper function which moves the action, equivalent to calling std::move but can be chained for convenience.
		 * 	This function is nessicary since delegates (and thus actions) can't be copied, only moved.
		 * @note After calling this function "this" will have been moved into the reciever and shouldn't be accessed anymore!
		 *
		 * @return Action&& moveable reference to the action
		 */
		Action&& Move() { return std::move(*this); }
		Action&& move() { return std::move(*this); }

		/**
		 * @brief Function which updates the state of the action and invokes the callback if a change occured.
		 * @note Automatically called by BufferedInput so there usually isn't a need to manually call this function!
		 * 
		 * @param name the name of this action to pass through to the callback
		 */
		void PollEvents(std::string_view name);

	protected:
		friend struct BufferedInput;

		// Functions which get called by BufferedInput to process actions
		void PumpButton(std::string_view name);
		void PumpAxis(std::string_view name);
		void PumpVector(std::string_view name);
		void PumpMultiButton(std::string_view name);
	};


	/**
	 * @brief InputManager which is responsible for a map of actions and updating their values
	 */
	struct BufferedInput {
		// Map associating names with actions
		std::map<std::string, Action> actions;

		// Access to the action map via the input object itself
		Action& operator[](const std::string& key) {
			return actions[key];
		}

		// Function which updates the state of all actions in the `actions` map.
		void PollEvents(bool whileUnfocused = false);
	};

}