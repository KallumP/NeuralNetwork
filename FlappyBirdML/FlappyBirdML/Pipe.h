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
	int TakeScore(int birdIndex) {

		if (!ScoreAlreadyTaken(birdIndex)) {

			//saves that the score has been taken for this one
			scoreTakenFor.push_back(birdIndex);

			return score;
		}

		return 0;
	}

	//returns if the score has already been taken for this bird
	bool ScoreAlreadyTaken(int birdIndex) {
		for (int i = 0; i < scoreTakenFor.size(); i++)
			if (scoreTakenFor[i] == birdIndex)
				return true;
		return false;
	}

	//returns if the pipe is offscreen
	bool OffScreen() {
		return position.x + width < 0;
	}

	Point GetPosition() { return position; }
	static int GetGap() { return gap; }
	int GetWidth() { return width; }

private:
	Point position;

	static const int gap = 200;
	int width = 80;
	int speed = 160;

	bool score = 1;

	std::vector<int> scoreTakenFor;
};