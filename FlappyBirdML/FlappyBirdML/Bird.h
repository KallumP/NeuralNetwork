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
	}

	//gives the bird upward velocity
	void Flap() {
		velocity = -flapVelocity;
	}

	//checks floor collision
	void CheckFloorCollision(Border barrier) {
		if (position.y + radius > barrier.GetY())
			position.y = barrier.GetY() - radius;
	}

	//checks ceiling collision
	void CheckCeilingCollision(Border barrier) {
		if (position.y - radius < barrier.GetY()) {
			position.y = barrier.GetY() + radius;
			velocity = 0;
		}
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

protected:
	Point position;
	float velocity = 0;
	int radius = 20;
	Color color = RED;

private:
	float accelaration = 40;
	int terminalVelocity = 600;

	int flapVelocity = 500;
};


class KBird : public Bird {

public:
	KBird() : Bird() {}
	KBird(Point _position) : Bird(_position) {

		brain = NeuralNetwork({ 4, 5, 1 });
		alive = true;
	}

	virtual void Draw(int screenWidth, int screenHeight) {
		DrawEllipseLines(position.x, position.y, radius, radius, color);

	}
	int GetDistanceToNextPipe(std::vector<Pipe> pipes) {

		//loops through the pipes
		for (int i = 0; i < pipes.size(); i++) {

			//ignore pipes that have gone past the bird already
			if (pipes[i].GetPosition().x < position.x)
				continue;

			//gets the positive value of the distance to this pipe
			return std::abs(pipes[i].GetPosition().x - position.x);
		}
	}

	void CleverFlap(std::vector<Pipe> pipes) {

		//sets up the inputs for the thought
		std::vector<float> inputs = {};
		inputs.push_back(position.y);
		inputs.push_back(velocity);
		inputs.push_back(GetDistanceToNextPipe(pipes));
		inputs.push_back(Pipe::GetGap());

		//gets what the brains thought
		kMatrix thought = brain.feedForward(inputs);

		//flaps if input is close to 1
		if (thought.AsVector().size() > 0)
			if (thought.AsVector()[0] > 0.5)
				Flap();
	}

	bool Alive() { return alive; }
	void Kill() { alive = false; }

private:
	NeuralNetwork brain;
	bool alive;
};