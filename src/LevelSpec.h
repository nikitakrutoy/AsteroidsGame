//
// Created by Nikita Krutoy on 10.07.2021.
//

#ifndef ASTEROIDS_LEVELSPEC_H
#define ASTEROIDS_LEVELSPEC_H

#include <ctime>

struct LevelSpec {
    size_t lives = 3;
    time_t invTime = 3;
    size_t quantity = 10;
    LevelSpec () = default;
    LevelSpec(size_t lives, time_t invTime, size_t quantity) :
            lives(lives), invTime(invTime), quantity(quantity) {};
    std::string ToString() {
        char buff[128];
        sprintf(buff, "lives: %zu\ninvicibility: %lds\nasteroids: %zu",
               lives, invTime, quantity);
        return std::string(buff);
    }
};


#endif //ASTEROIDS_LEVELSPEC_H
