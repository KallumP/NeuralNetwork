#include "raylib.h"

#include "Helper.h"


struct TrainingData {
	
	Point p;
	float bias = 1;
	int actualValue;

	TrainingData() {

		p = Point();

		//sets the data for if the point for this data is above the line
		float expectedY = Function::f(p.x);
		if (p.y > expectedY)
			actualValue = 1;
		else
			actualValue = -1;
	}

	void Draw() {

		if (actualValue == 1)
			DrawCircleLines(p.GetPixelX(), p.GetPixelY(), Helper::visualisationSize, BLACK);
		else
			DrawCircle(p.GetPixelX(), p.GetPixelY(), Helper::visualisationSize, BLACK);
	}
};