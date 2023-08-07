// FlappyBirdML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "raylib.h"
#include <vector>
#include <iostream>

#include "Game.h"
int main() {

    //raylib inits
    const int screenWidth = 400;
    const int screenHeight = 700;
    InitWindow(screenWidth, screenHeight, "Neural Bird");
    SetTargetFPS(60);

    Game game = Game();


    float tickTime = 0;
    while (!WindowShouldClose()) {

        float tickStart = GetTime();

        game.Tick(tickTime);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        game.Draw(screenWidth, screenHeight);

        EndDrawing();

        float tickEnd = GetTime();
        tickTime = tickEnd - tickStart;
    }

}

