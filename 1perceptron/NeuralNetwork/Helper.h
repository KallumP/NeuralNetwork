#pragma once
#include "raylib.h"

#include <random>


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

};

struct Point {
    float x;
    float y;

    Point() {
        x = Helper::RandomFloat(-1, 1);
        y = Helper::RandomFloat(-1, 1);
    }

    Point(float _x, float _y) {
        x = _x;
        y = _y;
    }

    float GetPixelX() {
        return Helper::Map(x, -1.0, 1.0, 0.0, GetScreenWidth());
    }

    float GetPixelY() {
        return Helper::Map(y, -1.0, 1.0, GetScreenHeight(), 0.0);
    }
};


//function to be approximated
struct Function {
    static float M;
    static float B;

    //sets a random set of function parameters
    static void SetFunctionValues() {
        M = Helper::RandomFloat(-1, 1);
        B = Helper::RandomFloat(-0.5, 0.5);
    }
    
    //get the y value for the given x from this function
    static float f(float x) {
        return M * x - B;
    }
};