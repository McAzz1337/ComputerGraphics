#include "input.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

KeymapEntry::KeymapEntry() : state(RELEASE), action([](float) { printf("null\n"); }), symbol('?') {}

KeymapEntry::KeymapEntry(const KeymapEntry& other) {
	this->symbol = other.symbol;
	this->state = other.state;
	this->action = other.action;
}

KeymapEntry::KeymapEntry(const char symbol) : symbol(symbol) {
	state = RELEASE;
	action = [](float) { printf("null");  };

}
void KeymapEntry::operator()(float delta) {
	action(delta);
}

void KeymapEntry::setAction(std::function<void(float)> action) {
	this->action = action;
}

void KeymapEntry::operator=(const KeymapEntry& other) {
	this->symbol = other.symbol;
	this->state = other.state;
	this->action = other.action;
}

Input::Input() {
	releaseAllKeys();
	releaseAllMouseButtons();
}

Input::~Input() {}

void Input::init(Window* window) {
	this->window = window;
	double x;
	double y;
	glfwGetCursorPos(window->getHandle(), &x, &y);
	mousePos = glm::vec2((float) x, (float) y);
	clearMouseDelta();
}

void Input::setAction(int key, std::function<void(float)> action) {
	keys[key].setAction(action);
}

void Input::setOnMouseMove(std::function<void(float, float, float)> onMouseMove) {
	this->onMouseMove = onMouseMove;
}

void Input::execute(float delta) {
	for (auto& key : keys) {
		if (key.state != RELEASE) {
			key(delta);
		}
	}
	glm::vec2 mx = getMouseDelta();
	onMouseMove(delta, mx.x, mx.y);
}

void Input::releaseAllKeys() {
	for (int i = 0; i < KEY_COUNT; i++) {
		keys[i] = KeymapEntry((const char) i);
	}
}

void Input::releaseAllMouseButtons() {
	for (int i = 0; i < MOUSE_BUTTON_COUNT; i++) {
		mouseButtons[i] = RELEASE;
	}
}


void Input::press(int key) {
	keys[key].state = PRESS;
}

void Input::release(int key) {
	keys[key].state = RELEASE;
}

void Input::hold(int key) {
	keys[key].state = HOLD;
}

bool Input::isPressed(int key) const {
	return keys[key].state == PRESS;
}

bool Input::isReleased(int key) const {
	return keys[key].state == RELEASE;
}

bool Input::isHeld(int key) const {
	return  keys[key].state == HOLD || keys[key].state == PRESS;
}


void Input::pressMouseButton(int button) {
	mouseButtons[button] = PRESS;
}
void  Input::releaseMouseButton(int button) {
	mouseButtons[button] = RELEASE;
}

bool Input::isMouseButtonPressed(int button) const {
	return mouseButtons[button] == PRESS;
}

bool Input::isMouseButtonReleased(int button) const {
	return mouseButtons[button] == RELEASE;
}


void Input::moveMouse(const glm::vec2& v) {
	if (mouseReset.has_value()) {
		mousePos = mouseReset.value();
	} else {
		if (v != mousePos) {
			mouseDelta = v - mousePos;
			mousePos = v;
		} else {
			mouseDelta = glm::vec2(0.0f);
		}
	}
}

const glm::vec2& Input::getMousePos() const {
	return mousePos;
}

const glm::vec2& Input::getMouseDelta() const {
	return mouseDelta;
}

void Input::clearMouseDelta() {
	mouseDelta = glm::vec2(0.0f);
}
