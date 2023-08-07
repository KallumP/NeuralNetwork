#pragma once
#include "raylib.h"

class Border {
public:
	Border() {}
	Border(int _y) {
		y = _y;
	}

	void Draw(int screenWidth, int screenHeight) {
		DrawLine(0, y, screenWidth, y, BLACK);
	}

	int GetY() { return y; }

private:
	int y;
};