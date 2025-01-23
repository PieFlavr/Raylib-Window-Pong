#include "BufferedRaylib.hpp"

#include "raymath.h"

#include <set>
#include <string>
#include <string_view>
#include <map>
#include <array>
#include <concepts>

namespace raylib {

	bool Button::operator<(const Button& o) const {
		if (type != o.type) return type < o.type;
		if (type == Type::Gamepad && gamepad.id == o.gamepad.id)
			return gamepad.button < o.gamepad.button;
		return keyboard < o.keyboard; // They should all be castable to a keyboard key, thus comparing only it should be fine!
	}

	bool Button::IsPressed(const Button& button) {
		switch(button.type) {
		break; case Button::Type::Keyboard:
			return IsKeyDown(button.keyboard);
		break; case Button::Type::Mouse:
			return IsMouseButtonDown(button.mouse);
		break; case Button::Type::Gamepad: {
			return IsGamepadButtonDown(button.gamepad.id, button.gamepad.button);
		}
		break; default: assert(button.type != Button::Type::Invalid);
		}
		return false;
	}

	uint8_t Button::IsSetPressed(const std::set<Button>& buttons) {
		uint8_t state = 0;
		for(auto& button: buttons)
			state += IsPressed(button);
		return state;
	}

	Action& Action::operator=(Action&& o) {
		type = o.type;
		data = std::move(o.data);
		callback = std::move(o.callback);
		if(o.type == Type::Button) data.button.buttons = std::exchange(o.data.button.buttons, nullptr);
		else if(o.type == Type::MultiButton) data.multi.quadButtons = std::exchange(o.data.multi.quadButtons, nullptr);
		return *this;
	}

	void Action::PumpButton(std::string_view name) {
		assert(data.button.buttons);
		uint8_t state = Button::IsSetPressed(*data.button.buttons);
		if (state != data.button.last_state) {
			if(data.button.combo) {
				if(bool comboState = state == data.button.buttons->size(), lastComboState = data.button.last_state == data.button.buttons->size(); comboState != lastComboState)
					callback(name, {(float)comboState}, {(float)lastComboState});
			} else callback(name, {(float)state}, {(float)data.button.last_state});
			data.button.last_state = state;
		}
	}

	void Action::PumpAxis(std::string_view name) {
		float state = data.axis.last_state;
		switch(data.axis.type) {
		break; case Data::Axis::Type::Gamepad: {
			float movement = GetGamepadAxisMovement(data.axis.gamepad.id, data.axis.gamepad.axis);
			if(movement != 0) state += movement;
		}
		break; case Data::Axis::Type::MouseWheel: {
			float movement = GetMouseWheelMove();
			if(movement != 0) state += movement;
		}
		break; default: assert(data.axis.type != Data::Axis::Type::Invalid);
		}
		if (state != data.axis.last_state) {
			callback(name, {state}, {state - data.axis.last_state});
			data.axis.last_state = state;
		}
	}

	void Action::PumpVector(std::string_view name) {
		Vector2 state = data.vector.last_state;
		switch(data.vector.type) {
		break; case Data::Vector::Type::MouseWheel:
			state = GetMouseWheelMoveV();
		break; case Data::Vector::Type::MousePosition:
			state = GetMousePosition();
		// break; case Type::MouseDelta:
		//     state = GetMouseDelta();
		break; case Data::Vector::Type::GamepadAxes: {
			state.x += GetGamepadAxisMovement(data.vector.gamepad.horizontal.id, data.vector.gamepad.horizontal.axis);
			state.y += GetGamepadAxisMovement(data.vector.gamepad.vertical.id, data.vector.gamepad.vertical.axis);
		}
		break; default: assert(data.vector.type != Data::Vector::Type::Invalid);
		}
		if (!Vector2Equals(state, data.vector.last_state)) {
			callback(name, state, Vector2Subtract(state, data.vector.last_state));
			data.vector.last_state = state;
		}
	}

	Action Action::gamepad_axes(GamepadAxis horizontal /*= GAMEPAD_AXIS_LEFT_X*/, GamepadAxis vertical /*= GAMEPAD_AXIS_LEFT_Y*/, int gamepadHorizontal /*= 0*/, int gamepadVertical /*= -1*/) {
		if(gamepadVertical < 0) gamepadVertical = gamepadHorizontal;

		Action out = {Action::Type::Vector, {.vector = { Data::Vector::Type::GamepadAxes}}};
		out.data.vector.gamepad = {{gamepadHorizontal, horizontal}, {gamepadVertical, vertical}};
		return out;
	}

	void Action::PumpMultiButton(std::string_view name) {
		Vector2 state = data.multi.last_state;
		{
			auto type = data.multi.type;
			std::array<uint8_t, 4> buttonState;
			for(uint8_t i = 0; i < (type == Data::MultiButton::Type::QuadButtons ? 4 : 2); i++) {
				buttonState[i] = Button::IsSetPressed(data.multi.quadButtons->directions[i]);
				if(data.multi.quadButtons->normalize && buttonState[i] > 0)
					buttonState[i] = 1;
			}
			if(type == Data::MultiButton::Type::QuadButtons)
				state.x = buttonState[MultiButtonData<4>::Direction::Left] - buttonState[MultiButtonData<4>::Direction::Right];
			state.y = buttonState[MultiButtonData<4>::Direction::Up] - buttonState[MultiButtonData<4>::Direction::Down];

			if(type == Data::MultiButton::Type::ButtonPair)
				state.x = state.y;
		}
		if (!Vector2Equals(state, data.multi.last_state)) {
			callback(name, state, Vector2Subtract(state, data.multi.last_state));
			data.multi.last_state = state;
		}
	}

	void Action::PollEvents(std::string_view name) {
		switch(type){
		break; case Action::Type::Button:
			PumpButton(name);
		break; case Action::Type::Axis:
			PumpAxis(name);
		break; case Action::Type::Vector:
			PumpVector(name);
		break; case Action::Type::MultiButton:
			PumpMultiButton(name);
		break; default: assert(type != Action::Type::Invalid);
		}
	}

	void BufferedInput::PollEvents(bool whileUnfocused /*= false*/) {
		if(!whileUnfocused && !IsWindowFocused()) return;
		for(auto& [name, action]: actions)
			action.PollEvents(name);
	}
}