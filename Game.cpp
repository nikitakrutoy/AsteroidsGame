#include "Engine.h"
#include <algorithm>
#include <utility>
#include <vector>
#include <cmath>
#include <random>
#include <iostream>
#include <ft2build.h>
#include <freetype/freetype.h>


#define _USE_MATH_DEFINES


#define NANOSVG_IMPLEMENTATION	// Expands implementation
#include "nanosvg.h"

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, VK_RETURN)
//
//  schedule_quit_game() - quit game after act()

class Color
{
public:
    float R, G, B, A;

    Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) :
        R(r), G(g), B(b), A(a) {};

    uint32_t ToUInt32() const {
        auto rgb = uint32_t(B * 255);
        rgb += uint32_t(G * 255) << 8;
        rgb += uint32_t(R * 255) << 16;
        return rgb;
    }
};
struct Point;
float Distance(Point p1, Point p2);

struct Point {
    float x, y;
    Point(): x(0), y(0) {};
    Point(float x, float y): x(x), y(y){};
    explicit Point(const float* p) {
        x = p[0];
        y = p[1];
    }

    Point Translate(Point position) {
        return Point(x + position.x, y +position.y);
    }

    Point Scale(float scale) {
        return Point(x * scale, y * scale);
    }

    Point Rotate(float degree) {
        return Point(
                x * std::cos(degree)  - y * std::sin(degree),
                x * std::sin(degree) + y * std::cos(degree)
                );
    }

    Point Normalize() {
        float dist  = Distance(Point(0, 0), *this);
        return Scale(1 / dist);
    }
};

struct Path {
    Path(std::initializer_list<Point> data, bool isClosed = true): data(data), isClosed(isClosed) {}
    Path(std::vector<Point> data, bool isClosed = true): data(std::move(data)), isClosed(isClosed) {}
    std::vector<Point> data;

    bool isClosed = true;

    Path Rotate(float degree) {
        std::vector<Point> newData;
        newData.reserve(data.size());
        for (Point p : data) {
            newData.push_back(p.Rotate(degree));
        }
        return Path(newData, isClosed);
    }


    Path Translate(Point position) {
        std::vector<Point> newData;
        newData.reserve(data.size());
        for (Point p : data) {
            newData.push_back(p.Translate(position));
        }
        return Path(newData, isClosed);
    }

    Path Scale(float scale) {
        std::vector<Point> newData;
        newData.reserve(data.size());
        for (Point p : data) {
            newData.push_back(p.Scale(scale));
        }
        return Path(newData, isClosed);
    }
};

struct GameObject {
    GameObject(Path& path): path(path) {} ;

    Path path;
    Point position = {0, 0};
    float scale = 1;


    Point forwardDirection = {1, 0};
    Point velocity = {0, 0};
    float speed = 1;

    void Rotate(float degree) {
        path = path.Rotate(degree);
        forwardDirection = forwardDirection.Rotate(degree);
    }

    virtual void UpdatePosition() {
        position = position.Translate(velocity.Scale(speed));
        position = Point(
                (SCREEN_WIDTH + int(position.x)) % SCREEN_WIDTH,
                (SCREEN_HEIGHT + int(position.y)) % SCREEN_HEIGHT);
    }

    void Boost(float amount) {
        velocity = velocity.Translate(forwardDirection.Scale(amount));
    }
};

struct Projectile: GameObject {
    Projectile(Path &path) : GameObject(path) {};
    void UpdatePosition() override {
        position = position.Translate(velocity.Scale(speed));
    }
};

struct Asteroid: GameObject {
    float radius = 5;
    Asteroid(Path path): GameObject(path) {};
};

Asteroid makeAsteroid(float radius = 32) {

    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<float>  distr(radius - 3, radius + 3);
    std::uniform_real_distribution<float>  dist2(0, 1);

    std::vector<Point> data;
    data.reserve(20);
    for (int i = 0; i < 20; i++) {

        float radian = 360 / 20 * i * (M_PI/180);
        data.push_back(
                Point(
                        std::cos(radian) * distr(generator),
                        std::sin(radian) * distr(generator)
                )
        );
    }
    Path path = Path(data);
    Asteroid result = Asteroid(path);
    result.position = Point(
            dist2(generator) * SCREEN_WIDTH,
            dist2(generator) * SCREEN_HEIGHT
    );
    result.velocity = Point(dist2(generator) + 1, dist2(generator) + 1);
    result.radius = radius;
    return result;
}

class GameState {
public:
    static unsigned int lives;
    static unsigned int score;
};

unsigned int GameState::lives = 3;
unsigned int GameState::score = 0;

Path path = Path({Point(-25, -15), Point(30, 0), Point(-25, 15), Point(-10, 0)});
GameObject gameObject = GameObject(path);
Point startPoint = Point(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);


std::vector<Asteroid> Asteroids;
std::vector<Projectile> Projectiles;

float Distance(Point p1, Point p2) {
    return std::sqrtf(std::powf((p1.x  - p2.x), 2) + std::powf((p1.y - p2.y), 2));
}

bool detectCollisions(std::vector<Asteroid>& as, Point p) {
    bool result = false;
    as.reserve(as.size() * 2);
    for (auto it = as.begin(); it != as.end();) {
        if (Distance(it->position, p) <= it->radius) {
            GameState::score += it->radius;
            if (it->radius / 2 >= 8) {
                Asteroid a1 = makeAsteroid(it->radius / 2);
                Asteroid a2 = makeAsteroid(it->radius / 2);
                a1.position = it->position.Translate(Point(it->radius / 2, 0));
                a2.position = it->position.Translate(Point(-it->radius / 2, 0));
                as.erase(it);
                as.push_back(a1);
                as.push_back(a2);
//                as.insert( as.end(), {a1} );
            } else {
                as.erase(it);
            }


            result = true;
        }
        else {
            ++it;
        }
    }
    as.shrink_to_fit();
    return result;
}

