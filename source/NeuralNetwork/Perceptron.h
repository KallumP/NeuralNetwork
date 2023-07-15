#pragma once
#include <array>

class Perceptron {

public:
	Perceptron();
	
	int guess(std::array<float, 2> inputs);

private:

	std::array<float, 2> inputs;
	std::array<float, 2> weights;
};

