#pragma once
#include <random>


static class Helper {

public:

    static float RandomFloat(float lowerBound, float upperBound) {

        std::random_device rd;  // Obtain a random seed from the hardware
        std::mt19937 gen(rd()); // Seed the random number engine

        std::uniform_real_distribution<float> dis(lowerBound, upperBound); // Define the distribution

        float randomNumber = dis(gen);  // Generate a random number between the lower and upper bounds

        return randomNumber;
    }

    //returns a random weight between 1, -1
	static double RandomInt(int lowerBound, int upperBound) {

        std::random_device rd;  // Obtain a random seed from the hardware
        std::mt19937 gen(rd()); // Seed the random number engine

        std::uniform_int_distribution<int> dis(lowerBound, upperBound); // Define the distribution

        float randomNumber = dis(gen);  // Generate a random number between the lower and upper bounds

        return randomNumber;
	}

    //sign activation function
    static int Sign(float toCheck) {

        if (toCheck >= 0)
            return 1;
        else
            return -1;
    }

    static float Map(float value, float realLower, float realUpper, float desiredLower, float desiredUpper) {
        return desiredLower + (desiredUpper - desiredLower) * ((value - realLower) / (realUpper - realLower));
        //https://stackoverflow.com/questions/17134839/how-does-the-map-function-in-processing-work
    }

};