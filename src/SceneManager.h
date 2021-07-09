//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_SCENEMANAGER_H
#define GAME_SCENEMANAGER_H


#include "Scene.h"
class SceneManager {
private:
    std::vector<std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Rasterizer> rasterizer;

public:
    std::shared_ptr<Scene> currentScene;

    SceneManager() = default;

    int AddScene(std::shared_ptr<Scene> s);

    void SetScene(size_t i);

    void SetRasterizer(std::shared_ptr<Rasterizer> r);

    void Delete();
};

extern SceneManager sceneManager;
#endif //GAME_SCENEMANAGER_H
