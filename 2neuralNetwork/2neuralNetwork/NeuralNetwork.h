#pragma once
#include <vector>
#include "Matrix.h"

class NeuralNetwork {

public:
	NeuralNetwork(int _numberOfInputNodes, int _numberOfHiddenNodes, int _numberOfOutputNodes);

	std::vector<float> feedForward(std::vector<float> vectorInput);

private:
	int numberOfInputNodes;
	int numberOfHiddenNodes;
	int numberOfOutputNodes;

	Matrix inputToHiddenWeights;
	Matrix hiddenToOutputWeights;

	Matrix biasHidden;
	Matrix biasOutput;
};

