//
// Created by Nikita Krutoy on 07.07.2021.
//


#ifndef GAME_RASTERIZER_CPP
#define GAME_RASTERIZER_CPP

#include <cstdint>
#include <iostream>
#include <cmath>
#include "Geometry.h"
#include "Rasterizer.h"

#include "MagicNumbers.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h" /* http://nothings.org/stb/stb_truetype.h */

#include "Resources.h"


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
            std::min(B * v, 1.0f),
            std::min(A * v, 1.0f)
            );
}

Color Color::FromUInt32(uint32_t a) {
    uint8_t r, g, b;
    r = a >> 16;
    g = a >> 8;
    b = a;
    return Color(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f);
}

Color Color::Blend(Color c1, Color c2) {
    return Color(
            (1.0f - c2.A) * c1.R + c2.A * c2.R,
            (1.0f - c2.A) * c1.G + c2.A * c2.G,
            (1.0f - c2.A) * c1.B + c2.A * c2.B,
            1
    );
}


Rasterizer::Rasterizer(uint32_t *buffer, size_t height, size_t width) : buf(buffer), height(height), width(width) {
    fontBuffer = Minecraft_ttf;
    /* prepare font */
    if (!stbtt_InitFont(&fontInfo, fontBuffer, 0))
    {
        printf("failed\n");
    }
    isSeamless = true;
};

void Rasterizer::SetPixel(int x, int y, const Color &c)
{
    std::swap(x, y);
    if (isSeamless){
        x = (height + x) % height;
        y = (width + y) % width;
    }
    if (x >= height || y >= width || x <= 0 || y <= 0) return;

    buf[x * width + y] =Color::Blend(
            Color::FromUInt32(buf[x * width + y]), c).ToUInt32();
//    buf[x * width + y] = c.ToUInt32();
}

void Rasterizer::DrawBlob(Point p, int size, Color c) {
    for (int i = p.x - size; i <= p.x + size; i++) {
        for (int j = p.y - size; j <= p.y + size; j++) {
            SetPixel(i, j, c);
        }
    }
}

void Rasterizer::DrawLine(Point p1, Point p2, Color c) {
    float xdiff = (p2.x - p1.x);
    float ydiff = (p2.y - p1.y);

    if(xdiff == 0.0f && ydiff == 0.0f) {
        SetPixel(p1.x, p2.y, c);
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
            SetPixel(x, y, c);
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
            SetPixel(x, y, c);
        }
    }
}

void Rasterizer::DrawPath(Path& p, Color c) {
    int i;
    for (i = 0; i < p.data.size() - 1; i++) {
        DrawLine(p.data[i], p.data[i + 1], c);
    }
    if (p.isClosed)
        DrawLine(p.data[i], p.data[0], c);
}

void Rasterizer::DrawGlyph(unsigned char* b, int width, int height, Point position, float angle, Color color) {
    float x, y;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float r = b[i * width + j];
            Color c = color * (r / 255);
            x = j * std::cos(angle) - i * std::sin(angle);
            y = j * std::sin(angle) + i * std::cos(angle);
            SetPixel(position.x + x, position.y + y, c);
        }
    }
}

void Rasterizer::FillColor(Color c) {
    uint32_t array[height * width];
    std::fill(array, array + height * width, c.ToUInt32());
    memcpy(buf, array, height * width * sizeof(uint32_t));
}


void Rasterizer::DrawText(const std::string& text, Point position, float size, float angle,
                          Color c, float space, bool align) {
    float radians = M_PI * angle / 180;
    float scale = stbtt_ScaleForPixelHeight(&fontInfo, size);
    if (align) {
        int text_width = 0;
        for (char ch : text) {
            if (ch == char(32)) {
                text_width += space * SPACE_LENGTH;
                continue;
            }
            int ax;
            int lsb;
            stbtt_GetCodepointHMetrics(&fontInfo, ch, &ax, &lsb);
            text_width += roundf(ax * scale);
        }
        position = position.Translate(Point(
                -text_width / 2 * cos(radians),
                -text_width / 2 * sin(radians)
                ));
    }

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);

    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);
    unsigned char* bitmap = (unsigned char*) calloc(4 * size * size, sizeof(unsigned char));

    for (int i = 0; i < text.size(); ++i) {
        /* how wide is this character */
        char ch = text[i];
        int ax;
        int lsb;
        stbtt_GetCodepointHMetrics(&fontInfo, ch, &ax, &lsb);

        /* get bounding box for character (may be offset to account for chars that dip above or below the line */
        int c_x1, c_y1, c_x2, c_y2, w, h;
        stbtt_GetCodepointBitmapBox(&fontInfo, ch, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
        w = (c_x2 - c_x1);
        h = (c_y2 - c_y1);
        /* compute y (different characters have different heights */
        /* render character (stride and offset is important here) */
        stbtt_MakeCodepointBitmap(&fontInfo, bitmap, w, h, w, scale, scale, ch);

        DrawGlyph(bitmap, w, h, position.Translate(Point(
                -(ascent + c_y1) * sin(radians), (ascent + c_y1) * cos(radians))), radians, c);
        memset(bitmap, 0, 4 * size * size);

        position = position.Translate(Point(
                roundf(ax * scale) * cos(radians),
                roundf(ax * scale) * sin(radians)
        ));
        if (ch == char(32))
            position = position.Translate(Point(space * SPACE_LENGTH, 0));
    }
    free(bitmap);
}

void Rasterizer::DrawImage(const unsigned char *bf, const size_t w, const size_t h, Point p) {
    Color c;
    float a, v;
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            v = float(bf[j * 2 * w + 2 * i]) / 255.0f;
            a = float(bf[j * 2 * w + 2 * i + 1]);
            c = Color(v, v, v, a);
            SetPixel(p.x + i, p.y + j, c);
        }
    }
}


#endif //GAME_RASTERIZER_CPP
