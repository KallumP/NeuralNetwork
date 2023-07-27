#include <iostream>
#include "kNeuralNetwork.h"
#include "TrainingData.h"
#include "raylib.h"

int imageSize;
int pixelSize;
int screenWidth;
int screenHeight;

float outputDelay;
int padding;
int stringHeight;

int numberOfGuessesPerAccuracy;
std::vector<bool> correctGuesses;

void DebugTrainingData(TrainingData d) {

	//selects a random image
	int index = Helper::RandomInt(0, d.imageDatas.size() - 1);
	std::cout << "Number being drawn: " << d.imageDatas[index].label << std::endl;

	//gets a matrix from the image
	kMatrix m = kMatrix(d.imageDatas[index].inputs, 28, 28);

	//sets the pixel and screen size
	int pixelSize = 5;
	const int screenWidth = 28 * pixelSize;
	const int screenHeight = 28 * pixelSize;

	InitWindow(screenWidth, screenHeight, "Number approximator");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {

		BeginDrawing();
		ClearBackground(WHITE);

		//draws the pixels of the training data image
		for (int r = 0; r < m.rows; r++)
			for (int c = 0; c < m.cols; c++)
				if (m.matrix[r][c] == 0)
					DrawRectangle(c * pixelSize, r * pixelSize, pixelSize, pixelSize, WHITE);
				else
					DrawRectangle(c * pixelSize, r * pixelSize, pixelSize, pixelSize, BLACK);

		EndDrawing();
	}

	CloseWindow();
}


//visually outputs a random number prediction
void OutputPrediction(TrainingData d, NeuralNetwork n, int iterations) {

	//resets the string write height
	int stringY = 0;

	//gets a random number image to predict from
	int dataIndex = Helper::RandomInt(0, d.imageDatas.size() - 1);
	std::vector<float> toPredict = d.imageDatas[dataIndex].inputs;
	kMatrix toPredictMatrix = kMatrix(toPredict, 28, 28);
	int label = d.imageDatas[dataIndex].label;

	//gets the prediction
	std::vector<float> rawOutput = n.feedForward(toPredict).AsVector();
	int prediction = 0;
	for (int i = 1; i < rawOutput.size(); i++)
		if (rawOutput[i] > rawOutput[prediction])
			prediction = i;

	//adds the guess correctness to the list
	correctGuesses.push_back(prediction == label);
	if (correctGuesses.size() > numberOfGuessesPerAccuracy)
		correctGuesses.erase(correctGuesses.begin());

	//gets the accuracy of the guess previous x guesses
	float accuracy = 0.0f;
	for (int i = 0; i < correctGuesses.size(); i++)
		if (correctGuesses[i])
			accuracy++;
	accuracy = accuracy / correctGuesses.size();


	BeginDrawing();
	ClearBackground(RAYWHITE);

	//draws the input number
	for (int r = 0; r < toPredictMatrix.rows; r++)
		for (int c = 0; c < toPredictMatrix.cols; c++)
			if (toPredictMatrix.matrix[r][c] == 0)
				DrawRectangle(c * pixelSize, r * pixelSize, pixelSize, pixelSize, WHITE);
			else
				DrawRectangle(c * pixelSize, r * pixelSize, pixelSize, pixelSize, BLACK);

	//iteration output
	std::string iterationsString = "Iterations: " + std::to_string(iterations);
	DrawText(iterationsString.c_str(), pixelSize * imageSize + padding, stringY + padding, 20, BLACK);
	stringY += stringHeight;
	stringY += stringHeight;


	//prediction output
	std::string actualString = "Actual:      " + std::to_string(label);
	DrawText(actualString.c_str(), pixelSize * imageSize + padding, stringY + padding, 20, BLACK);
	stringY += stringHeight;

	//prediction output
	std::string predictionString = "Prediction: " + std::to_string(prediction);
	DrawText(predictionString.c_str(), pixelSize * imageSize + padding, stringY + padding, 20, BLACK);
	stringY += stringHeight;
	stringY += stringHeight;

	//prediction output
	std::string accuracyString = "Accuracy:  " + std::to_string(accuracy * 100).substr(0, 4) + "%";
	DrawText(accuracyString.c_str(), pixelSize * imageSize + padding, stringY + padding, 20, BLACK);
	stringY += stringHeight;

	EndDrawing();
}

//trains the neural network from the passed dataset
void unCappedTrain(TrainingData d) {


	NeuralNetwork n = NeuralNetwork({ imageSize * imageSize, 10, 10, 10 });

	InitWindow(screenWidth, screenHeight, "Number approximator");
	SetTargetFPS(60);

	int iterations = 0;
	float timeSinceOutput = 0;
	while (!WindowShouldClose()) {

		//time at the start of the frame
		double timeBeforeTraining = GetTime();

		//registers one iteration
		iterations++;

		//gets the random image to train on
		int dataIndex = Helper::RandomInt(0, d.imageDatas.size() - 1);

		//trains the network
		n.train(d.imageDatas[dataIndex].inputs, d.imageDatas[dataIndex].targets);

		//time taken to train
		double epochTime = GetTime() - timeBeforeTraining;

		//output constraint
		timeSinceOutput += epochTime;
		if (timeSinceOutput < outputDelay)
			continue;
		timeSinceOutput = 0;

		//output to screen
		OutputPrediction(d, n, iterations);
	}
}


int main() {

	imageSize = 28;
	pixelSize = 5;
	screenWidth = imageSize * pixelSize + 250;
	screenHeight = imageSize * pixelSize + 10;

	outputDelay = 0.5;

	padding = 20;
	stringHeight = 20;

	numberOfGuessesPerAccuracy = 30;

	int toLoad = 1000;
	TrainingData d = TrainingData(toLoad);

	if (d.imageDatas.size() > 0)
		unCappedTrain(d);

	return 0;
}
