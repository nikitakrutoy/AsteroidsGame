//
// Created by Nikita Krutoy on 08.07.2021.
//

#include "Geometry.h"
#include <cmath>

float frame(float x, int y)  {
    float empty = 0;
    return int(y + std::floor(x)) % y + std::modf(x, &empty);
}

float frame2(float x, int y_min, int y_max)  {
    if (x < y_min) {
        x = y_max - (y_min - x);
    }
    if (x > y_max) {
        x = y_min + (x - y_max);
    }
    return x;
}



float Distance(Point p1, Point p2) {
    return sqrtf(powf((p1.x  - p2.x), 2) + powf((p1.y - p2.y), 2));
}

Point Point::Translate(Point position) const {
    return {x + position.x, y +position.y};
}

Point Point::Scale(float scale) const {
    return {x * scale, y * scale};
}

Point Point::Rotate(float degree) const {
    return {
            x * std::cos(degree)  - y * std::sin(degree),
            x * std::sin(degree) + y * std::cos(degree)
    };
}

Point Point::Normalize() {
    float dist  = Distance(Point(0, 0), *this);
    return Scale(1 / dist);
}


Path Path::Rotate(float degree) {
    std::vector<Point> newData;
    newData.reserve(data.size());
    for (Point p : data) {
        newData.push_back(p.Rotate(degree));
    }
    return Path(newData, isClosed);
}


Path Path::Translate(Point position) {
    std::vector<Point> newData;
    newData.reserve(data.size());
    for (Point p : data) {
        newData.push_back(p.Translate(position));
    }
    return Path(newData, isClosed);
}

Path Path::Scale(float scale) {
    std::vector<Point> newData;
    newData.reserve(data.size());
    for (Point p : data) {
        newData.push_back(p.Scale(scale));
    }
    return Path(newData, isClosed);
}