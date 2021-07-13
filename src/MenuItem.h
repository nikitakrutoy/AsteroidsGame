//
// Created by Nikita Krutoy on 09.07.2021.
//

#ifndef GAME_MENUITEM_H
#define GAME_MENUITEM_H

#include <cmath>
#include "GameObject.h"
#include "Engine.h"
#include "SceneManager.h"
#include "MagicNumbers.h"

struct MenuItem: GameObject {
    std::string text;
    bool isSelected = false;
    bool align = true;
    MenuItem(std::string text, Point p, bool align = true);
protected:
    void Draw() override{
        Color c = Color();
        if (isSelected) {
            float v = std::round((0.5 * std::sin(elapsedTime * 10) + 0.5));
            c = c * v;
            c.A = v;
        }
        r->drawText(text, position, NORMAL_TEXT_SIZE, 0, c, 2, align);
    }
    void Update(float dt) override{
        elapsedTime += dt;
    }
private:
    float elapsedTime = 0;
};

struct ExitMenuItem: MenuItem {
    ExitMenuItem(std::string text, Point p): MenuItem(text, p){};

protected:
    void Update(float dt) override {
        MenuItem::Update(dt);
        if (is_key_pressed(VK_RETURN) && isSelected) {
            schedule_quit_game();
        }
    }
};



struct SceneMenuItem: MenuItem  {
public:
    SceneMenuItem(std::string text, std::string bgSceneName, std::string gameSceneName, std::string uiSceneName,
                  Point p, bool align = true): MenuItem(text, p, align),
        bgSceneName(bgSceneName), gameSceneName(gameSceneName), uiSceneName(uiSceneName){};
    std::string GetGameSceneName() {return gameSceneName;}
private:
    bool enterWasPressed = false;
    bool doReset = false;
protected:
    std::string bgSceneName;
    std::string gameSceneName;
    std::string uiSceneName;
    void Update(float dt) override{
        MenuItem::Update(dt);
        if (is_key_pressed(VK_RETURN) && !enterWasPressed) {
            enterWasPressed = true;
        }

        if (!is_key_pressed(VK_RETURN) && enterWasPressed && isSelected) {
            SetScene();
            enterWasPressed = false;
        }
    }
protected:
    virtual void SetScene() {
        if (!bgSceneName.empty()) sceneManager.SetBackgroundScene(bgSceneName);
        else sceneManager.UnsetBackgroundScene();

        if (!gameSceneName.empty()) sceneManager.SetGameScene(gameSceneName);
        else sceneManager.UnsetGameScene();

        if (!uiSceneName.empty()) sceneManager.SetUIScene(uiSceneName);
        else sceneManager.UnsetUIScene();
    }
};


struct ContinueMenuItem: SceneMenuItem {
    ContinueMenuItem(std::string text,std::string background, Point p, bool align = true):
        SceneMenuItem(text, background, "", "",  p, align) {};
protected:
    void Update(float dt) override {
        gameSceneName = sceneManager.lastGameSceneName;
        SceneMenuItem::Update(dt);
    }

    void SetScene() override {
        SceneMenuItem::SetScene();
        sceneManager.currentGameScene->doUpdate = true;
    }
};

struct RetryMenuItem: ContinueMenuItem {
    RetryMenuItem(std::string text, std::string background, Point p, bool align = true):
            ContinueMenuItem(text, background, p, align) {};
protected:

    void SetScene() override {
        ContinueMenuItem::SetScene();
        sceneManager.currentGameScene->Init();
    }
};

#endif //GAME_MENUITEM_H
