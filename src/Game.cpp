#include "Engine.h"
#include <algorithm>
#include "GameFieldScene.h"
#include "MenuScene.h"
#include "SceneManager.h"
#include "GameState.h"
#include "MagicNumbers.h"


#define NANOSVG_IMPLEMENTATION	// Expands implementation
#include "nanosvg.h"


SceneManager sceneManager;

unsigned int GameState::lives = MAX_LIVES;
unsigned int GameState::score = 0;

// initialize game data in this function
void initialize()
{
    std::shared_ptr<Scene> gameField = std::shared_ptr<Scene>(new GameFieldScene());
    std::shared_ptr<Scene> gameOver = std::shared_ptr<Scene>(new GameOverScene());
    std::shared_ptr<Scene> pauseMenu = std::shared_ptr<Scene>(new PauseScene());
    std::shared_ptr<Scene> mainMenu = std::shared_ptr<Scene>(new TitleScene());
//    std::shared_ptr<Scene> levelsMenu = std::shared_ptr<Scene>(new TitleScene());
    std::shared_ptr<Rasterizer> rasterizer =std::shared_ptr<Rasterizer>(
            new Rasterizer(&buffer[0][0], SCREEN_HEIGHT, SCREEN_WIDTH));
    sceneManager.SetRasterizer(rasterizer);
    sceneManager.AddScene("GameField", gameField);
    sceneManager.AddScene("GameOver", gameOver);
    sceneManager.AddScene("Pause", pauseMenu);
    sceneManager.AddScene("MainMenu", mainMenu);
    sceneManager.SetScene("MainMenu");
    sceneManager.currentScene->Init();
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

