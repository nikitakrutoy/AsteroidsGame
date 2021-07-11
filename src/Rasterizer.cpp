//
// Created by Nikita Krutoy on 07.07.2021.
//


#ifndef GAME_RASTERIZER_CPP
#define GAME_RASTERIZER_CPP

#include <ft2build.h>
#include <freetype/freetype.h>
#include <cstdint>
#include <iostream>
#include <cmath>
#include "Geometry.h"
#include "Rasterizer.h"

#include "MagicNumbers.h"


uint32_t Color::ToUInt32() const {
    auto rgb = uint32_t(std::min(int(B * 255), 255));
    rgb += uint32_t(std::min(int(G * 255), 255)) << 8;
    rgb += uint32_t(std::min(int(R * 255), 255)) << 16;
    return rgb;
}

Color Color::operator*(float v) const {
    return Color(
            std::min(R * v, 1.0f),
            std::min(G * v, 1.0f),
            std::min(B * v, 1.0f)
            );
}


Rasterizer::Rasterizer(uint32_t *buffer, size_t height, size_t width) : buf(buffer), height(height), width(width) {
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
    auto error = FT_New_Face(ft, "../resources/fonts/Minecraft.ttf", 0, &face);
    if (error)
    {
        if ( error == FT_Err_Unknown_File_Format )
        {
            std::cout << "ERROR::FREETYPE: Wrong file format" << std::endl;
        }
        else if ( error )
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        }
    }
    isSeamless = true;
};

void Rasterizer::setPixel(int x, int y, const Color &c)
{
    std::swap(x, y);
    if (isSeamless){
        x = (height + x) % height;
        y = (width + y) % width;
    }
    if (x >= height || y >= width || x <= 0 || y <= 0) return;

    buf[x * width + y] = c.ToUInt32();
}

void Rasterizer::drawBlob(Point p, int size, Color c) {
    for (int i = p.x - size; i <= p.x + size; i++) {
        for (int j = p.y - size; j <= p.y + size; j++) {
            setPixel(i, j, c);
        }
    }
}

void Rasterizer::drawLine(Point p1, Point p2, Color c) {
    float xdiff = (p2.x - p1.x);
    float ydiff = (p2.y - p1.y);

    if(xdiff == 0.0f && ydiff == 0.0f) {
        setPixel(p1.x, p2.y, c);
        return;
    }

    if(fabs(xdiff) > fabs(ydiff)) {
        float xmin, xmax;

        // set xmin to the lower x value given
        // and xmax to the higher value
        if(p1.x < p2.x) {
            xmin = p1.x;
            xmax = p2.x;
        } else {
            xmin = p2.x;
            xmax = p1.x;
        }

        // draw line in terms of y slope
        float slope = ydiff / xdiff;
        for(float x = xmin; x <= xmax; x += 1.0f) {
            float y = p1.y + ((x - p1.x) * slope);
            setPixel(x, y, c);
        }
    } else {
        float ymin, ymax;

        // set ymin to the lower y value given
        // and ymax to the higher value
        if(p1.y < p2.y) {
            ymin = p1.y ;
            ymax = p2.y;
        } else {
            ymin = p2.y;
            ymax = p1.y;
        }

        // draw line in terms of x slope
        float slope = xdiff / ydiff;
        for(float y = ymin; y <= ymax; y += 1.0f) {
            float x = p1.x + ((y - p1.y) * slope);
            setPixel(x, y, c);
        }
    }
}

void Rasterizer::drawPath(Path& p, Color c) {
    int i;
    for (i = 0; i < p.data.size() - 1; i++) {
        drawLine(p.data[i], p.data[i + 1], c);
    }
    if (p.isClosed)
        drawLine(p.data[i], p.data[0], c);
}

void Rasterizer::drawGlyph(FT_Bitmap& bm, Point position, Color color) {
    for (int i = 0; i < bm.rows; i++) {
        for (int j = 0; j < bm.width; j++) {
            float r = bm.buffer[i * bm.width + j] / 255;
            Color c = color * r;
            setPixel(position.x + j, position.y + i, c);
            ;
        }
    }
}

void Rasterizer::drawChar(char ch, Point position, Color color) {
    FT_Set_Pixel_Sizes(face, 0, 48);
    if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
    }
    auto error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    FT_Bitmap bm = face->glyph->bitmap;
    drawGlyph(bm, position, color);
}

void Rasterizer::fillColor(Color c) {
    uint32_t array[height * width];
    std::fill(array, array + height * width, c.ToUInt32());
    memcpy(buf, array, height * width * sizeof(uint32_t));
}


void Rasterizer::drawText(const std::string& text, Point position, float size, Color c, float space, bool align) {
    FT_Set_Pixel_Sizes(face, 0, size);
    if (align) {
        int text_width = 0;
        for (char ch : text) {
            if (ch == char(32)) {
                text_width += space * SPACE_LENGTH;
                continue;
            }

            if (FT_Load_Char(face, ch, FT_LOAD_NO_BITMAP)) {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            }
            text_width += face->glyph->bitmap.width + space;
        }
        text_width -= space;
        position = position.Translate(Point(-text_width / 2, 0));
    }

    for (char ch : text) {
        if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        }
        FT_Bitmap bm = face->glyph->bitmap;
        int bt = face->glyph->bitmap_top;
        drawGlyph(bm, position.Translate(Point(0, (size-bt))), c);
        position = position.Translate(Point(bm.width + space, 0));
        if (ch == char(32))
            position = position.Translate(Point(space * SPACE_LENGTH, 0));
    }
}



#endif //GAME_RASTERIZER_CPP
