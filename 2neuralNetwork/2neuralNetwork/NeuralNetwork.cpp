#include "NeuralNetwork.h"


NeuralNetwork::NeuralNetwork(int _numberOfInputNodes, int _numberOfHiddenNodes, int _numberOfOutputNodes) {

	numberOfInputNodes = _numberOfInputNodes;
	numberOfHiddenNodes = _numberOfHiddenNodes;
	numberOfOutputNodes = _numberOfOutputNodes;

	inputToHiddenWeights = Matrix(numberOfHiddenNodes, numberOfInputNodes);
	inputToHiddenWeights.RandomizeFloats(-1, 1);

	hiddenToOutputWeights = Matrix(numberOfOutputNodes, numberOfHiddenNodes);
	hiddenToOutputWeights.RandomizeFloats(-1, 1);

	biasHidden = Matrix(numberOfHiddenNodes, 1);
	biasHidden.RandomizeFloats(-1, 1);

	biasOutput = Matrix(numberOfOutputNodes, 1);
	biasOutput.RandomizeFloats(-1, 1);
}

std::vector<float> NeuralNetwork::feedForward(std::vector<float> vectorInput) {

	Matrix inputMatrix = Matrix(vectorInput);

	Matrix hiddenOutputs = Matrix::Multiply(inputToHiddenWeights, inputMatrix);
	hiddenOutputs.ElementAdd(biasHidden);
	hiddenOutputs.ApplySigmoid();

	Matrix output = Matrix::Multiply(hiddenToOutputWeights, hiddenOutputs);
	output.ElementAdd(biasOutput);
	output.ApplySigmoid();

	return output.AsVector();
}