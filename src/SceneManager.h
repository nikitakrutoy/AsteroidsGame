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
    std::map<std::type_index, std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Rasterizer> rasterizer;

public:
    std::shared_ptr<Scene> currentScene;

    SceneManager() = default;

    void AddScene(std::shared_ptr<Scene>& s);

    template<typename T>
    void SetScene() {
        currentScene = scenes[typeid(T)];
        currentScene->setRasterizer(rasterizer);
        currentScene->Init();
    }

    void SetRasterizer(std::shared_ptr<Rasterizer> r);

    void Delete();
};

extern SceneManager sceneManager;
#endif //GAME_SCENEMANAGER_H
