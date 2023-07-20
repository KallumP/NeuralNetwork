#pragma once
#include <vector>
#include "Matrix.h"

class NeuralNetwork {

public:
	NeuralNetwork(int _numberOfInputNodes, int _numberOfHiddenNodes, int _numberOfOutputNodes);

	Matrix feedForward(std::vector<float> vectorInput);
	void train(std::vector<float> vectorInput, std::vector<float> targets);
private:
	int numberOfInputNodes;
	int numberOfHiddenNodes;
	int numberOfOutputNodes;
	float learningRate;

	Matrix inputToHiddenWeights;
	Matrix hiddenToOutputWeights;

	Matrix biasHidden;
	Matrix biasOutput;
};

