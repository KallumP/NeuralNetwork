#pragma once
#include <vector>
#include <random>
#include <cmath>
#include <iostream>
#include <string>
#include <functional>

struct Point {
	float x;
	float y;

	Point() = default;
	Point(float _x, float _y) {
		x = _x;
		y = _y;
	}
};

class Helper {

public:

	//returns a random float between the given ranges
	static float RandomFloat(float lowerBound, float upperBound) {

		std::random_device rd;  // Obtain a random seed from the hardware
		std::mt19937 gen(rd()); // Seed the random number engine

		std::uniform_real_distribution<float> dis(lowerBound, upperBound); // Define the distribution

		float randomNumber = dis(gen);  // Generate a random number between the lower and upper bounds

		return randomNumber;
	}

	//returns a random weight between the given ranges
	static int RandomInt(int lowerBound, int upperBound) {

		std::random_device rd;  // Obtain a random seed from the hardware
		std::mt19937 gen(rd()); // Seed the random number engine

		std::uniform_int_distribution<int> dis(lowerBound, upperBound); // Define the distribution

		int randomNumber = dis(gen);  // Generate a random number between the lower and upper bounds

		return randomNumber;
	}

	//maps a value with a given range to a desired range
	static float Map(float value, float realLower, float realUpper, float desiredLower, float desiredUpper) {

		return desiredLower + (desiredUpper - desiredLower) * ((value - realLower) / (realUpper - realLower));
		//https://stackoverflow.com/questions/17134839/how-does-the-map-function-in-processing-work
	}

	static float Sigmoid(float x) {
		return 1.0f / (1.0f + std::exp(-x));
	}

	static float dSigmoid(float x) {
		return x * (1.0f - x);
	}
};

class kMatrix {

public:

	//default constructor
	kMatrix() {
		rows = 0;
		cols = 0;
	}

	//empty matrix constructor
	kMatrix(int _rows, int _cols) {
		rows = _rows;
		cols = _cols;

		//loops through each row to be initialised
		for (int r = 0; r < rows; r++) {

			//creates a vector of columns
			std::vector<float> rowBuffer;

			//loops through each column to be initialised
			for (int c = 0; c < cols; c++)

				//creates the column for this row
				rowBuffer.push_back(0.0f);

			//adds this row to the matrix
			matrix.push_back(rowBuffer);
		}
	}

	//makes a matrix out of the passed vector
	kMatrix(std::vector<float> input) {

		rows = input.size();
		cols = 1;

		//loops through each row to be initialised
		for (int r = 0; r < rows; r++) {

			//creates a vector of columns
			std::vector<float> rowBuffer;

			//adds the vector value for this row
			rowBuffer.push_back(input[r]);

			//adds this row to the matrix
			matrix.push_back(rowBuffer);
		}
	}

	//makes a matrix of a given size out of the input vector
	kMatrix(std::vector<float> input, int xSize, int ySize) {

		rows = ySize;
		cols = xSize;

		//checks if the input vector had enough values
		if (input.size() != rows * cols)
			return;

		for (int r = 0; r < rows; r++) {

			//creates a vector of columns
			std::vector<float> rowBuffer;

			for (int c = 0; c < cols; c++)

				//adds the vector value for this row
				rowBuffer.push_back(input[c + r * xSize]);

			//adds this row to the matrix
			matrix.push_back(rowBuffer);
		}
	}

	//copy constructor
	kMatrix(const kMatrix& toCopy) {

		rows = toCopy.rows;
		cols = toCopy.cols;

		matrix = {};

		//loops through each row to be initialised
		for (int r = 0; r < rows; r++) {

			//creates a vector of columns
			std::vector<float> rowBuffer;

			//loops through each column to be initialised
			for (int c = 0; c < cols; c++)

				//creates the column for this row
				rowBuffer.push_back(toCopy.matrix[r][c]);

			//adds this row to the matrix
			matrix.push_back(rowBuffer);
		}
	}

	//draws the matrix to the console
	void TextDraw() {
		for (int r = 0; r < rows; r++) {
			std::string rowString = "";
			for (int c = 0; c < cols; c++) {
				rowString += std::to_string(matrix[r][c]);

				if (c == cols - 1)
					continue;
				rowString += ", ";
			}

			if (r != rows - 1);
			std::cout << rowString << std::endl;
		}
	}

	//returns a flattened vector (the values from each row will be next to each other)
	std::vector<float> AsVector() {

		std::vector<float> toReturn;

		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				toReturn.push_back(matrix[r][c]);

		return toReturn;
	}

