//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_RASTERIZER_H
#define GAME_RASTERIZER_H

#include <ft2build.h>
#include <freetype/freetype.h>
#include "Geometry.h"

struct Color {
    float R, G, B, A;
    explicit Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) :
            R(r), G(g), B(b), A(a) {};
    uint32_t ToUInt32() const;

    Color operator*(float v) const;
};

struct Rasterizer {
private:
    uint32_t* buf;
    FT_Library ft;
    FT_Face face;
    bool isSeamless = true;
public:
    size_t height, width;
    Rasterizer(uint32_t *buffer, size_t height, size_t width);
    void setPixel(int x, int y, const Color &c);
    void fillColor(Color c = Color(0, 0, 0));
    void drawBlob(Point p, int size = 1, Color c = Color());
    void drawLine(Point p1, Point p2, Color c = Color());
    void drawPath(Path& p, Color c = Color());
    void drawGlyph(FT_Bitmap& bm, Point position, Color c = Color());
    void drawChar(char ch, Point position, Color c = Color());
    void drawText(const std::string& text, Point position, float size = 24,
                  Color c = Color(), float space = 2, bool align = false);


    void enableSeamless() { isSeamless = true; };
    void disableSeamless() { isSeamless = false; };
};
#endif //GAME_RASTERIZER_H
