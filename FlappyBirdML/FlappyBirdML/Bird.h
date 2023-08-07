#pragma once
#include <cmath>

#include "raylib.h"
#include "Border.h"
#include "Pipe.h"

class Bird {
public:
	Bird() {}
	Bird(Point _position) {
		position = _position;
	}

	void Draw(int screenWidth, int screenHeight) {
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

		//position.x = GetMouseX();
		//position.y = GetMouseY();
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

		// Calculate the distance between to the  closest point
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

private:
	Point position;
	int radius = 20;

	float accelaration = 40;
	float velocity = 0;
	int terminalVelocity = 600;

	int flapVelocity = 500;

	Color color = RED;
};
