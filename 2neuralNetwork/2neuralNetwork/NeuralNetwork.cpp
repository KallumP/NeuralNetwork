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
			layerOutputs.push_back(layerOutput);
		}
	}

	//returns the last layer's outputs (the output layer)
	return layerOutputs[layerOutputs.size() - 1];
}

void NeuralNetwork::train(std::vector<float> vectorInput, std::vector<float> vectorTargets) {


	//does the prediction
	Matrix finalOutput = feedForward(vectorInput);

	//gets the final output targets
	Matrix targets = Matrix(vectorTargets);

	//gets the final output errors
	Matrix finalOutputErrors = Matrix::ElementSubtract(targets, finalOutput);

	Matrix nextLayerError; //container to hold the next layers error (so that it doesnt have to be recursivley computed each time)

	//loops through each layer
	for (int i = layers.size() - 1; i > 0; i--) {

		int weightsIndex = i - 1;

		Matrix layerOutput = layerOutputs[i]; //gets the current layer outputs
		Matrix layerErrors = finalOutputErrors; //sets the current error to the final error

		if (i < layers.size() - 1) { //not the last layer

			//calculate the errors for the layer based on the next layer
			Matrix nextLayersWeights_t = Matrix::Transpose(weights[weightsIndex + 1]);
			layerErrors = Matrix::Multiply(nextLayersWeights_t, nextLayerError);
		}
		
		//stores this layers error to be used in the next iteration
		nextLayerError = layerErrors;

		//calculates the gradients
		Matrix layerGradients = Matrix::ScalarMap(layerOutput, Helper::dSigmoid); //unsigmoids the output
		layerGradients.ElementMultiply(layerErrors); //times each gradient by the errors
		layerGradients.ScalarMultiply(learningRate); //times each errored gradient by the learning rate

		//calculate the changes needed to the hidden to output weights
		Matrix previousLayerOutput = layerOutputs[i - 1]; //gets the previous layer output
		Matrix previousLayerOutput_t = Matrix::Transpose(previousLayerOutput); //transpose the hidden layer's output
		Matrix deltaWeights = Matrix::Multiply(layerGradients, previousLayerOutput_t);

		//update hidden to output weights
		weights[weightsIndex].ElementAdd(deltaWeights);
		biases[weightsIndex].ElementAdd(layerGradients);
	}
}