void setPixel(unsigned int x, unsigned int y, const Color &c)
{
    std::swap(x, y);

    x = (SCREEN_HEIGHT + x) % SCREEN_HEIGHT;
    y = (SCREEN_WIDTH + y) % SCREEN_WIDTH;
//    if(x >= SCREEN_HEIGHT || y >= SCREEN_WIDTH)
//        return;

    buffer[x][y] = c.ToUInt32();
}

void drawBlob(Point p, int size = 1, Color c = Color()) {
    for (int i = p.x - size; i <= p.x + size; i++) {
        for (int j = p.y - size; j <= p.y + size; j++) {
            setPixel(i, j, c);
        }
    }
}

void drawLine(Point p1, Point p2, Color c = Color()) {
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

void drawPath(Path& p) {
    int i;
    for (i = 0; i < p.data.size() - 1; i++) {
        drawLine(p.data[i], p.data[i + 1]);
    }
    if (p.isClosed)
        drawLine(p.data[i], p.data[0]);
}

void drawShape(GameObject& s) {
    Path p = s.path.Scale(s.scale).Translate(s.position);
    drawPath(p);
}
FT_Library ft;
FT_Face face;

void drawGlyph(FT_Bitmap& bm, Point position) {
    for (int i = 0; i < bm.rows; i++) {
        for (int j = 0; j < bm.width; j++) {
            float r = bm.buffer[i * bm.width + j] / 255;
            Color c = Color(r ,r ,r);
            setPixel(position.x + j, position.y + i, c);
            ;
        }
    }
}

void drawChar(char ch, Point position) {
    FT_Set_Pixel_Sizes(face, 0, 48);
    if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
    }
    auto error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    FT_Bitmap bm = face->glyph->bitmap;
    drawGlyph(bm, position);
}



void drawText(std::string text, Point position, float size = 24, float space = 2) {
    FT_Set_Pixel_Sizes(face, 0, size);
    for (char ch : text) {
        if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        }
        FT_Bitmap bm = face->glyph->bitmap;
        drawGlyph(bm, position);
        position = position.Translate(Point(bm.width + space, 0));
    }
}
// initialize game data in this function
void initialize()
{
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
    auto error = FT_New_Face(ft, "/Users/nikitakrutoy/Downloads/GameTemplateLinux/resources/fonts/Minecraft.ttf", 0, &face);
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

    gameObject.position = startPoint;
    for (int i = 0; i < 5; i ++) {
        Asteroids.push_back(makeAsteroid());
    }
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)

bool wasSpacePressed = false;
void act(float dt)
{
    gameObject.UpdatePosition();
    for (GameObject& a : Asteroids) {
        a.UpdatePosition();
    }

    for (GameObject& p : Projectiles) {
        p.UpdatePosition();
    }

    if (is_key_pressed(VK_ESCAPE))
    schedule_quit_game();

    if (is_key_pressed(VK_RIGHT))
        gameObject.Rotate(2 * dt);

    if (is_key_pressed(VK_LEFT))
        gameObject.Rotate(-2 * dt);

    if (is_key_pressed(VK_UP))
        gameObject.Boost(0.1);
    if (is_key_pressed(VK_SPACE) && !wasSpacePressed) {
        wasSpacePressed = true;
    }

    if (!is_key_pressed(VK_SPACE) && wasSpacePressed) {
        Path empty = Path({});
        Projectile projectile = Projectile(empty);
        projectile.velocity = gameObject.forwardDirection.Scale(5);
        projectile.position = gameObject.position;
        Projectiles.push_back(projectile);
        wasSpacePressed = false;
    }
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, buffer)
void draw()
{
    Color backgroundColor = Color(0.0f,0.0f,0.0f);
    uint32_t array[SCREEN_HEIGHT * SCREEN_WIDTH];
    std::fill(array, array+SCREEN_HEIGHT * SCREEN_WIDTH, backgroundColor.ToUInt32());
    memcpy(buffer, array, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

    if (GameState::lives <= 0) {
        drawText("Defeat", startPoint.Translate(Point(-100, -12)), 48);
        return;
    }

    if (detectCollisions(Asteroids, gameObject.position)) {
        std::cout << "Starship crashed!" << std::endl;
        gameObject.position = startPoint;
        gameObject.velocity = Point(0, 0);
        GameState::lives -= 1;
    }

    drawShape(gameObject);
    for (GameObject &a : Asteroids) {
        drawShape(a);
    }

    for (auto it = Projectiles.begin(); it != Projectiles.end();) {
        Point p = it->position;
        bool notInsideScreen = p.x > SCREEN_WIDTH || p.x < 0 || p.y > SCREEN_HEIGHT || p.y < 0;
        if (notInsideScreen || detectCollisions(Asteroids, p))
            Projectiles.erase(it);
        else {
            drawBlob(it->position, 1);
            ++it;
        }
    }

    drawText("Score " + std::to_string(GameState::score), Point(10, 10));
    drawText("Lives " + std::to_string(GameState::lives), Point(10, 30));

    drawLine(
            gameObject.position,
            gameObject.position.Translate(gameObject.forwardDirection.Scale(40)),
            Color(0, 0, 1)
    );

    drawLine(
            gameObject.position,
            gameObject.position.Translate(gameObject.velocity.Normalize().Scale(40)),
            Color(0, 1, 0)
    );
//    memset(buffer, 65280, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
//    rgb = 0;
}

// free game data in this function
void finalize()
{
}

