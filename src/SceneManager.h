//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_SCENEMANAGER_H
#define GAME_SCENEMANAGER_H


#include <typeindex>
#include <map>
#include "Scene.h"
#include "GameFieldScene.h"
class SceneManager {
private:
    std::map<std::string, std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Rasterizer> rasterizer;

public:
    std::shared_ptr<Scene> currentScene;

    SceneManager() = default;

    void AddScene(std::string name, std::shared_ptr<Scene>& s);

    void SetScene(std::string name) {
        currentScene = scenes[name];
        currentScene->setRasterizer(rasterizer);
        if (dynamic_cast<GameFieldScene*>(currentScene.get())) {
            GameState::currentGameFieldSceneName = name;
        }
    }

    std::shared_ptr<Scene> GetScene(std::string name) {
        return scenes[name];
    }

    void SetRasterizer(std::shared_ptr<Rasterizer> r);

    void Delete();
};

extern SceneManager sceneManager;
#endif //GAME_SCENEMANAGER_H