	//fills the matrix in with random floats between a bound
	void RandomizeFloats(int lowerBound, int upperBound) {
		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				matrix[r][c] = Helper::RandomFloat(lowerBound, upperBound);
	}

	//fills the matrix in with random ints between a bound
	void RandomizeInts(int lowerBound, int upperBound) {
		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				matrix[r][c] = Helper::RandomFloat(lowerBound, upperBound);
	}



	//adds the input value to all cells in the matrix
	void Add(float n) {
		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				matrix[r][c] += n;
	}

	//elementwise adds the two matrices together
	void Add(kMatrix m) {

		if (!ElementWiseCompatible(*this, m))
			return;

		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				matrix[r][c] += m.matrix[r][c];
	}

	//elementwise subtract
	static kMatrix Subtract(kMatrix m1, kMatrix m2) {

		if (!ElementWiseCompatible(m1, m2))
			return kMatrix();

		kMatrix toReturn = kMatrix(m1.rows, m1.cols);

		for (int r = 0; r < m1.rows; r++)
			for (int c = 0; c < m1.cols; c++)
				toReturn.matrix[r][c] = m1.matrix[r][c] - m2.matrix[r][c];

		return toReturn;
	}

	//multiplies the input value to all the cells in the matrix
	void Multiply(float n) {
		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				matrix[r][c] *= n;
	}

	//elementwise multiplies the two matrices together
	void Multiply(kMatrix m) {

		if (!ElementWiseCompatible(*this, m))
			return;

		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				matrix[r][c] *= m.matrix[r][c];
	}

	//returns a new matrix whcich has the result of a matrix multiplcation on the two input matrices
	static kMatrix Multiply(kMatrix m1, kMatrix m2) {

		if (!MatrixMultiplyCompatible(m1, m2))
			return kMatrix(0, 0);

		kMatrix toReturn = kMatrix(m1.rows, m2.cols);

		//loops through each cell in the resulting matrix
		for (int r = 0; r < toReturn.rows; r++)
			for (int c = 0; c < toReturn.cols; c++)

				//loops through each value to add
				for (int i = 0; i < m1.cols; i++)

					toReturn.matrix[r][c] += m1.matrix[r][i] * m2.matrix[i][c];

		return toReturn;
	}



	//returns a transposed matrix of the input matrix
	static kMatrix Transpose(kMatrix m1) {

		kMatrix m2 = kMatrix(m1.cols, m1.rows);

		for (int r = 0; r < m1.rows; r++)
			for (int c = 0; c < m1.cols; c++)
				m2.matrix[c][r] = m1.matrix[r][c];

		return m2;
	}

	//applies a function to all cells in the matrix
	void ScalarMap(const std::function<float(float x)>& func) {
		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < cols; c++) {
				float val = matrix[r][c];
				float newVal = func(val);
				matrix[r][c] = newVal;
			}
		}
	}

	//static function applies the map function to an input 
	static kMatrix ScalarMap(kMatrix m1, const std::function<float(float x)>& func) {

		kMatrix toReturn = kMatrix(m1.rows, m1.cols);

		for (int r = 0; r < toReturn.rows; r++) {
			for (int c = 0; c < toReturn.cols; c++) {
				float val = m1.matrix[r][c];
				float newVal = func(val);
				toReturn.matrix[r][c] = newVal;
			}
		}

		return toReturn;
	}

	//returns if the two matrices can be elementwise operated
	static bool ElementWiseCompatible(kMatrix m1, kMatrix m2) {
		return m1.rows == m2.rows && m1.cols == m2.cols;
	}

	//returns if two matrices can be matrix multiplied
	static bool MatrixMultiplyCompatible(kMatrix m1, kMatrix m2) {
		return m1.cols == m2.rows;
	}

	//mutates the values of this matrix
	void Mutate(float mutateRate) {
		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				if (Helper::RandomFloat(0, 1) < mutateRate)
					matrix[r][c] *= Helper::RandomFloat(0.9, 1.1);
	}

	std::vector<std::vector<float>> matrix; //rows x cols
	int rows;
	int cols;
};

