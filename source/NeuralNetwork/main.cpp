#include <iostream>

#include "Helper.h"

#include "Perceptron.h"

int main(void)
{

    Perceptron p;

    std::array<float, 2> inputs = { 10, 20 };
    int guess = p.guess(inputs);
    std::cout << guess;
}