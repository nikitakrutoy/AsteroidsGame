//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_RASTERIZER_H
#define GAME_RASTERIZER_H

#include <stb_truetype.h>
#include "Geometry.h"

struct Color {
    float R, G, B, A;
    explicit Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) :
            R(r), G(g), B(b), A(a) {};
    uint32_t ToUInt32() const;
    static Color FromUInt32(uint32_t a);
    static Color Blend(Color c1, Color c2);

    Color operator*(float v) const;
};

struct Rasterizer {
private:
    uint32_t* buf;
    unsigned char* fontBuffer;
    stbtt_fontinfo fontInfo;
    bool isSeamless = true;

public:
    size_t height, width;
    Rasterizer(uint32_t *buffer, size_t height, size_t width);
    void setPixel(int x, int y, const Color &c);
    void fillColor(Color c = Color(0, 0, 0));
    void drawBlob(Point p, int size = 1, Color c = Color());
    void drawLine(Point p1, Point p2, Color c = Color());
    void drawPath(Path& p, Color c = Color());
    void drawGlyph(unsigned char* b, int width, int height, Point position, float angle = 0,
                   Color color = Color());
    void drawText(const std::string& text, Point position, float size = 24, float angle = 0,
                  Color c = Color(), float space = 2, bool align = false);
    void drawImage(const unsigned char *bf, const size_t w, const size_t h, Point p);


    void enableSeamless() { isSeamless = true; };
    void disableSeamless() { isSeamless = false; };
};
#endif //GAME_RASTERIZER_H
