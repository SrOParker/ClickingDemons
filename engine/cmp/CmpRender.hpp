#pragma once
#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <random>  // Para generación aleatoria mejorada
#include <algorithm>

enum class ShapeType {
    CIRCLE,
    RECTANGLE,
    TRIANGLE,
    PENTAGON,
    HEXAGON,
    OCTAGON,
    STAR
};

struct Shape {
    ShapeType type;
    Color color;
    float size;
    Vector2 position;
    bool hasBorder;
    Color borderColor;
    bool hasInnerShape;
    Color innerColor;
};

struct EnemySprite{
    std::vector<Vector2> spritePoli;
    std::vector<Vector2> spritePoliUpdated;
    std::vector<Vector2> eyes;
    std::vector<Vector2> mouth;
    Vector2 position;
    int sides;
    float radius;
    float variation;
    Color color;
};

struct CmpRender{
    Shape sprite;
    EnemySprite spriteE;
    
    CmpRender(float x, float y);
    CmpRender();
    CmpRender(int sides, float radius, float variation);
    Shape GenerateShape(Vector2 position);
    std::vector<Vector2> CreateDemonShape(int sides, float baseRadius, float chaos);
};