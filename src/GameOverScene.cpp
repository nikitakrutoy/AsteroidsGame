//
// Created by Nikita Krutoy on 09.07.2021.
//

#include "GameOverScene.h"
#include "GameState.h"
#include "MagicNumbers.h"

void GameOverScene::Draw() {
    r->fillColor();
    r->drawText("Game Over", position, GAMEOVER_TEXT_SIZE, Color(), 2, true);
    r->drawText(
            "Your score is " + std::to_string(GameState::score),
            position.Translate(Point(0, GAMEOVER_TEXT_SIZE + 10)),
            NORMAL_TEXT_SIZE,
            Color(),
            2, true);
    for (auto& m : menuItems) {
        m->SafeDraw();
    }
}
