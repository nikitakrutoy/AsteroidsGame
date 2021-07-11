//
// Created by Nikita Krutoy on 11.07.2021.
//

#ifndef GAME_GAMESTATE_CPP
#define GAME_GAMESTATE_CPP

#include <string>
#include <fstream>
#include <iostream>
#include "GameState.h"
#include "MagicNumbers.h"

unsigned int GameState::lives = MAX_LIVES;
unsigned int GameState::score = 0;
std::string GameState::savePath = "../data";
std::string GameState::currentGameFieldSceneName{};
std::map<std::string, bool> GameState::isLevelCompleted = {};

void GameState::setSavePath(std::string path) {
    savePath = path;
}

void GameState::Load() {
    std::string line, key;
    bool value;
    std::ifstream saveFile (savePath + "/save");
    if (saveFile.is_open())
    {
        while ( getline(saveFile,line) )
        {
            size_t pos = line.find(' ');
            key = line.substr(0, pos);
            value = bool(std::stoi(line.substr(pos + 1)));
            isLevelCompleted[key] = value;
        }
        saveFile.close();
    }
}

void GameState::Save() {
    std::ofstream saveFile (savePath + "/save");
    if (saveFile.is_open())
    {
        for (auto& kv : isLevelCompleted) {
            saveFile << kv.first << " " << int(kv.second) << std::endl;
        }
    }
    else std::cout << "Unable to open save file";
}

#endif //GAME_GAMESTATE_CPP