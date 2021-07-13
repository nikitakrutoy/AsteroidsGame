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
    Color c = Color();
    MenuItem(std::string text, Point p, bool align = true);
protected:
    void Draw() override;
    void Update(float dt) override { elapsedTime += dt; }
private:
    float elapsedTime = 0;
};

struct ExitMenuItem: MenuItem {
    ExitMenuItem(std::string text, Point p): MenuItem(text, p){};

protected:
    void Update(float dt)  override;
};



struct SceneMenuItem: MenuItem  {
public:
    SceneMenuItem(std::string text, std::string bgSceneName, std::string gameSceneName, std::string uiSceneName,
                  Point p, bool align = true): MenuItem(text, p, align),
        bgSceneName(bgSceneName), gameSceneName(gameSceneName), uiSceneName(uiSceneName){};
    std::string GetGameSceneName() {return gameSceneName;}
private:
    bool enterWasPressed = false;
protected:
    std::string bgSceneName;
    std::string gameSceneName;
    std::string uiSceneName;
    void Update(float dt) ;
protected:
    virtual void SetScene();
};


struct ContinueMenuItem: SceneMenuItem {
    ContinueMenuItem(std::string text,std::string background, Point p, bool align = true):
        SceneMenuItem(text, background, "", "",  p, align) {};
protected:
    void Update(float dt) override;
    void SetScene() override ;
};

struct RetryMenuItem: ContinueMenuItem {
    RetryMenuItem(std::string text, std::string background, Point p, bool align = true):
            ContinueMenuItem(text, background, p, align) {};
protected:
    void SetScene() override;
};

#endif //GAME_MENUITEM_H
