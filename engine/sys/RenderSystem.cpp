#include "RenderSystem.hpp"

RenderSystem::RenderSystem(){}

void RenderSystem::update(Manentity_type& EM) 
{ 
    auto update_one_entity = [&](Entity& e, CmpRender& crend, CmpPhysics& phy)
    { 
        if (e.hasTag<TRenderizable>())
            DrawShape(crend);
    };
    EM.foreach<SYSCMPs, SYSTAGs>(update_one_entity);
    
};

// Funciones para dibujar las formas

void DrawCircleShape(Shape& shape) {
    DrawCircleV(shape.position, shape.size, shape.color); // Dibuja el círculo

    if (shape.hasBorder) {
        DrawCircleLines(shape.position.x, shape.position.y, shape.size, shape.borderColor); // Borde de círculo
    }

    if (shape.hasInnerShape) {
        float innerSize = shape.size * 0.6f; // Tamaño más pequeño para forma interna
        DrawCircleV(shape.position, innerSize, shape.innerColor); // Forma interna
    }
}

void DrawRectangleShape(Shape& shape) {
    // Dibuja el rectángulo
    DrawRectangleV(shape.position, (Vector2){shape.size, shape.size * 2}, shape.color);

    if (shape.hasBorder) {
        DrawRectangleLines(shape.position.x, shape.position.y, shape.size, shape.size * 2, shape.borderColor); // Borde
    }

    if (shape.hasInnerShape) {
        float innerSize = shape.size * 0.5f; // Tamaño más pequeño para forma interna
        DrawRectangleV((Vector2){shape.position.x + (shape.size * 0.25f), shape.position.y + (shape.size * 0.25f)}, (Vector2){innerSize, innerSize * 2}, shape.innerColor); // Rectángulo interno
    }
}

void DrawTriangleShape(Shape& shape) {
    Vector2 p1 = {shape.position.x, shape.position.y - shape.size};
    Vector2 p2 = {shape.position.x - shape.size, shape.position.y + shape.size};
    Vector2 p3 = {shape.position.x + shape.size, shape.position.y + shape.size};
    DrawTriangle(p1, p2, p3, shape.color); // Dibuja el triángulo

    if (shape.hasBorder) {
        DrawTriangleLines(p1, p2, p3, shape.borderColor); // Borde del triángulo
    }

    if (shape.hasInnerShape) {
        Vector2 p4 = {shape.position.x, shape.position.y};
        DrawTriangle(p1, p2, p4, shape.innerColor); // Forma interna triangular
    }
}

void DrawPolygonShape(Shape& shape, int sides) {
    Vector2 points[sides];
    for (int i = 0; i < sides; ++i) {
        points[i] = (Vector2){
            (float)(shape.position.x + shape.size * cos(i * 2 * PI / sides)),
            (float)(shape.position.y + shape.size * sin(i * 2 * PI / sides))
        };
    }
    DrawPoly(shape.position, sides, shape.size,0, shape.color); // Dibuja el polígono

    if (shape.hasBorder) {
        DrawPolyLines(shape.position, sides, shape.size,0, shape.borderColor); // Borde del polígono
    }

    if (shape.hasInnerShape) {
        float innerSize = shape.size * 0.6f; // Tamaño más pequeño para forma interna
        DrawPoly(shape.position, sides, innerSize,0, shape.innerColor); // Forma interna
    }
}

void DrawStarShape(Shape& shape) {
    Vector2 points[10];
    for (int i = 0; i < 10; ++i) {
        float radius = (i % 2 == 0) ? shape.size : shape.size / 2;
        points[i] = (Vector2){
            (float)(shape.position.x + radius * cos(i * 2 * PI / 10)),
            (float)(shape.position.y + radius * sin(i * 2 * PI / 10))
        };
    }
    DrawPoly(shape.position, 10, shape.size,0, shape.color); // Dibuja la estrella

    if (shape.hasBorder) {
        DrawPolyLines(shape.position, 10, shape.size,0, shape.borderColor); // Borde de la estrella
    }

    if (shape.hasInnerShape) {
        float innerSize = shape.size * 0.5f; // Tamaño más pequeño para forma interna
        DrawPoly(shape.position, 10, innerSize,0, shape.innerColor); // Forma interna
    }
}

// Función para dibujar una forma
void RenderSystem::DrawShape(CmpRender& rend) {
    switch (rend.sprite.type) {
        case ShapeType::CIRCLE:
            DrawCircleShape(rend.sprite);
            break;
        case ShapeType::RECTANGLE:
            DrawRectangleShape(rend.sprite);
            break;
        case ShapeType::TRIANGLE:
            DrawTriangleShape(rend.sprite);
            break;
        case ShapeType::PENTAGON:
            DrawPolygonShape(rend.sprite, 5); // 5 lados para el pentágono
            break;
        case ShapeType::HEXAGON:
            DrawPolygonShape(rend.sprite, 6); // 6 lados para el hexágono
            break;
        case ShapeType::OCTAGON:
            DrawPolygonShape(rend.sprite, 8); // 8 lados para el octágono
            break;
        case ShapeType::STAR:
            DrawStarShape(rend.sprite);
            break;
    }
}