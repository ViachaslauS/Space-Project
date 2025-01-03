/*
  Raylib example file.
  This is an example main file for a simple raylib project.
  Use this as a starting point or replace it with your code.

  by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "GameObject.h"

#include "resource_dir.h" // utility header for SearchAndSetResourceDir

#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"

#include "Popups/Popups.h"
#include "Popups/MainMenuPopup.h"
#include "Game.h"
#include "Helpers.h"
#include "AppContext.h"

void updateContext(float dt, AppContext& ctx)
{
    ctx.popups.update(dt);
}

void renderContext(AppContext& ctx)
{
    ctx.popups.render();
}

int main ()
{
    // Tell the window to use vsync and work on high DPI displays
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    // Create the window and OpenGL context
    InitWindow(1280, 800, "gravigun is gravifun");

    // Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
    SearchAndSetResourceDir("resources");

    // Load a texture from the resources directory
    Texture wabbit = LoadTexture("wabbit_alpha.png");

    // Add quit signal
    helpers::MulticastDelegate quitSignal;

    auto quitRequest = false;
    quitSignal.add([&quitRequest]() {
        quitRequest = true;
    });

    // Init game classes for UI
    AppContext context;
    auto mainMenuPopup = new MainMenuPopup(quitSignal);
    mainMenuPopup->show(true);
    context.popups.addPopup(mainMenuPopup);
    Game game(context);

    //Disable esc key
    SetExitKey(0);

    // game loop
    while (!WindowShouldClose())  // run the loop untill the user presses ESCAPE or presses the Close button on the window
    {
        //Update
        const auto dt = GetFrameTime();
        game.update(dt);
        updateContext(dt, context);

        //Render
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        game.render();
        renderContext(context);

        EndDrawing();

        if (quitRequest)
        {
            break;
        }
    }

    // cleanup
    // unload our texture so it can be cleaned up
    UnloadTexture(wabbit);

    // destroy the window and cleanup the OpenGL context
    CloseWindow();
    return 0;
}
