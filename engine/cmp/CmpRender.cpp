#include "CmpRender.hpp"
CmpRender::CmpRender(){
    sprite = GenerateShape(Vector2{0,0});
}
CmpRender::CmpRender(float x, float y){
    sprite = GenerateShape(Vector2{x,y});
}

// Función para generar un color neón aleatorio
Color GenerateNeonColor(std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(100, 255); // Colores brillantes
    int r = dist(rng);
    int g = dist(rng);
    int b = dist(rng);
    return (Color){(unsigned char)r, (unsigned char)g, (unsigned char)b, 255}; // Color con alto brillo
}

// Función para generar una forma aleatoria
Shape CmpRender::GenerateShape(Vector2 position) {
    // Generar tipo aleatorio de forma
    std::random_device rd;
    std::mt19937 rng(rd()); // Generador de números aleatorios
    ShapeType type = static_cast<ShapeType>(rng() % 8); // Genera un valor entre 0 y 7

    Shape shape;
    shape.type = type;
    shape.position = position;
    shape.color = GenerateNeonColor(rng); // Color de la forma (neón)
    shape.size = 50; // Tamaño 50
    shape.hasBorder = rng() % 2 == 0; // Aleatoriamente decide si tiene borde
    shape.borderColor = GenerateNeonColor(rng); // Color de borde
    shape.hasInnerShape = rng() % 2 == 0; // Aleatoriamente decide si tiene forma interna
    shape.innerColor = GenerateNeonColor(rng); // Color de forma interna

    return shape;
}