class kNeuralNetwork {

public:
	kNeuralNetwork() = default;
	kNeuralNetwork(std::vector<int> _layers) {

		//saves the numbers of nodes per layer
		layers = _layers;

		//for each pair of layers
		for (int i = 1; i < layers.size(); i++) {

			//generates a matrix of weights for this layer pair
			kMatrix w = kMatrix(layers[i], layers[i - 1]);
			w.RandomizeFloats(-1, 1);
			weights.push_back(w);

			//generates the bias weights for this layer pair
			kMatrix b = kMatrix(layers[i], 1);
			b.RandomizeFloats(-1, 1);
			biases.push_back(b);
		}

		learningRate = 0.1f;
	}
	kNeuralNetwork(const kNeuralNetwork& toCopy) {

		layers = toCopy.layers;

		for (int i = 0; i < toCopy.weights.size(); i++) {

			weights.push_back(kMatrix(toCopy.weights[i]));
			biases.push_back(kMatrix(toCopy.biases[i]));
		}

		learningRate = toCopy.learningRate;
	}

	//forward propegation to process the input through the layer up to the output (feed forwarding)
	kMatrix guess(std::vector<float> vectorInput) {

		//resets the vector of outputs
		layerOutputs.clear();

		//loops through each layer
		for (int i = 0; i < layers.size(); i++) {

			if (i == 0) { //input layer (doesn't have a previous layer to process)

				//turn the input into a matrix
				kMatrix inputs = kMatrix(vectorInput);
				layerOutputs.push_back(inputs);

			} else {

				int weightsIndex = i - 1; //weights and biases has one less entry, so index is one less

				//computes this layer's output
				kMatrix layerOutput = kMatrix::Multiply(weights[weightsIndex], layerOutputs[i - 1]); //layer's weights multiplied with the previous layer's output
				layerOutput.Add(biases[weightsIndex]); //layer's output after bias
				layerOutput.ScalarMap(Helper::Sigmoid); //activation function
				layerOutputs.push_back(layerOutput); //save the output
			}
		}

		//returns the last layer's outputs (the output layer)
		return layerOutputs[layerOutputs.size() - 1];
	}

	//trains the network using the the target data (backward propegation)
	kMatrix train(std::vector<float> vectorInput, std::vector<float> vectorTargets) {

		//does the prediction
		kMatrix finalOutputs = guess(vectorInput);

		//gets the final output targets
		kMatrix targets = kMatrix(vectorTargets);

		//gets the final output errors
		kMatrix finalOutputsErrors = kMatrix::Subtract(targets, finalOutputs);

		//loops through each layer
		for (int i = layers.size() - 1; i > 0; i--) {

			int weightsIndex = i - 1; //weights and biases has one less entry, so index is one less

			kMatrix outputs = layerOutputs[i]; //gets the current layer outputs
			kMatrix errors = getLayerError(i, finalOutputsErrors); //gets the errors for this layer

			//calculates the gradients
			kMatrix gradients = kMatrix::ScalarMap(outputs, Helper::dSigmoid); //unsigmoids the output
			gradients.Multiply(errors); //times each gradient by the errors
			gradients.Multiply(learningRate); //times each errored gradient by the learning rate

			//calculate the changes needed to the hidden to output weights
			kMatrix previousOutputs = layerOutputs[i - 1]; //gets the previous layer output
			kMatrix previousOutputs_t = kMatrix::Transpose(previousOutputs); //transpose the hidden layer's output
			kMatrix deltaWeights = kMatrix::Multiply(gradients, previousOutputs_t); //gets the change in weights required to get better outputs

			//update hidden to output weights
			weights[weightsIndex].Add(deltaWeights);
			biases[weightsIndex].Add(gradients);
		}

		return finalOutputsErrors;
	}

	//mutates the weightings
	void Mutate(float mutateRate) {

		for (int i = 0; i < weights.size(); i++) {
			weights[i].Mutate(mutateRate);
			biases[i].Mutate(mutateRate);
		}
	}

private:

	//returns the error for the layer of the given index
	kMatrix getLayerError(int layerIndex, kMatrix finalOutputErrors) {

		int weightsIndex = layerIndex - 1; //weights and biases has one less entry, so index is one less

		if (layerIndex < layers.size() - 1) { //not the last layer

			//transposes the next layers's weights
			kMatrix nextWeights_t = kMatrix::Transpose(weights[weightsIndex + 1]);

			//returns the error by multiplying next layers transposed weights by the next layers errors (recursive)
			return kMatrix::Multiply(nextWeights_t, getLayerError(layerIndex + 1, finalOutputErrors));
		}

		//last layer
		return finalOutputErrors; //returns the final output error
	}

	std::vector<int> layers;
	std::vector<kMatrix> weights;
	std::vector<kMatrix> biases;
	std::vector<kMatrix> layerOutputs;

	float learningRate;
};