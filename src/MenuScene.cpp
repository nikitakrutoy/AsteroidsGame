//
// Created by Nikita Krutoy on 09.07.2021.
//

#include "MenuScene.h"
#include "GameState.h"
#include "MagicNumbers.h"

void MenuScene::Draw() {
    r->fillColor();
    for (auto& m : menuItems) {
        m->SafeDraw();
    }
    for (auto& l : labels) {
        l->SafeDraw();
    }
}

void MenuScene::Update(float dt) {
    for (auto& m: menuItems) {m->SafeUpdate(dt);};
    for (auto& l: labels) {l->SafeUpdate(dt);};
    if (is_key_pressed(VK_UP) && !upWasPressed) {
        upWasPressed = true;
    }

    if (!is_key_pressed(VK_UP) && upWasPressed) {
        menuItems[selectedMenuItem]->isSelected = false;
        selectedMenuItem = (menuItems.size() + selectedMenuItem - 1) % menuItems.size();
        menuItems[selectedMenuItem]->isSelected = true;
        upWasPressed = false;
    }

    if (is_key_pressed(VK_DOWN) && !downWasPressed) {
        downWasPressed = true;
    }

    if (!is_key_pressed(VK_DOWN) && downWasPressed) {
        menuItems[selectedMenuItem]->isSelected = false;
        selectedMenuItem = (menuItems.size() + selectedMenuItem + 1) % menuItems.size();
        menuItems[selectedMenuItem]->isSelected = true;
        downWasPressed = false;
    }
}

void GameOverScene::Init() {
    position = Point(r->width / 2, r->height / 2 - 80);
    menuItems = std::vector<MenuItem*>({
           new SceneMenuItem(
                   "Retry", "GameField", true,
                   position.Translate(Point(0, 100))),
           new SceneMenuItem(
                   "Main Menu", "MainMenu", true,
                   position.Translate(Point(0, 135))),
           new ExitMenuItem(
                   "Exit", position.Translate(Point(0, 170)))
    });
    Point scoreTextPosition = position.Translate(Point(0, GAMEOVER_TEXT_SIZE + 10));
    labels = std::vector<TextObject*>({
        new TextObject("Game Over", position, GAMEOVER_TEXT_SIZE, Color(), 2, true),
        new ScoreText("", scoreTextPosition, NORMAL_TEXT_SIZE, Color(),  2, true)
    });
    MenuScene::Init();
}

void PauseScene::Init() {
    position = Point(r->width / 2, r->height / 2 - 80);
    menuItems = std::vector<MenuItem*>({
           new SceneMenuItem(
                   "Continue", "GameField", false,
                   position.Translate(Point(0, 100))),
           new SceneMenuItem(
                   "Main Menu", "MainMenu", true,
                   position.Translate(Point(0, 135))),
           new SceneMenuItem(
                   "Retry", "GameField", true,
                   position.Translate(Point(0, 170))),
           new ExitMenuItem(
                   "Exit", position.Translate(Point(0, 205)))
    });
    Point scoreTextPosition = position.Translate(Point(0, GAMEOVER_TEXT_SIZE + 10));
    labels = std::vector<TextObject*>({
          new TextObject("Pause", position, GAMEOVER_TEXT_SIZE, Color(), 2, true),
          new ScoreText("", scoreTextPosition, NORMAL_TEXT_SIZE, Color(),  2, true)
    });
    MenuScene::Init();
}

void LevelsScene::Init() {
    position = Point(100, r->height / 2 - 80);
    menuItems = std::vector<MenuItem*>({
           new SceneMenuItem(
                   "Level1", "Level1", true,
                   position.Translate(Point(0, 100)), false),
           new SceneMenuItem(
                   "Level2", "Level2", true,
                   position.Translate(Point(0, 135)), false),
           new SceneMenuItem(
                   "Level3", "Level3", true,
                   position.Translate(Point(0, 170)), false),
           new SceneMenuItem(
                   "Back", "MainMenu", true,
                   position.Translate(Point(0, 205)), false),
    });
    labels = std::vector<TextObject*>({
          new TextObject("Select Level", position, GAMEOVER_TEXT_SIZE, Color(), 2, false),
          new TextObject("", Point(500, r->height / 2 - 80),
                         GAMEOVER_TEXT_SIZE, Color(), 2, false)
    });
    descriptions = std::vector<std::string>({
        "123", "456", "345", "Hello my brother"
    });
    MenuScene::Init();
}


void TitleScene::Init() {
    position = Point(r->width / 2, TITLE_TEXT_SIZE / 2 + 10);
    Point center = Point(r->width / 2, r->height / 2 - 50);
    menuItems = std::vector<MenuItem*>({
           new SceneMenuItem(
                   "Infinite Mode", "GameField", true, center),
           new SceneMenuItem(
                   "Select Level", "Levels", true,
                   center.Translate(Point(0, NORMAL_TEXT_SIZE + 10))),
           new ExitMenuItem(
                   "Exit", center.Translate(Point(0, 2 * (NORMAL_TEXT_SIZE + 10))))
    });
    labels = std::vector<TextObject*>({
          new TextObject("Asteroids", position, TITLE_TEXT_SIZE, Color(), 10, true),
    });
    MenuScene::Init();
}
