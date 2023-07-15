#include "raylib.h"

#include "Helper.h"

struct Point {
	float x;
	float y;
	int actualValue;

	static const int visualisationSize = 10;

	Point() {

		x = Helper::RandomFloat(-1, 1);
		y = Helper::RandomFloat(-1, 1);
		actualValue = -1;

		if (x < y)
			actualValue = 1;
	}

	void Draw() {


		if (actualValue == 1)
			DrawCircle(GetPixelX(), GetPixelY(), visualisationSize, BLACK);
		else
			DrawCircleLines(GetPixelX(), GetPixelY(), visualisationSize, BLACK);
	}

	float GetPixelX() {
		return Helper::Map(x, -1.0, 1.0, 0.0, GetScreenWidth());
	}

	float GetPixelY() {
		return Helper::Map(y, -1.0, 1.0, GetScreenHeight(), 0.0);
	}
};