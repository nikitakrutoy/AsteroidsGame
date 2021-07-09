#include "Engine.h"
#include <algorithm>
#include "GameFieldScene.h"
#include "GameOverScene.h"
#include "SceneManager.h"
#include "GameState.h"



#define NANOSVG_IMPLEMENTATION	// Expands implementation
#include "nanosvg.h"


SceneManager sceneManager;

unsigned int GameState::lives = 3;
unsigned int GameState::score = 0;

// initialize game data in this function
void initialize()
{
    std::shared_ptr<Scene> gameField = std::shared_ptr<Scene>(new GameFieldScene());
    std::shared_ptr<Scene> gameOver = std::shared_ptr<Scene>(new GameOverScene());
    std::shared_ptr<Rasterizer> rasterizer =std::shared_ptr<Rasterizer>(
            new Rasterizer(&buffer[0][0], SCREEN_HEIGHT, SCREEN_WIDTH));
    sceneManager.SetRasterizer(rasterizer);
//    sceneManager.AddScene(gameField);
    sceneManager.AddScene(gameOver);
    sceneManager.SetScene(0);
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)

void act(float dt)
{
    sceneManager.currentScene->SafeUpdate(dt);
}

// fill buf in this function
// uint32_t buf[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, buf)
void draw()
{
    sceneManager.currentScene->SafeDraw();
}

// free game data in this function
void finalize()
{
    sceneManager.Delete();
}

