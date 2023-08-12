#pragma once
#include <cmath>

#include "raylib.h"
#include "kNeuralNetwork.h"
#include "Border.h"
#include "Pipe.h"

class Bird {
public:
	Bird() = default;
	Bird(Point _position) {
		position = _position;
		score = 0;
	}

	virtual void Draw(int screenWidth, int screenHeight) {
		DrawEllipse(position.x, position.y, radius, radius, color);
	}

	void Tick(float timeElapsed) {

		//move the bird
		position.y += velocity * timeElapsed;

		//increase bird's speed
		velocity += accelaration;

		//doesnt let velocity go above terminal velocity
		if (velocity > terminalVelocity)
			velocity = terminalVelocity;
		if (velocity < -terminalVelocity)
			velocity = -terminalVelocity;
	}

	//gives the bird upward velocity
	void Flap() {
		velocity = -flapVelocity;
	}

	//checks floor collision
	bool CheckFloorCollision(Border barrier) {
		if (position.y + radius > barrier.GetY()) {
			position.y = barrier.GetY() - radius;
			return true;
		}
		return false;
	}

	//checks ceiling collision
	bool CheckCeilingCollision(Border barrier) {
		if (position.y - radius < barrier.GetY()) {
			position.y = barrier.GetY() + radius;
			velocity = 0;
			return true;
		}
		return false;
	}

	//returns if the bird was anywhere within the pipe
	bool CheckPipeCollision(Pipe pipe) {

		// Calculate the closest points on the top pipe
		float topClosestX = std::max(pipe.GetPosition().x - pipe.GetWidth() / 2.0f, std::min(position.x, pipe.GetPosition().x + pipe.GetWidth() / 2.0f));
		float topClosestY = std::max(0.0f, std::min(position.y, pipe.GetPosition().y - pipe.GetGap() / 2.0f));

		// Calculate the distance between the closest point and the circle's center
		float topDistanceX = position.x - topClosestX;
		float topDistanceY = position.y - topClosestY;
		float topDistance = std::sqrt(topDistanceX * topDistanceX + topDistanceY * topDistanceY);

		// Check if there is a collision
		bool topCollision = topDistance <= radius;


		// Calculate the closest point on the bottom pipe
		float bottomClosestX = std::max(pipe.GetPosition().x - pipe.GetWidth() / 2.0f, std::min(position.x, pipe.GetPosition().x + pipe.GetWidth() / 2.0f));
		float bottomClosestY = std::max(pipe.GetPosition().y + pipe.GetGap() / 2.0f, std::min(position.y, (float)GetScreenHeight()));

		// Calculate the distance between to the closest point
		float bottomDistanceX = position.x - bottomClosestX;
		float bottomDistanceY = position.y - bottomClosestY;
		float bottomDistance = std::sqrt(bottomDistanceX * bottomDistanceX + bottomDistanceY * bottomDistanceY);

		// Check if there is a collision
		bool bottomCollision = bottomDistance <= radius;

		return topCollision || bottomCollision;
	}

	//checks if the bird center has passed the pipe center
	bool CheckPassedPipe(Pipe pipe) {
		return position.x > pipe.GetPosition().x;
	}

	void SetColor(Color _color) { color = _color; }

	int GetScore() { return score; }
	void AddScore(int toAdd) { score += toAdd; }

protected:
	Point position;
	float velocity = 0;
	int radius = 20;
	Color color = RED;
	int terminalVelocity = 600;

private:
	float accelaration = 40;

	int flapVelocity = 500;
	int score;
};


class KBird : public Bird {

public:
	KBird() : Bird() {}
	KBird(Point _position) : Bird(_position) {

		brain = NeuralNetwork({ 4, 7, 2 });
		alive = true;
		frameCount = 0;
	}
	KBird(Point _position, NeuralNetwork _brain) : Bird(_position) {

		brain = _brain;
		alive = true;
		frameCount = 0;
	}

	void Draw(int screenWidth, int screenHeight) {

		//draws the bird
		DrawEllipseLines(position.x, position.y, radius, radius, color);

		//draws the score
		std::string scoreString = std::to_string(GetScore());
		int scoreStringSize = 20;
		int scoreStringWidth = MeasureText(scoreString.c_str(), scoreStringSize);
		DrawText(scoreString.c_str(), position.x - scoreStringWidth / 2, position.y - scoreStringSize / 2, scoreStringSize, BLACK);
	}

	Pipe GetClosestPipe(std::vector<Pipe> pipes) {

		//loops through the pipes
		for (int i = 0; i < pipes.size(); i++) {

			//if this pipe's x is smaller (passed the bird)
			if ((pipes[i].GetPosition().x + pipes[i].GetWidth() / 2) - (float)position.x < 0)
				continue;

			return pipes[i];
		}
	}

	//gets the scalar distance to p
	float GetXDistanceToPipe(Pipe p) {
		return std::abs((p.GetPosition().x + p.GetWidth() / 2) - (float)position.x);
	}
	float GetYDistanceToPipe(Pipe p) {
		return p.GetPosition().y - (float)position.y;
	}

	//decides whether to flap
	void Think(std::vector<Pipe> pipes) {

		Pipe closestPipe = GetClosestPipe(pipes);

		//sets up the inputs for the thought
		std::vector<float> inputs = {};
		inputs.push_back(position.y / GetScreenHeight()); //bird y
		inputs.push_back(GetXDistanceToPipe(closestPipe) / GetScreenWidth()); //pipe x distance
		inputs.push_back((GetYDistanceToPipe(closestPipe) / GetScreenHeight() * 2) + 0.5); //pipe y distance
		inputs.push_back((velocity / terminalVelocity * 2) + 0.5); //bird velocity

		//gets what the brains thought
		kMatrix thought = brain.feedForward(inputs);

		if (thought.AsVector()[0] > thought.AsVector()[1])
			Flap();
	}

	bool Alive() { return alive; }
	void Kill() { alive = false; }
	NeuralNetwork GetBrain() { return brain; }

	int GetFrameCount() { return frameCount; }
	void IncrementFrameCount() { frameCount++; }

	void SetFitness(float _fitness) { fitness = _fitness; }
	float GetFitness() { return fitness; }

private:
	NeuralNetwork brain;
	bool alive;
	int frameCount;
	float fitness;
};