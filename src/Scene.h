//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "GameObject.h"

struct Scene: GameObject {
    virtual void Init() {};
};

#endif //GAME_SCENE_H