//
// Created by Nikita Krutoy on 09.07.2021.
//

#include "MenuScene.h"
#include "GameState.h"
#include "MagicNumbers.h"
#include "Resources.h"

void MenuScene::Init()  {
    for (auto& m : menuItems) m->SetRasterizer(r);
    for (auto& l : labels) l->SetRasterizer(r);
    menuItems[selectedMenuItem]->isSelected = true;
};
void MenuScene::Delete() {
    for (auto& m : menuItems) {
        m->Delete();
        delete m;
    }
    for (auto& l : labels) {
        l->Delete();
        delete l;
    }
}

void MenuScene::Draw() {
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

void EndGameScene::Init() {
    position = Point(r->width / 2, r->height / 2 - 80);

    menuItems = std::vector<MenuItem*>({
           new RetryMenuItem(
                   "Retry", "SpaceBackgroundDark", position.Translate(Point(0, 100))),
           new SceneMenuItem(
                   "Main Menu", "SpaceBackgroundLight", "", "MainMenu",
                   position.Translate(Point(0, 135))),
           new ExitMenuItem(
                   "Exit", position.Translate(Point(0, 170)))
    });
    Point scoreTextPosition = position.Translate(Point(0, GAMEOVER_TEXT_SIZE + 10));
    labels = std::vector<TextObject*>({
        new TextObject(title, position, GAMEOVER_TEXT_SIZE, Color(), 2, true),
        new ScoreText("", scoreTextPosition, NORMAL_TEXT_SIZE, Color(),  2, true)
    });
    MenuScene::Init();
}

void PauseScene::Init() {
    position = Point(r->width / 2, r->height / 2 - 80);
    menuItems = std::vector<MenuItem*>({
           new ContinueMenuItem(
                   "Continue", "SpaceBackgroundDark", position.Translate(Point(0, 100))),
           new RetryMenuItem(
                   "Retry", "SpaceBackgroundDark", position.Translate(Point(0, 135))),
           new SceneMenuItem(
                   "Main Menu", "SpaceBackgroundLight", "", "MainMenu",
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
                   "Level1", "SpaceBackgroundDark", "Level1", "",
                   position.Translate(Point(0, 100)), false),
           new SceneMenuItem(
                   "Level2", "SpaceBackgroundDark", "Level2", "",
                   position.Translate(Point(0, 135)), false),
           new SceneMenuItem(
                   "Level3", "SpaceBackgroundDark", "Level3", "",
                   position.Translate(Point(0, 170)), false),
           new SceneMenuItem(
                   "Level4", "SpaceBackgroundDark", "Level4", "",
                   position.Translate(Point(0, 205)), false),
           new SceneMenuItem(
                   "Back", "SpaceBackgroundLight", "", "MainMenu",
                   position.Translate(Point(0, 240)), false),
    });
    labels = std::vector<TextObject*>({
          new TextObject("Select Level", position, GAMEOVER_TEXT_SIZE, Color(), 2, false),
          new TextObject("", Point(500, r->height / 2 - 80),
                         GAMEOVER_TEXT_SIZE, Color(), 2, false),
          new TextObject("", Point(500, r->height / 2 + 20),
                         NORMAL_TEXT_SIZE, Color(), 2, false),
          new TextObject("", Point(500, r->height / 2 + 55),
                         NORMAL_TEXT_SIZE, Color(), 2, false),
          new TextObject("", Point(500, r->height / 2 + 90),
                         NORMAL_TEXT_SIZE, Color(), 2, false),
          new TextObject("[completed]", position.Translate(Point(100, 100)),
                         NORMAL_TEXT_SIZE, Color(), 2, false),
          new TextObject("[completed]", position.Translate(Point(100, 135)),
                         NORMAL_TEXT_SIZE, Color(), 2, false),
          new TextObject("[completed]", position.Translate(Point(100, 170)),
                         NORMAL_TEXT_SIZE, Color(), 2, false),
          new TextObject("[completed]", position.Translate(Point(100, 205)),
                         NORMAL_TEXT_SIZE, Color(), 2, false),
    });

    for (auto it = labels.begin() + 5; it != labels.end(); it++) {
        (*it)->enabled = false;
    }
    descriptions = std::vector<std::string>({
        "Crybaby", "Midcore Gamer", "Osu! Lover", "Dark Souls Fan"
    });

    levelSpecs = std::vector<LevelSpec>(menuItems.size());
    for (int i = 0; i < menuItems.size() - 1; i++) {
        auto* s = dynamic_cast<SceneMenuItem*>(menuItems[i]);
        if (s) {
            auto s2 = dynamic_cast<GameFieldScene*>(sceneManager.GetScene(s->GetGameSceneName()).get());
            levelSpecs[i] = s2->GetSpec();
        }
    }
    MenuScene::Init();
}

void LevelsScene::Update(float dt) {
    if (selectedMenuItem < menuItems.size() - 1){
        labels[1]->text = descriptions[selectedMenuItem];
        labels[2]->text = "lives : " + std::to_string(levelSpecs[selectedMenuItem].lives);
        labels[3]->text = "invincibility : " + std::to_string(levelSpecs[selectedMenuItem].invTime) + "s";
        labels[4]->text = "asteroids : " + std::to_string(levelSpecs[selectedMenuItem].quantity);
    } else {
        labels[1]->text = "";
        labels[2]->text = "";
        labels[3]->text = "";
        labels[4]->text = "";
    }

    for (auto& kv : levelName2LabelIndex) {
        if (GameState::isLevelCompleted[kv.first]) {
            labels[kv.second]->enabled = true;
        }
    }

    MenuScene::Update(dt);
}


void TitleScene::Init() {
    position = Point(r->width / 2, TITLE_TEXT_SIZE / 2 + 50);
    Point center = Point(r->width / 2, r->height / 2 + 25);
    menuItems = std::vector<MenuItem*>({
           new SceneMenuItem(
                   "Infinite Mode", "SpaceBackgroundDark", "GameField", "", center),
           new SceneMenuItem(
                   "Select Level", "SpaceBackgroundLight", "Levels", "",
                   center.Translate(Point(0, NORMAL_TEXT_SIZE + 10))),
           new ExitMenuItem(
                   "Exit", center.Translate(Point(0, 2 * (NORMAL_TEXT_SIZE + 10))))
    });
    labels = std::vector<TextObject*>({
          new TextObject("Asteroids", position, TITLE_TEXT_SIZE, Color(), 10, true),
          new TextObject("Made by Nikita Krutoy", position.Translate(Point(140, 70)),
                         NORMAL_TEXT_SIZE + 10, Color(0.83529f, 0.83529f,	0.31765f), 2, true)
    });
    labels[1]->rotation = -15;
    MenuScene::Init();
}


void TitleScene::Update(float dt) {
    elapsedTime += dt;
    labels[1]->size = 3 * sin(elapsedTime * 10) + NORMAL_TEXT_SIZE + 10;
    MenuScene::Update(dt);
}

void GameOverScene::Init() {
    title = "Game Over"; EndGameScene::Init();
    im = Billboard(pepe_outline_png, pepe_outline_png_len);
    im.position = Point(0, r->height - im.height);
    im.SetRasterizer(r);
    EndGameScene::Init();
}
