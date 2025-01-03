/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "GameObject.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	bool showMessageBox = false;
	bool quitGame = false;

	VitalityParams tempParams;
	GameObject tempObj(tempParams);

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		const float dt = GetFrameTime();
		tempObj.update(dt);

	 BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

			tempObj.render();

			if (GuiButton((Rectangle { 600, 370, 120, 30 }), "Start Game")) showMessageBox = true;

			if (GuiButton((Rectangle{ 600, 450, 120, 30 }), "Rage Quit")) quitGame = true;

            if (showMessageBox)
            {
               //Start Game logic
            }

			if (quitGame)
			{
				CloseWindow();
			}

        EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
