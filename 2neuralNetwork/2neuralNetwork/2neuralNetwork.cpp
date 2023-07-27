#include <iostream>
#include <vector>

#include "NeuralNetwork.h"
#include "TrainingData.h"

int outputRestraint = 1000;

void uncappedIterations(NeuralNetwork n, std::vector<DataStructure> data) {

	int i = 0;
	while (true) {

		int j = Helper::RandomInt(0, 3);
		n.train(data[j].inputs, data[j].targets);

		//output restraint
		i++;
		if (i % outputRestraint != 0)
			continue;

		std::cout << "Progress after " << i << " iterations: " << std::endl;
		//show the final numbers
		for (int i = 0; i < data.size(); i++) {
			std::cout << "Input: (" << data[i].inputs[0] << ", " << data[i].inputs[1] << ") || Target: " << data[i].targets[0] << " || Output: ";
			n.feedForward(data[i].inputs).TextDraw();
		}

		std::cout << std::endl;
	}
}

void fixedIterations(NeuralNetwork n, std::vector<DataStructure> data, int timesToTrain) {

	for (int i = 0; i < timesToTrain; i++) {

		int j = Helper::RandomInt(0, 3);
		n.train(data[j].inputs, data[j].targets);

		//output restraint
		if (i % outputRestraint != 0)
			continue;

		//output
		std::cout << "Progress: " << 100 * i / timesToTrain << " %" << std::endl;
	}

	//show the final numbers
	for (int i = 0; i < data.size(); i++) {
		std::cout << "Input: (" << data[i].inputs[0] << ", " << data[i].inputs[1] << ") || Target: " << data[i].targets[0] << " || Output: ";
		n.feedForward(data[i].inputs).TextDraw();
	}
}

int main()
{
	//setup
	std::vector<int> layerCounts = { 2, 3, 2, 1 };
	NeuralNetwork n = NeuralNetwork(layerCounts);
	std::vector<DataStructure> data = TrainingData::XorData();

	//fixedIterations(n, data, 7500);
	uncappedIterations(n, data);
}
