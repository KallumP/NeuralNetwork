#pragma once
#include <array>

class Perceptron {

public:
	Perceptron();
	
	int guess(std::array<float, 2> inputs);
	void train(std::array<float, 2> inputs, int actualValue);

private:

	std::array<float, 2> inputs;
	std::array<float, 2> weights;
	float learningRate = 0.1f;
};

