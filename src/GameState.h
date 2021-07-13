//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_GAMESTATE_H
#define GAME_GAMESTATE_H

#include <map>


struct GameState {
    static unsigned int lives;
    static unsigned int score;
    static std::map<std::string, bool> isLevelCompleted;
    static void setSavePath(std::string path);
    static void Load();
    static void Save();

private:
    static std::string savePath;
};
#endif //GAME_GAMESTATE_H
