#pragma once

#include <optional>

#include <glm/vec2.hpp>
#include <functional>
#include <unordered_map>

#include "window.h"

enum KeyState {

	RELEASE,
	PRESS,
	HOLD,
};

struct KeymapEntry {
	KeyState state;
	std::function<void(float)> action;
	char symbol;

	KeymapEntry();
	KeymapEntry(const KeymapEntry& other);
	KeymapEntry(const char symbol);

	void setAction(std::function<void(float)> action);

	void operator()(float delta);

	void operator=(const KeymapEntry& other);
};

class Input {

	static const int KEY_COUNT = 512;
	KeymapEntry keys[KEY_COUNT];
	//KeyState keys[KEY_COUNT];

	static const int MOUSE_BUTTON_COUNT = 3;
	KeyState mouseButtons[MOUSE_BUTTON_COUNT];
	glm::vec2 mousePos;
	glm::vec2 mouseDelta = glm::vec2(0.0f, 0.0f);
	std::optional<glm::ivec2> mouseReset;
	std::function<void(float, float, float)> onMouseMove;

	Window* window = nullptr;

public:
	Input();
	~Input();
	
	void init(Window* window);

	void setAction(int key, std::function<void(float)> action);
	void setOnMouseMove(std::function<void(float, float, float)> onMouseMove);
	void execute(float delta);

	void releaseAllKeys();
	void releaseAllMouseButtons();

	void press(int key);
	void release(int key);
	void hold(int key);

	bool isPressed(int key) const;
	bool isReleased(int key) const;
	bool isHeld(int key) const;

	void pressMouseButton(int button);
	void releaseMouseButton(int button);

	bool isMouseButtonPressed(int button) const;
	bool isMouseButtonReleased(int button) const;

	void moveMouse(const glm::vec2& v);

	const glm::vec2& getMousePos() const;
	const glm::vec2& getMouseDelta() const;

	void clearMouseDelta();

	inline void setMousePos(const glm::vec2& v) {

		mousePos = v;
	}

	inline void setMouseReset(const glm::ivec2& v) {

		mouseReset = { v };
	}
};