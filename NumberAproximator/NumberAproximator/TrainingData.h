#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

struct ImageData {

	ImageData(int _label, std::vector<float> _pixels) {

		targets = { 0,0,0,0,0,0,0,0,0,0 };
		targets[_label] = 1;

		label = _label;

		inputs = _pixels;
	}

	std::vector<float> inputs;
	std::vector<float> targets;
	int label;
};

class TrainingData {

public:

	TrainingData(int toLoad) {

		//doesnt allow more than negative data
		bool unlimited = (toLoad < 1);

		//opens the file
		std::string fileName = "train.csv";
		std::ifstream file(fileName);

		if (!file.is_open()) {
			std::cerr << "Error: Unable to open the file " << fileName << std::endl;
			std::cerr << "Get the train.csv file from https://www.kaggle.com/c/digit-recognizer/data" << std::endl;
			return;
		}

		//loops through each line in the file
		std::string line;
		std::getline(file, line); //ignore first line
		while (std::getline(file, line)) {

			std::stringstream ss(line);
			std::string subString;

			std::getline(ss, subString, ',');
			int targets = std::stoi(subString);

			//gets the pixel values from the 
			std::vector<float> pixelValues;
			while (std::getline(ss, subString, ','))
				pixelValues.push_back(std::stoi(subString) / 255.0f);

			imageDatas.push_back(ImageData(targets, pixelValues));


			if (!unlimited) //checks image count if not unlimited allowed
				if (imageDatas.size() >= toLoad) 
					return;
				
			//output restraint
			if (imageDatas.size() % outputRestraint != 0)
				continue;

			std::cout << imageDatas.size() << " images added" << std::endl;
		}

		std::cout << imageDatas.size() << " images added" << std::endl;
	}

	std::vector<ImageData> imageDatas;

private:
	int outputRestraint = 1000;
	int iterations = 0;

};



