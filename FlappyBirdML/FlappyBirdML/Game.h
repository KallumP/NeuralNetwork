#pragma once
#include "kNeuralNetwork.h"

#include <vector>

#include "Bird.h"
#include "Pipe.h"
#include "Border.h"

class Game {

public:
	Game() {

		learning = true;
		firstGame = true;

		//game borders
		floor = Border(GetScreenHeight() - 100);
		ceiling = Border(0);

		StartGame();
	}

	void StartGame() {

		if (learning) {

			//does not do any generation stuff for first game
			if (firstGame) {

				for (int i = 0; i < birdCount; i++)
					birds.push_back(KBird(Point(40, GetScreenHeight() / 2)));
				firstGame = false;

			} else {
				NextGeneration();
			}

		} else
			bird = Bird(Point(40, GetScreenHeight() / 2));

		pipes.clear();
		SpawnPipe();
		timeSinceLastSpawn = 0;
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
		DrawUI();
		if (crashed)
			DrawGameOver(screenWidth, screenHeight);
	}

	void DrawUI() {

		//draws the score
		int textY = 20;

		if (learning) {

			std::string scoreString = "Bird count: " + std::to_string(NumberOfAliveBirds());
			DrawText(scoreString.c_str(), 20, textY, 20, BLACK);
			textY += 20;

			std::string genString = "Generation: " + std::to_string(generation);
			DrawText(genString.c_str(), 20, textY, 20, BLACK);
			textY += 20;

			std::string frameString = "Frame time: " + std::to_string(previousFrameTime);
			DrawText(frameString.c_str(), 20, textY, 20, BLACK);
			textY += 20;

			std::string iterString = "Iterations per frame: " + std::to_string(ticksPerFrame);
			DrawText(iterString.c_str(), 20, textY, 20, BLACK);
			textY += 20;

		} else {

			std::string scoreString = "Score: " + std::to_string(bird.GetScore());
			DrawText(scoreString.c_str(), 20, textY, 20, BLACK);
			textY += 20;
		}
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

	void Tick(float timeElapsed, float desiredFrameTime) {

		previousFrameTime = timeElapsed;

		if (learning) {
			LearningInputs();
			for (int i = 0; i < ticksPerFrame; i++) {
				LearningBirdLogic(desiredFrameTime);
				PipeLogic(desiredFrameTime);
			}
		} else {
			ManualBirdLogic(timeElapsed);
			PipeLogic(timeElapsed);
		}
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
		if (bird.CheckFloorCollision(floor) || bird.CheckCeilingCollision(ceiling))
			crashed = true;

		//loops through the pipes
		for (int i = pipes.size() - 1; i >= 0; i--) {

			//check for pipe collisions
			if (bird.CheckPipeCollision(pipes[i])) {
				crashed = true;
				break;
			}

			//increases the score if the bird passes this pipe
			if (!crashed && bird.CheckPassedPipe(pipes[i]))
				bird.AddScore(pipes[i].TakeScore(0));
		}
	}

	void LearningBirdLogic(float timeElapsed) {

		//keeps looping while there are still alive birds left
		if (AliveBirdsRemain()) {

			//loops through each bird in the game
			for (int i = 0; i < birds.size(); i++) {

				//only does logic if the bird is alive
				if (birds[i].Alive()) {

					birds[i].Think(pipes);
					birds[i].Tick(timeElapsed);

					//check for game border collisions
					if (birds[i].CheckCeilingCollision(ceiling) || birds[i].CheckFloorCollision(floor))
						birds[i].Kill();

					//loops through each pipe
					for (int j = 0; j < pipes.size(); j++) {

						//check for pipe collisions
						if (birds[i].CheckPipeCollision(pipes[j])) {
							birds[i].Kill();
							break;
						}

						//increases the score if the bird passes this pipe
						if (birds[i].Alive() && birds[i].CheckPassedPipe(pipes[j]))
							birds[i].AddScore(pipes[j].TakeScore(i));
					}

					if (birds[i].Alive())
						birds[i].IncrementFrameCount();
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

	void LearningInputs() {
		if (IsKeyDown(KEY_LEFT))
			ticksPerFrame--;
		if (IsKeyDown(KEY_RIGHT))
			ticksPerFrame++;

		if (ticksPerFrame < 1)
			ticksPerFrame = 1;

		if (IsKeyDown(KEY_R)) {
			generation = 0;
			StartGame();
		}
	}

	//returns the number of alive birds
	int NumberOfAliveBirds() {
		int x = 0;
		for (int i = 0; i < birds.size(); i++)
			if (birds[i].Alive())
				x++;
		return x;
	}

	//returns if any birds are still alive
	bool AliveBirdsRemain() {
		return NumberOfAliveBirds() > 0;
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

	//gets the next generation of birds
	void NextGeneration() {

		//saves this generation of birds
		previousGeneration = birds;
		CalculateFitness();

		//clears the birds
		birds.clear();

		//repopulates the birds
		for (int i = 0; i < birdCount; i++) {

			NeuralNetwork newBrain = PickBrain();
			newBrain.Mutate(0.3);
			birds.push_back(KBird(Point(40, GetScreenHeight() / 2), newBrain));
		}

		generation++;
	}

	//calculates the fitness of each bird
	void CalculateFitness() {
		float sum = 0;
		for (int i = 0; i < previousGeneration.size(); i++)
			sum += previousGeneration[i].GetFrameCount();

		for (int i = 0; i < previousGeneration.size(); i++)
			previousGeneration[i].SetFitness(previousGeneration[i].GetFrameCount() / sum);
	}

	//picks a new brain based off the fitness
	NeuralNetwork PickBrain() {

		int index = 0;
		float r = Helper::RandomFloat(0, 1);
		while (r > 0 && index < previousGeneration.size()) {
			r = r - previousGeneration[index].GetFitness();
			index++;
		}
		index--;
		KBird pickedBird = previousGeneration[index];

		return pickedBird.GetBrain();
	}

private:
	bool crashed;

	Bird bird;

	std::vector<Pipe> pipes;
	float pipeSpawnRate = 1.8f;
	float timeSinceLastSpawn;

	Border floor;
	Border ceiling;

	bool learning;
	std::vector<KBird> birds;
	std::vector<KBird> previousGeneration;
	int birdCount = 40;
	bool firstGame;
	int ticksPerFrame = 1;
	float previousFrameTime;
	int generation = 1;
};