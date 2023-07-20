#include <iostream>
#include <vector>

#include "NeuralNetwork.h"
#include "TrainingData.h"

int main()
{

	//Matrix m = Matrix(3, 2);
	//m.matrix[0] = { 2,13 };
	//m.matrix[1] = { -9,11 };
	//m.matrix[2] = { 3,17 };

	//Matrix m2 = Matrix::Transpose(m);
	//m2.TextDraw();




	NeuralNetwork n = NeuralNetwork(2, 2, 1);

	std::vector<DataStructure> data = TrainingData::SetupXorData();

	int timesToTrain = 50000;
	for (int i = 0; i < timesToTrain; i++) {


		int j = Helper::RandomInt(0, 3);
		//int j = 1;
		n.train(data[j].inputs, data[j].targets);

		if (i != 0)
			if (i % 1000 == 0)
				std::cout << "Progress: " << i << " / " << timesToTrain << std::endl;
	}


	for (int i = 0; i < data.size(); i++)
		n.feedForward(data[i].inputs).TextDraw();

	//int toTrain = 3;
	//n.train(data[toTrain].inputs, data[toTrain].targets);

}
