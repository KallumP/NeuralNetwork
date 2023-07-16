#pragma once
#include <vector>

class Perceptron {

public:
	Perceptron(int size);
	
	void Draw();

	int guess(std::vector<float> inputs);
	void train(std::vector<float> inputs, int actualValue);

private:

	int sign(float toCheck);
	float approximatedF(float x);

	std::vector<float> inputs;
	std::vector<float> weights;
	float learningRate = 0.1f;
};

