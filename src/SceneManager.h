//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_SCENEMANAGER_H
#define GAME_SCENEMANAGER_H


#include <map>
#include "Scene.h"
#include "GameFieldScene.h"
class SceneManager {
private:
    std::map<std::string, std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Rasterizer> rasterizer;

public:
    std::shared_ptr<Scene> currentBackgroundScene;
    std::shared_ptr<Scene> currentGameScene;
    std::shared_ptr<Scene> currentUIScene;

    std::string currentBackgroundSceneName;
    std::string currentGameSceneName;
    std::string currentUISceneName;

    std::string lastGameSceneName;

    SceneManager() = default;

    void AddScene(std::string name, std::shared_ptr<Scene>& s);

    void SetBackgroundScene(std::string name) {
        if (name == currentBackgroundSceneName) return;
        currentBackgroundScene = scenes[name];
        currentBackgroundSceneName = name;
        currentBackgroundScene->setRasterizer(rasterizer);
        currentBackgroundScene->Init();
    }
    void SetUIScene(std::string name) {
        if (name == currentUISceneName) return;
        currentUIScene = scenes[name];
        currentUISceneName = name;
        currentUIScene->setRasterizer(rasterizer);
        currentUIScene->Init();
    }

    void SetGameScene(std::string name) {
        if (name == currentGameSceneName) return;
        currentGameScene = scenes[name];
        currentGameScene->setRasterizer(rasterizer);
        lastGameSceneName = name;
        currentGameSceneName = name;
        currentGameScene->Init();

    }
    void UnsetBackgroundScene() {
        currentBackgroundScene = nullptr;
        currentBackgroundSceneName = "";
    }

    void UnsetUIScene() {
        currentUIScene = nullptr;
        currentUISceneName = "";
    }

    void UnsetGameScene() {
        currentGameScene = nullptr;
        currentGameSceneName = "";
    }

    std::shared_ptr<Scene> GetScene(std::string name) {
        return scenes[name];
    }

    void SetRasterizer(std::shared_ptr<Rasterizer> r);

    void Delete();
};

extern SceneManager sceneManager;
#endif //GAME_SCENEMANAGER_H
