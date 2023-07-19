#include "raylib.h"

#include <iostream>
#include <array>

#include "Helper.h"
#include "Perceptron.h"
#include "TrainingData.h"

// Initialization of static variables
float Function::M = 0.0f;
float Function::B = 0.0f;

std::vector<TrainingData> GenerateTrainingData(int desiredDataCount) {
	std::vector<TrainingData> data;
	for (int i = 0; i < desiredDataCount; i++)
		data.push_back(TrainingData());

	return data;
}

int main(void)
{

	const int screenWidth = 720;
	const int screenHeight = 720;
	InitWindow(screenWidth, screenHeight, "Perceptron");
	SetTargetFPS(60);



	//sets the function to be approximated
	Function::SetFunctionValues();

	//creates the set of training data
	std::vector<TrainingData> data = GenerateTrainingData(1000);


	Perceptron perceptron = Perceptron(3);

	while (!WindowShouldClose()) {

		BeginDrawing();
		ClearBackground(RAYWHITE);

		//draws the function line
		Point lineStart = Point(-1, Function::f(-1));
		Point lineEnd = Point(1, Function::f(1));
		DrawLine(lineStart.GetPixelX(), lineStart.GetPixelY(), lineEnd.GetPixelX(), lineEnd.GetPixelY(), BLACK);

		//draws the training data
		for (int i = 0; i < data.size(); i++) {

			//draws the datapoint
			data[i].Draw();

			//creates an input out of the training data
			std::vector<float> pointInputs = { data[i].p.x, data[i].p.y, data[i].bias };
			int target = data[i].actualValue;

			//draws if the guess was right
			int guess = perceptron.guess(pointInputs);
			if (guess == target)
				DrawCircle(data[i].p.GetPixelX(), data[i].p.GetPixelY(), Helper::visualisationSize - 3, GREEN);
			else
				DrawCircle(data[i].p.GetPixelX(), data[i].p.GetPixelY(), Helper::visualisationSize - 3, RED);
		}

		//draws the function that the perceptron has approximated so far
		perceptron.Draw();


		//trains the perceptron
		for (int i = 0; i < data.size(); i++) {

			//creates an input out of the training data
			std::vector<float> pointInputs = { data[i].p.x, data[i].p.y, data[i].bias };
			int target = data[i].actualValue;

			//trains the data
			perceptron.train(pointInputs, target);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}