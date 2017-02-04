#ifndef INPUT
#define INPUT

#include <glm\glm.hpp>

struct Controller
{
	bool up;
	bool down;
	bool left;
	bool right;
};

struct Input
{
	bool KeyDown[255];
	bool Key[255];
	bool KeyUp[255];
	bool MouseButtonDown[3];
	bool MouseButton[3];
	bool MouseButtonUp[3];
	glm::vec2 MouseMoveDistance;
	glm::vec2 CentrePos;
	Controller controller;
	void ResetInput();
};



enum MouseButton
{
	MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE
};

enum Key
{
	Q, W, E, R, T, Y, U, I, O, P, A, S, D, F, G, H, J, K, L, Z, X, C, V, B, N, M,
	ESCAPE, SPACE, RETURN, DEL, BACKSPACE, UP, DOWN, LEFT, RIGHT
};

#endif