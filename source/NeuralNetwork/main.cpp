#include "raylib.h"

#include <iostream>
#include <array>

#include "Helper.h"
#include "Perceptron.h"
#include "TrainingData.h"

int main(void)
{

	const int screenWidth = 450;
	const int screenHeight = 450;
	InitWindow(screenWidth, screenHeight, "Neural Net");
	SetTargetFPS(60);





	//creates the set of training data
	std::array<Point, 100> points;
	for (int i = 0; i < points.size(); i++)
		points[i] = Point();



	Perceptron perceptron;
	//std::array<float, 2> inputs = { 10, 20 };
	//int guess = perceptron.guess(inputs);
	//std::cout << guess;

	while (!WindowShouldClose()) {

		BeginDrawing();

		ClearBackground(RAYWHITE);

		//draws the line
		DrawLine(0, 0, screenWidth, screenHeight, BLACK);

		//draws the training data
		for (int i = 0; i < points.size(); i++) {

			points[i].Draw();

			//creates an input out of the training data
			std::array<float, 2> pointInputs = { points[i].x, points[i].y };
			int target = points[i].actualValue;

			//draws if the guess was right
			int guess = perceptron.guess(pointInputs);
			if (guess == target)
				DrawCircle(points[i].x, points[i].y, Point::visualisationSize - 3, GREEN);
			else
				DrawCircle(points[i].x, points[i].y, Point::visualisationSize - 3, RED);
		}

		//if the left mouse is clicked
		if (IsMouseButtonPressed(0)) {

			//trains the perceptron
			for (int i = 0; i < points.size(); i++) {

				//creates an input out of the training data
				std::array<float, 2> pointInputs = { points[i].x, points[i].y };
				int target = points[i].actualValue;

				//trains the data
				perceptron.train(pointInputs, target);
			}
		}


		EndDrawing();
	}

	CloseWindow();
	return 0;
}