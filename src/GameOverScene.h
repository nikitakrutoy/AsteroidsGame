//
// Created by Nikita Krutoy on 09.07.2021.
//

#ifndef GAME_GAMEOVERSCENE_H
#define GAME_GAMEOVERSCENE_H

#include "Scene.h"

struct GameOverScene: Scene {
    std::string text;
    void Init() override {
        text = "Game Over";
        position = Point(r->width / 2, r->height / 2);
    };
    void Draw() override {
        r->fillColor();
        r->drawText(text, position, 24, 2, true);
    };
};


#endif //GAME_GAMEOVERSCENE_H
