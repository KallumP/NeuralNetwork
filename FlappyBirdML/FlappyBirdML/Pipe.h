#pragma once
#include <vector>
#include "kNeuralNetwork.h"

class Pipe {

public:
	Pipe() {}
	Pipe(int x, int lowerBoarder) {
		position = Point(x + width, Helper::RandomInt(gap, lowerBoarder - gap));

	}

	void Draw(int screenWidth, int screenHeight) {

		//top pipe
		DrawRectangle(position.x - width / 2, 0, width, position.y - gap / 2, GREEN);

		//bottom pipe
		DrawRectangle(position.x - width / 2, position.y + gap / 2, width, screenHeight - position.y + gap / 2, GREEN);
	}

	void Tick(float timeElapsed) {
		position.x -= speed * timeElapsed;
	}

	//returns and empties the score of this pipe
	int TakeScore() {
		int toReturn = score;
		score = 0;
		return toReturn;
	}

	//returns if the pipe is offscreen
	bool OffScreen() {
		return position.x + width < 0;
	}

	Point GetPosition() { return position; }
	int GetGap() { return gap; }
	int GetWidth() { return width; }



private:
	Point position;

	int width = 80;
	int gap = 150;
	int speed = 160;

	bool score = 1;
};