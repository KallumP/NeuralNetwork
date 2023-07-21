#include <iostream>
#include <vector>

#include "NeuralNetwork.h"
#include "TrainingData.h"

int main()
{
	std::vector<int> layerCounts = { 2, 3, 2, 1 };
	NeuralNetwork n = NeuralNetwork(layerCounts);

	std::vector<DataStructure> data = TrainingData::XorData();

	int timesToTrain = 50000;
	for (int i = 0; i < timesToTrain; i++) {

		int j = Helper::RandomInt(0, 3);
		n.train(data[j].inputs, data[j].targets);

		if (i == 0)
			continue;

		if (i % 2000 != 0)
			continue;
		
		std::cout << "Progress: " << 100 * i / timesToTrain << " %" << std::endl;
	}

	for (int i = 0; i < data.size(); i++) {
		std::cout << "Target: " << data[i].targets[0] << " || Output: ";
		n.feedForward(data[i].inputs).TextDraw();
	}
}
