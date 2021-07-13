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

    bool SetBackgroundScene(std::string name);
    bool SetGameScene(std::string name);
    bool SetUIScene(std::string name);


    void UnsetBackgroundScene();
    void UnsetUIScene();
    void UnsetGameScene();

    std::shared_ptr<Scene> GetScene(std::string name) {return scenes[name];}

    void SetRasterizer(std::shared_ptr<Rasterizer> r);

    void Delete();
};

extern SceneManager sceneManager;
#endif //GAME_SCENEMANAGER_H
