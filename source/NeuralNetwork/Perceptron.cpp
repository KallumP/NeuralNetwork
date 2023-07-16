#include "Perceptron.h"
#include "Helper.h"

Perceptron::Perceptron(int size) {

	weights.resize(size);

	//initialises the weights randomly
	for (int i = 0; i < weights.size(); i++) 
		weights[i] = Helper::RandomInt(-1, 1);
}

void Perceptron::Draw() {

	Point lineStart = Point(-1, approximatedF(-1));
	Point lineEnd = Point(1, approximatedF(1));
	DrawLine(lineStart.GetPixelX(), lineStart.GetPixelY(), lineEnd.GetPixelX(), lineEnd.GetPixelY(), BLACK);
}

//sign activation function
int Perceptron::sign(float toCheck) {

	if (toCheck >= 0)
		return 1;
	else
		return -1;
}

int Perceptron::guess(std::vector<float> inputs) {

	float sum = 0;

	//sums the weighted inputs
	for (int i = 0; i < inputs.size(); i++) 
		sum += inputs[i] * weights[i];
	
	int output = sign(sum);
	return output;
}

void Perceptron::train(std::vector<float> inputs, int actualValue) {

	//takes the guess
	int guessedValue = guess(inputs);

	//calculates the error
	int error = actualValue - guessedValue;

	//adjusts the weight based on the error
	for (int i = 0; i < weights.size(); i++)
		weights[i] += error * inputs[i] * learningRate;
}

//returns the current approximated y value for a given x
float Perceptron::approximatedF(float x) {

	return -(weights[2] / weights[1]) - (weights[0] / weights[1]) * x;
}