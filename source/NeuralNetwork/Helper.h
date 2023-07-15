#pragma once
#include <random>


static class Helper {

public:

	static double RandomWeight() {

        std::random_device rd;  // Obtain a random seed from the hardware
        std::mt19937 gen(rd()); // Seed the random number engine

        std::uniform_real_distribution<double> dis(-1.0, 1.0); // Define the distribution

        double randomNumber = dis(gen);  // Generate a random number between -1 and 1


        return randomNumber;
	}

    //sign activation function
    static int Sign(float toCheck) {

        if (toCheck >= 0)
            return 1;
        else
            return -1;
    }

};