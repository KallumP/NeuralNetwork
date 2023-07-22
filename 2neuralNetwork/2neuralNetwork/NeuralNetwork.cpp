#include "NeuralNetwork.h"


NeuralNetwork::NeuralNetwork(std::vector<int> _layers) {

	//saves the numbers of nodes per layer
	layers = _layers;

	//for each pair of layers
	for (int i = 1; i < layers.size(); i++) {

		//generates a matrix of weights for this layer pair
		Matrix w = Matrix(layers[i], layers[i - 1]);
		w.RandomizeFloats(-1, 1);
		weights.push_back(w);

		//generates the bias weights for this layer pair
		Matrix b = Matrix(layers[i], 1);
		b.RandomizeFloats(-1, 1);
		biases.push_back(b);
	}

	learningRate = 0.1f;
}

//forward propegation to process the input through the layer up to the output
Matrix NeuralNetwork::feedForward(std::vector<float> vectorInput) {

	//resets the vector of outputs
	layerOutputs.clear();

	//loops through each layer
	for (int i = 0; i < layers.size(); i++) {

		if (i == 0) { //input layer (doesn't have a previous layer to process)

			//turn the input into a matrix
			Matrix inputs = Matrix(vectorInput);
			layerOutputs.push_back(inputs);

		} else {

			int weightsIndex = i - 1; //weights and biases has one less entry, so index is one less

			//computes this layer's output
			Matrix layerOutput = Matrix::Multiply(weights[weightsIndex], layerOutputs[i - 1]); //layer's weights multiplied with the previous layer's output
			layerOutput.ElementAdd(biases[weightsIndex]); //layer's output after bias
			layerOutput.ScalarMap(Helper::Sigmoid); //activation function
			layerOutputs.push_back(layerOutput); //save the output
		}
	}

	//returns the last layer's outputs (the output layer)
	return layerOutputs[layerOutputs.size() - 1];
}

//trains the network using the the target data
void NeuralNetwork::train(std::vector<float> vectorInput, std::vector<float> vectorTargets) {

	//does the prediction
	Matrix finalOutputs = feedForward(vectorInput);

	//gets the final output targets
	Matrix targets = Matrix(vectorTargets);

	//gets the final output errors
	Matrix finalOutputsErrors = Matrix::ElementSubtract(targets, finalOutputs);

	//loops through each layer
	for (int i = layers.size() - 1; i > 0; i--) {

		int weightsIndex = i - 1; //weights and biases has one less entry, so index is one less

		Matrix outputs = layerOutputs[i]; //gets the current layer outputs
		Matrix errors = getLayerError(i, finalOutputsErrors);

		//calculates the gradients
		Matrix gradients = Matrix::ScalarMap(outputs, Helper::dSigmoid); //unsigmoids the output
		gradients.ElementMultiply(errors); //times each gradient by the errors
		gradients.ScalarMultiply(learningRate); //times each errored gradient by the learning rate

		//calculate the changes needed to the hidden to output weights
		Matrix previousOutputs = layerOutputs[i - 1]; //gets the previous layer output
		Matrix previousOutputs_t = Matrix::Transpose(previousOutputs); //transpose the hidden layer's output
		Matrix deltaWeights = Matrix::Multiply(gradients, previousOutputs_t);

		//update hidden to output weights
		weights[weightsIndex].ElementAdd(deltaWeights);
		biases[weightsIndex].ElementAdd(gradients);
	}
}

//returns the error for the layer of the given index
Matrix NeuralNetwork::getLayerError(int layerIndex, Matrix finalOutputErrors) {

	int weightsIndex = layerIndex - 1; //weights and biases has one less entry, so index is one less

	if (layerIndex < layers.size() - 1) { //not the last layer

		//transposes the next layers's weights
		Matrix nextWeights_t = Matrix::Transpose(weights[weightsIndex + 1]);

		//returns the error by multiplying next layers transposed weights by the next layers errors (recursive)
		return Matrix::Multiply(nextWeights_t, getLayerError(layerIndex + 1, finalOutputErrors));
	}

	//last layer
	return finalOutputErrors; //returns the final output error
}