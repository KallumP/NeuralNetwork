#include "Perceptron.h"
#include "Helper.h"

Perceptron::Perceptron() {

	//initialises the weights randomly
	for (int i = 0; i < weights.size(); i++) 
		weights[i] = Helper::RandomInt(-1, 1);
	
}

int Perceptron::guess(std::array<float, 2> inputs) {

	float sum = 0;

	//sums the weighted inputs
	for (int i = 0; i < inputs.size(); i++) 
		sum += inputs[i] * weights[i];
	
	int output = Helper::Sign(sum);
	return output;
}

void Perceptron::train(std::array<float, 2> inputs, int actualValue) {

	//takes the guess
	int guessedValue = guess(inputs);

	//calculates the error
	int error = actualValue - guessedValue;

	//adjusts the weight based on the error
	for (int i = 0; i < weights.size(); i++)
		weights[i] += error * inputs[i] * learningRate;
}