#include "input.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

KeymapEntry::KeymapEntry() {

}

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
}

Input::~Input() {}

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
	printf("released all keys\n");
	for (int i = 0; i < KEY_COUNT; i++) {
		keys[i] = KeymapEntry((const char) i);
	}
}

void Input::press(int key) {
	printf("pressed: %c\n", (char) key);
	keys[key].state = PRESS;
}

void Input::release(int key) {
	printf("released: %c\n", (char) key);
	keys[key].state = RELEASE;
}

void Input::hold(int key) {
	printf("holding: %c\n", (char) key);
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

void Input::moveMouse(const glm::vec2& v) {
	mouseDelta = v - mousePos;
	if (mouseReset.has_value()) {
		mousePos = mouseReset.value();
	} else {
		mousePos = v;
	}
	glfwSetCursorPos(window->getHandle(), (double) mousePos.x, (double) mousePos.y);
}

const glm::vec2& Input::getMousePos() const {
	return mousePos;
}

const glm::vec2& Input::getMouseDelta() const {
	return mouseDelta;
}

void Input::clearMouseDelta() {
	mouseDelta.x = 0.0f;
	mouseDelta.y = 0.0f;
}
