//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_SCENEMANAGER_CPP
#define GAME_SCENEMANAGER_CPP



#include "SceneManager.h"

int SceneManager::AddScene(std::shared_ptr<Scene> s) {
    scenes.push_back(s);
    return scenes.size() - 1;
}

void SceneManager::SetScene(size_t i) {
    currentScene = scenes[i];
    currentScene->setRasterizer(rasterizer);
    currentScene->Init();

}

void SceneManager::SetRasterizer(std::shared_ptr<Rasterizer> r) {
    rasterizer = r;
}

void SceneManager::Delete() {
    rasterizer.reset();
    for (auto s: scenes) s.reset();
}
#endif //GAME_SCENEMANAGER_CPP