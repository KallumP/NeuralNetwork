#pragma once
#include <random>
#include <cmath>

class Helper {

public:

    static const int visualisationSize = 5;

    //returns a random float betwee the given ranges
    static float RandomFloat(float lowerBound, float upperBound) {

        std::random_device rd;  // Obtain a random seed from the hardware
        std::mt19937 gen(rd()); // Seed the random number engine

        std::uniform_real_distribution<float> dis(lowerBound, upperBound); // Define the distribution

        float randomNumber = dis(gen);  // Generate a random number between the lower and upper bounds

        return randomNumber;
    }

    //returns a random weight between 1, -1
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
        return (float)1.0 / ((float)1.0f + std::expf(-x));
    }
};
