//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_SCENEMANAGER_CPP
#define GAME_SCENEMANAGER_CPP



#include "SceneManager.h"

void SceneManager::AddScene(std::shared_ptr<Scene>& s) {
    scenes.insert({s->getTypeIndex(), s});
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


#endif //GAME_SCENEMANAGER_CPP