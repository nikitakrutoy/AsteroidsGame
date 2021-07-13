//
// Created by Nikita Krutoy on 09.07.2021.
//

#ifndef GAME_MENUITEM_CPP
#define GAME_MENUITEM_CPP

#include "MenuItem.h"

MenuItem::MenuItem(std::string text, Point p, bool align):
    text(text), align(align){
    position = p;
}

void MenuItem::Draw() {
    Color color = c;
    if (isSelected) {
        float v = std::round((0.5 * std::sin(elapsedTime * 10) + 0.5));
        color = color * v;
        color.A = v;
    }
    r->drawText(text, position, NORMAL_TEXT_SIZE, 0, color, 2, align);
}

void ExitMenuItem::Update(float dt)  {
    MenuItem::Update(dt);
    if (is_key_pressed(VK_RETURN) && isSelected) {
        schedule_quit_game();
    }
}

void SceneMenuItem::Update(float dt) {
    MenuItem::Update(dt);
    if (is_key_pressed(VK_RETURN) && !enterWasPressed) {
        enterWasPressed = true;
    }

    if (!is_key_pressed(VK_RETURN) && enterWasPressed && isSelected) {
        SetScene();
        enterWasPressed = false;
    }
}

void SceneMenuItem::SetScene() {
    if (!bgSceneName.empty()) sceneManager.SetBackgroundScene(bgSceneName);
    else sceneManager.UnsetBackgroundScene();

    if (!gameSceneName.empty()) sceneManager.SetGameScene(gameSceneName);
    else sceneManager.UnsetGameScene();

    if (!uiSceneName.empty()) sceneManager.SetUIScene(uiSceneName);
    else sceneManager.UnsetUIScene();
}
void ContinueMenuItem::Update(float dt)  {
    gameSceneName = sceneManager.lastGameSceneName;
    SceneMenuItem::Update(dt);
}

void ContinueMenuItem::SetScene() {
    SceneMenuItem::SetScene();
    sceneManager.currentGameScene->doUpdate = true;
}


void RetryMenuItem::SetScene()  {
    ContinueMenuItem::SetScene();
    sceneManager.currentGameScene->Init();
}

#endif //GAME_MENUITEM_CPP