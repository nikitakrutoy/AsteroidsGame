//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_SCENEMANAGER_CPP
#define GAME_SCENEMANAGER_CPP



#include "SceneManager.h"

void SceneManager::AddScene(std::string name, std::shared_ptr<Scene>& s) {
    scenes.insert({name, s});
}

void SceneManager::Delete() {
    rasterizer.reset();
    for (auto& kv: scenes) {
        kv.second->Delete();
        kv.second.reset();
    }
}

void SceneManager::SetRasterizer(std::shared_ptr<Rasterizer> r) {
    rasterizer = r;
}

bool SceneManager::SetBackgroundScene(std::string name) {
    if (scenes.find(name) == scenes.end()) {
        std::cout << "Could not find UI scene – " << name << std::endl;
        return false;
    }
    if (name == currentBackgroundSceneName) return true;
    currentBackgroundScene = scenes[name];
    currentBackgroundSceneName = name;
    currentBackgroundScene->SetRasterizer(rasterizer);
    currentBackgroundScene->Init();
    return true;
}

bool SceneManager::SetUIScene(std::string name) {
    if (scenes.find(name) == scenes.end()) {
        std::cout << "Could not find UI scene – " << name << std::endl;
        return false;
    }
    if (name == currentUISceneName) return true;
    currentUIScene = scenes[name];
    currentUISceneName = name;
    currentUIScene->SetRasterizer(rasterizer);
    currentUIScene->Init();
    return true;
}

bool SceneManager::SetGameScene(std::string name) {
    if (scenes.find(name) == scenes.end()) {
        std::cout << "Could not find UI scene – " << name << std::endl;
        return false;
    }
    if (name == currentGameSceneName) return true;
    currentGameScene = scenes[name];
    currentGameScene->SetRasterizer(rasterizer);
    lastGameSceneName = name;
    currentGameSceneName = name;
    currentGameScene->Init();
    return true;
}

void SceneManager::UnsetBackgroundScene() {
    currentBackgroundScene = nullptr;
    currentBackgroundSceneName = "";
}


void SceneManager::UnsetUIScene() {
    currentUIScene = nullptr;
    currentUISceneName = "";
}

void SceneManager::UnsetGameScene() {
    currentGameScene = nullptr;
    currentGameSceneName = "";
}

#endif //GAME_SCENEMANAGER_CPP