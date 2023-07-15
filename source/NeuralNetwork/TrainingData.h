#include "raylib.h"

#include "Helper.h"

struct Point {
	float x;
	float y;
	int actualValue;

	static const int visualisationSize = 10;

	Point() {

		x = Helper::RandomFloat(0, GetScreenWidth());
		y = Helper::RandomFloat(0, GetScreenHeight());
		actualValue = -1;

		if (x < y)
			actualValue = 1;
	}

	void Draw() {


		if (actualValue == 1)
			DrawCircle(x, y, visualisationSize, BLACK);
		else
			DrawCircleLines(x, y, visualisationSize, BLACK);
	}
};