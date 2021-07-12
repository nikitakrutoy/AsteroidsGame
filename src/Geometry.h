//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_GEOMETRY_H
#define GAME_GEOMETRY_H


#include <vector>
#include <cmath>

float frame(float x, int y);
float frame2(float x, int y_min, int y_max);
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

struct Point {
    float x, y;
    Point(): x(0), y(0) {};
    Point(float x, float y): x(x), y(y){};
    explicit Point(const float* p): x(p[0]), y(p[1]){};
    Point Translate(Point position) const;
    Point Scale(float scale) const;
    Point Rotate(float degree) const;
    Point Normalize();
};

float Distance(Point p1, Point p2);

struct Path {
    Path(std::initializer_list<Point> data, bool isClosed = true) : data(data), isClosed(isClosed) {}

    Path(std::vector<Point> data, bool isClosed = true) : data(std::move(data)), isClosed(isClosed) {}

    std::vector<Point> data;

    bool isClosed = true;
    Path Rotate(float degree);
    Path Translate(Point position);
    Path Scale(float scale);
};

#endif //GAME_GEOMETRY_H
