#pragma once
#include "kNeuralNetwork.h"

#include <vector>

#include "Bird.h"
#include "Pipe.h"
#include "Border.h"

class Game {

public:




	Game() {

		//game borders
		floor = Border(GetScreenHeight() - 100);
		ceiling = Border(0);

		StartGame();
	}

	void StartGame() {

		bird = Bird(Point(40, GetScreenHeight() / 2));

		pipes.clear();
		SpawnPipe();
		timeSinceLastSpawn = 0;
		score = 0;
		crashed = false;
	}


	void Draw(int screenWidth, int screenHeight) {

		//draws the game borders
		floor.Draw(screenWidth, screenHeight);
		ceiling.Draw(screenWidth, screenHeight);

		//draws the pipes
		for (int i = 0; i < pipes.size(); i++)
			pipes[i].Draw(screenWidth, screenHeight);

		//draws the bird
		bird.Draw(screenWidth, screenHeight);

		DrawScore();

		if (crashed)
			DrawGameOver(screenWidth, screenHeight);
	}

	void DrawScore() {

		//draws the score
		int textY = 20;
		std::string scoreString = "Score: " + std::to_string(score);
		DrawText(scoreString.c_str(), 20, textY, 20, BLACK);
		textY += 20;
	}

	//draws the game over screen
	void DrawGameOver(int screenWidth, int screenHeight) {

		std::string gameOverString = "GAME OVER";
		int gameOverFontSize = 50;
		int gameOverStringWidth = MeasureText(gameOverString.c_str(), gameOverFontSize);
		int gameOverHeight = 80;

		std::string restartString = "Press r to restart";
		int restartFontSize = 20;
		int restartStringWidth = MeasureText(restartString.c_str(), restartFontSize);

		int pad = 20;


		DrawRectangle(
			screenWidth / 2 - gameOverStringWidth / 2 - pad,
			screenHeight / 2 - gameOverHeight / 2 - pad,
			gameOverStringWidth + pad * 2,
			gameOverHeight + pad * 2,
			GRAY);

		DrawText(gameOverString.c_str(), screenWidth / 2 - gameOverStringWidth / 2, screenHeight / 2 - gameOverHeight / 2, gameOverFontSize, BLACK);
		DrawText(restartString.c_str(), screenWidth / 2 - restartStringWidth / 2, screenHeight / 2 - gameOverHeight / 2 + 60, restartFontSize, BLACK);


	}

	void Tick(float timeElapsed) {

		//inputs
		if (IsKeyDown(KEY_R))
			StartGame();

		if (crashed)
			return;

		//inputs
		if (IsKeyDown(KEY_SPACE))
			bird.Flap();

		//bird movement
		bird.Tick(timeElapsed);

		//pipe spawning
		AttemptToSpawnPipe(timeElapsed);

		//loops through the pipes
		for (int i = pipes.size() - 1; i >= 0; i--) {

			//moves the pipes
			pipes[i].Tick(timeElapsed);

			//increases the score if the bird passes this pipe
			if (bird.CheckPassedPipe(pipes[i]))
				score += pipes[i].TakeScore();

			//removes the pipes if they go offscreen
			if (pipes[i].OffScreen())
				pipes.erase(pipes.begin() + i);
		}


		//check for game border collisions
		bird.CheckFloorCollision(floor);
		bird.CheckCeilingCollision(ceiling);

		//check for pipe collisions
		for (int i = 0; i < pipes.size(); i++) {
			if (bird.CheckPipeCollision(pipes[i])) {
				crashed = true;
				break;
			}
		}
	}

	//attempts the spawn a pipe based on how long its been
	void AttemptToSpawnPipe(float timeElapsed) {

		timeSinceLastSpawn += timeElapsed;
		if (timeSinceLastSpawn > pipeSpawnRate) {

			SpawnPipe();
			timeSinceLastSpawn = 0;
		}

	}

	//spawns a new pipe
	void SpawnPipe() {

		pipes.push_back(Pipe(GetScreenWidth(), floor.GetY()));
	}

private:
	bool crashed;
	int score;

	Bird bird;

	std::vector<Pipe> pipes;
	float pipeSpawnRate = 1.8f;
	float timeSinceLastSpawn;

	Border floor;
	Border ceiling;
};