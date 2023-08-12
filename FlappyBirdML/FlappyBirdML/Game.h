#pragma once
#include "kNeuralNetwork.h"

#include <vector>

#include "Bird.h"
#include "Pipe.h"
#include "Border.h"

class Game {

public:
	Game() {

		learning = false;

		//game borders
		floor = Border(GetScreenHeight() - 100);
		ceiling = Border(0);

		StartGame();
	}

	void StartGame() {

		if (learning) {
			birds.clear();
			for (int i = 0; i < birdCount; i++)
				birds.push_back(KBird(Point(40, GetScreenHeight() / 2)));

		} else 
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
		if (learning) {
			for (int i = 0; i < birds.size(); i++)
				if (birds[i].Alive())
					birds[i].Draw(screenWidth, screenHeight);

		} else
			bird.Draw(screenWidth, screenHeight);

		//draws the ui
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

		if (learning)
			LearningBirdLogic(timeElapsed);
		else
			ManualBirdLogic(timeElapsed);

		PipeLogic(timeElapsed);
	}

	void ManualBirdLogic(float timeElapsed) {

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

		//check for game border collisions
		bird.CheckFloorCollision(floor);
		bird.CheckCeilingCollision(ceiling);

		//loops through the pipes
		for (int i = pipes.size() - 1; i >= 0; i--) {

			//check for pipe collisions
			if (bird.CheckPipeCollision(pipes[i])) {
				crashed = true;
				break;
			}

			//increases the score if the bird passes this pipe
			if (bird.CheckPassedPipe(pipes[i]))
				score += pipes[i].TakeScore();
		}
	}

	void LearningBirdLogic(float timeElapsed) {

		//keeps looping while there are still alive birds left
		if (AliveBirdsRemain()) {

			//loops through each bird in the game
			for (int i = 0; i < birds.size(); i++) {

				//only does logic if the bird is alive
				if (birds[i].Alive()) {

					birds[i].CleverFlap(pipes);
					birds[i].Tick(timeElapsed);

					//check for game border collisions
					birds[i].CheckFloorCollision(floor);
					birds[i].CheckCeilingCollision(ceiling);

					//loops through each pipe
					for (int j = 0; j < pipes.size(); j++) {

						//check for pipe collisions
						if (birds[i].CheckPipeCollision(pipes[j])) {
							birds[i].Kill();
							break;
						}

						//increases the score if the bird passes this pipe
						if (birds[i].CheckPassedPipe(pipes[j]))
							score += pipes[j].TakeScore();
					}
				}
			}
		} else {
			StartGame();
		}
	}

	void PipeLogic(float timeElapsed) {

		if (!crashed) {

			//pipe spawning
			AttemptToSpawnPipe(timeElapsed);

			//loops through the pipes
			for (int i = pipes.size() - 1; i >= 0; i--) {

				//moves the pipes
				pipes[i].Tick(timeElapsed);

				//removes the pipes if they go offscreen
				if (pipes[i].OffScreen())
					pipes.erase(pipes.begin() + i);
			}
		}
	}

	//returns if any birds are still alive
	bool AliveBirdsRemain() {
		for (int i = 0; i < birds.size(); i++)
			if (birds[i].Alive())
				return true;
		return false;
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
	std::vector<KBird> birds;

	std::vector<Pipe> pipes;
	float pipeSpawnRate = 1.8f;
	float timeSinceLastSpawn;

	Border floor;
	Border ceiling;

	bool learning;
	int birdCount = 200;
};