#include "NeuralNetwork.h"


NeuralNetwork::NeuralNetwork(int _numberOfInputNodes, int _numberOfHiddenNodes, int _numberOfOutputNodes) {

	numberOfInputNodes = _numberOfInputNodes;
	numberOfHiddenNodes = _numberOfHiddenNodes;
	numberOfOutputNodes = _numberOfOutputNodes;

	learningRate = 0.1f;
	
	inputToHiddenWeights = Matrix(numberOfHiddenNodes, numberOfInputNodes);
	inputToHiddenWeights.RandomizeFloats(-1, 1);

	hiddenToOutputWeights = Matrix(numberOfOutputNodes, numberOfHiddenNodes);
	hiddenToOutputWeights.RandomizeFloats(-1, 1);

	biasHidden = Matrix(numberOfHiddenNodes, 1);
	biasHidden.RandomizeFloats(-1, 1);

	biasOutput = Matrix(numberOfOutputNodes, 1);
	biasOutput.RandomizeFloats(-1, 1);
}

Matrix NeuralNetwork::feedForward(std::vector<float> vectorInput) {

	//turn the input into a matrix
	Matrix inputs = Matrix(vectorInput);

	//compute the hidden layers output by multiplying the inputs and the weights, then applying the sigmoid function
	Matrix hidden = Matrix::Multiply(inputToHiddenWeights, inputs);
	hidden.ElementAdd(biasHidden);
	hidden.ScalarMap(Helper::Sigmoid);

	//compute the output layers output by multiplying the hidden output and the weights, then applying the sigmoid function
	Matrix outputs = Matrix::Multiply(hiddenToOutputWeights, hidden);
	outputs.ElementAdd(biasOutput);
	outputs.ScalarMap(Helper::Sigmoid);

	return outputs;
}

void NeuralNetwork::train(std::vector<float> vectorInput, std::vector<float> vectorTargets) {

	//turn the input into a matrix
	Matrix inputs = Matrix(vectorInput);

	//compute the hidden layers output by multiplying the inputs and the weights, then applying the sigmoid function
	Matrix hidden = Matrix::Multiply(inputToHiddenWeights, inputs);
	hidden.ElementAdd(biasHidden);
	hidden.ScalarMap(Helper::Sigmoid);

	//compute the output layers output by multiplying the hidden output and the weights, then applying the sigmoid function
	Matrix outputs = Matrix::Multiply(hiddenToOutputWeights, hidden);
	outputs.ElementAdd(biasOutput);
	outputs.ScalarMap(Helper::Sigmoid);


	Matrix targets = Matrix(vectorTargets);

	//calculates the errors
	Matrix outputErrors = Matrix::ElementSubtract(targets, outputs);

	//std::cout << "Error: ";
	//outputErrors.TextDraw();
	
	//calculates the gradients
	Matrix hiddenToOutputGradients = Matrix::ScalarMap(outputs, Helper::dSigmoid); // unsigmoids the output
	hiddenToOutputGradients.ElementMultiply(outputErrors); // times each gradient by the errors
	hiddenToOutputGradients.ScalarMultiply(learningRate); // times each errored gradient by the learning rate

	//calculate the changes needed to the hidden to output weights
	Matrix hidden_t = Matrix::Transpose(hidden); //transpose the hidden layer's output
	Matrix deltaHiddenToOutputWeights = Matrix::Multiply(hiddenToOutputGradients, hidden_t);

	//update hidden to output weights
	hiddenToOutputWeights.ElementAdd(deltaHiddenToOutputWeights);
	biasOutput.ElementAdd(hiddenToOutputGradients);


	//calculate the errors for the hidden layer
	Matrix hiddenToOutputWeights_t = Matrix::Transpose(hiddenToOutputWeights);
	Matrix hiddenErrors = Matrix::Multiply(hiddenToOutputWeights_t, outputErrors);

	//gets the gradients
	Matrix inputToHiddenGradients = Matrix::ScalarMap(hidden, Helper::dSigmoid);
	inputToHiddenGradients.ElementMultiply(hiddenErrors);
	inputToHiddenGradients.ScalarMultiply(learningRate);

	//calculates the changes needed
	Matrix input_t = Matrix::Transpose(inputs);
	Matrix deltaInputToHiddenWeights = Matrix::Multiply(inputToHiddenGradients, input_t);


	//apply the changes to the weights 
	inputToHiddenWeights.ElementAdd(deltaInputToHiddenWeights);
	biasHidden.ElementAdd(inputToHiddenGradients);
}
