#include "Engine.h"
#include <algorithm>
#include "GameFieldScene.h"
#include "MenuScene.h"
#include "SceneManager.h"
#include "GameState.h"
#include "BackgroundScene.h"
#include <filesystem>
#include <unistd.h>



SceneManager sceneManager;

// initialize game data in this function
void initialize()
{
    GameState::setSavePath("/tmp");
    GameState::Load();

    std::shared_ptr<Scene> solidBackground = std::shared_ptr<Scene>(new SolidBackgroundScene());
    std::shared_ptr<Scene> spaceBackgroundDark = std::shared_ptr<Scene>(new SpaceBackgroundScene(0.1, 0.2));
    std::shared_ptr<Scene> spaceBackgroundLight = std::shared_ptr<Scene>(new SpaceBackgroundScene(0.1, 0.3));

    // Define Infinite Mode Scene and Levels Scenes
    std::shared_ptr<Scene> gameField = std::shared_ptr<Scene>(new GameFieldScene(20, true));
    std::shared_ptr<Scene> level1 = std::shared_ptr<Scene>(new GameFieldScene(1, false, 3, 3));
    std::shared_ptr<Scene> level2 = std::shared_ptr<Scene>(new GameFieldScene(30, false, 3, 2));
    std::shared_ptr<Scene> level3 = std::shared_ptr<Scene>(new GameFieldScene(40, false, 1, 0));
    std::shared_ptr<Scene> level4 = std::shared_ptr<Scene>(new GameFieldScene(50, false, 0, 0));

    // Define menu scenes
    std::shared_ptr<Scene> gameOverMenu = std::shared_ptr<Scene>(new GameOverScene());
    std::shared_ptr<Scene> pauseMenu = std::shared_ptr<Scene>(new PauseScene());
    std::shared_ptr<Scene> mainMenu = std::shared_ptr<Scene>(new TitleScene());
    std::shared_ptr<Scene> levelsMenu = std::shared_ptr<Scene>(new LevelsScene());
    std::shared_ptr<Scene> completeMenu = std::shared_ptr<Scene>(new CompleteScene());

    // Init and set rasterizer
    std::shared_ptr<Rasterizer> rasterizer =std::shared_ptr<Rasterizer>(
            new Rasterizer(&buffer[0][0], SCREEN_HEIGHT, SCREEN_WIDTH));
    sceneManager.SetRasterizer(rasterizer);

    // Add defined scenes to scene manager
    sceneManager.AddScene("SolidBackground", solidBackground);
    sceneManager.AddScene("SpaceBackgroundDark", spaceBackgroundDark);
    sceneManager.AddScene("SpaceBackgroundLight", spaceBackgroundLight);
    sceneManager.AddScene("GameField", gameField);
    sceneManager.AddScene("GameOver", gameOverMenu);
    sceneManager.AddScene("Complete", completeMenu);
    sceneManager.AddScene("Pause", pauseMenu);
    sceneManager.AddScene("MainMenu", mainMenu);
    sceneManager.AddScene("Levels", levelsMenu);
    sceneManager.AddScene("Level1", level1);
    sceneManager.AddScene("Level2", level2);
    sceneManager.AddScene("Level3", level3);
    sceneManager.AddScene("Level4", level4);

    // Init and set main menu screne
    sceneManager.SetBackgroundScene("SpaceBackgroundLight");
    sceneManager.SetUIScene("MainMenu");
    sceneManager.currentBackgroundScene->Init();
    sceneManager.currentUIScene->Init();

}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
    if(sceneManager.currentBackgroundScene) sceneManager.currentBackgroundScene->SafeUpdate(dt);
    if(sceneManager.currentGameScene) sceneManager.currentGameScene->SafeUpdate(dt);
    if(sceneManager.currentUIScene) sceneManager.currentUIScene->SafeUpdate(dt);
}

// fill buf in this function
// uint32_t buf[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, buf)
void draw()
{
    if(sceneManager.currentBackgroundScene) sceneManager.currentBackgroundScene->SafeDraw();
    else sceneManager.GetScene("SolidBackground")->SafeDraw();
    if(sceneManager.currentGameScene) sceneManager.currentGameScene->SafeDraw();
    if(sceneManager.currentUIScene) sceneManager.currentUIScene->SafeDraw();
}

// free game data in this function
void finalize()
{
    sceneManager.Delete();
}

