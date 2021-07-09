//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_SCENEMANAGER_H
#define GAME_SCENEMANAGER_H


#include <typeindex>
#include <map>
#include "Scene.h"
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
    }

    void SetRasterizer(std::shared_ptr<Rasterizer> r);

    void Delete();
};

extern SceneManager sceneManager;
#endif //GAME_SCENEMANAGER_H
