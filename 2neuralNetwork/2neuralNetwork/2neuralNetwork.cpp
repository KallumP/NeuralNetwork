#include <iostream>
#include <vector>
#include "NeuralNetwork.h"

int main()
{

	NeuralNetwork n = NeuralNetwork(2, 3, 3);

	std::vector<float> input = { 1, 0 };

	std::vector<float> output = n.feedForward(input);


	for (int i = 0; i < output.size(); i++) {

		std::cout << output[i];
		if (i == output.size() - 1)
			continue;
		std::cout << ", ";

	}
	std::cout << std::endl;


	std::cout << "breakline";
}
