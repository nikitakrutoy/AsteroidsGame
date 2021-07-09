//
// Created by Nikita Krutoy on 09.07.2021.
//

#include "MenuItem.h"
MenuItem::MenuItem(std::string text, Point p, bool align):
    text(text), align(align){
    position = p;
}