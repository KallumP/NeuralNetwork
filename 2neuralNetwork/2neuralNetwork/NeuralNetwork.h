#pragma once
#include <vector>
#include "Matrix.h"

class NeuralNetwork {

public:
	NeuralNetwork(std::vector<int> _nodeCounts);

	Matrix feedForward(std::vector<float> vectorInput);
	Matrix train(std::vector<float> vectorInput, std::vector<float> targets);

private:

	Matrix getLayerError(int index, Matrix finalOutputErrors);

	std::vector<int> layers;
	std::vector<Matrix> weights;
	std::vector<Matrix> biases;
	std::vector<Matrix> layerOutputs;
	
	float learningRate;
};

