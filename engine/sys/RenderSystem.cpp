#include "RenderSystem.hpp"

RenderSystem::RenderSystem(){}

void RenderSystem::update(Manentity_type& EM) 
{ 
    PaintGameInterface();
    auto update_one_entity = [&](Entity& e, CmpRender& crend, CmpPhysics& phy)
    { 
        float deltaTime = GetFrameTime();
        if(e.hasTag<TEnemy>()){
            UpdateDemonShapeWithReturn(crend.spriteE.spritePoliUpdated, crend.spriteE.spritePoli, deltaTime) ;
            DrawDemonShape(crend.spriteE.spritePoliUpdated, crend.spriteE.position, crend.spriteE.color, crend.spriteE.eyes, crend.spriteE.mouth);
           
        } else if (e.hasTag<TRenderizable>() && e.hasTag<TJewel>() && e.hasTag<TActiveJewel>()){
            DrawShape(crend);
        } else if (e.hasTag<TTutorial>()){
            DrawShape(crend);
        }
        
        
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
    float shapeReposX = 15;
    float shapeReposY = 28;
    DrawRectangleV(Vector2(shape.position.x -shapeReposX, shape.position.y -shapeReposY), (Vector2){shape.size, shape.size * 2}, shape.color);

    if (shape.hasBorder) {
        DrawRectangleLines(shape.position.x-shapeReposX, shape.position.y-shapeReposY, shape.size, shape.size * 2, shape.borderColor); // Borde
    }

    if (shape.hasInnerShape) {
        float innerSize = shape.size * 0.5f; // Tamaño más pequeño para forma interna
        DrawRectangleV((Vector2){shape.position.x-shapeReposX + (shape.size * 0.25f), shape.position.y-shapeReposY + (shape.size * 0.25f)}, (Vector2){innerSize, innerSize * 2}, shape.innerColor); // Rectángulo interno
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

void RenderSystem::DrawDemonShape(const std::vector<Vector2>& vertices, Vector2 position, Color color, const std::vector<Vector2>& eyes, const std::vector<Vector2>& mouth) {
    // Crear un vector de vértices desplazados según la posición deseada
    std::vector<Vector2> translatedVertices;
    for (const auto& vertex : vertices) {
        translatedVertices.push_back({ vertex.x + position.x, vertex.y + position.y });
    }

    // === OJOS CON PUPILAS DE SERPIENTE ===
    Color eyeColor = RED;  // Generamos un color aleatorio para los ojos
    for (const auto& eye : eyes) {
        Vector2 eyePos = { eye.x + position.x, eye.y + position.y };
        float radius = 6;

        // Círculo del ojo
        DrawCircleV(eyePos, radius, eyeColor);

        // Pupila en forma de línea vertical
        float pupilaHeight = radius * 1.2f;
        Vector2 top = { eyePos.x, eyePos.y - pupilaHeight / 2 };
        Vector2 bottom = { eyePos.x, eyePos.y + pupilaHeight / 2 };
        DrawLineEx(top, bottom, 2, BLACK);
    }

    // === BOCA CON COLMILLOS ===
    Color mouthColor = color;  // Generamos un color aleatorio para la boca
    for (size_t i = 0; i < mouth.size() - 1; ++i) {
        Vector2 p1 = { mouth[i].x + position.x, mouth[i].y + position.y };
        Vector2 p2 = { mouth[i + 1].x + position.x, mouth[i + 1].y + position.y };
        DrawLineV(p1, p2, mouthColor);
    }

    // Colmillos: ubicamos un par en puntos clave, pero con variación aleatoria
    if (mouth.size() >= 3) {
        // Definimos los índices para los colmillos
        std::vector<size_t> fangIndices = {1, mouth.size() / 2, mouth.size() - 2};

        // Dibujar los colmillos
        for (size_t i : fangIndices) {
            Vector2 base = { mouth[i].x + position.x, mouth[i].y + position.y };
            
            // Variación aleatoria en el tamaño y ángulo de los colmillos
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_real_distribution<float> distSize(6.0f, 12.0f); // Tamaño del colmillo
            std::uniform_real_distribution<float> distAngle(-0.1f, 0.1f); // Ángulo de variación del colmillo

            float fangLength = distSize(rng);
            float angleOffset = distAngle(rng);

            // Ajustar la posición vertical de los colmillos para asegurarnos de que estén debajo de la boca
            Vector2 fangBaseBelowMouth = { base.x, base.y + 6 };  // Movemos los colmillos hacia abajo un poco más
            Vector2 fangTip = { fangBaseBelowMouth.x, fangBaseBelowMouth.y - fangLength };  // Colmillo hacia abajo

            // Usamos un color aleatorio para los colmillos
            Color fangColor = WHITE;

            // Dibujar el colmillo
            DrawLineV(fangBaseBelowMouth, fangTip, fangColor);
        }
    }

    // Dibujar el contorno del polígono (líneas)
    for (size_t i = 0; i < vertices.size(); ++i) {
        size_t nextIndex = (i + 1) % vertices.size();
        Vector2 p1 = { vertices[i].x + position.x, vertices[i].y + position.y };
        Vector2 p2 = { vertices[nextIndex].x + position.x, vertices[nextIndex].y + position.y };
        DrawLineV(p1, p2, color);
    }
}

void RenderSystem::UpdateDemonShapeWithReturn(std::vector<Vector2>& vertices, const std::vector<Vector2>& originalVertices, float deltaTime) {
    static float timeElapsed = 0.0f;  // Tiempo acumulado para la animación
    float moveSpeed = 2.0f;  // Velocidad de la interpolación, puedes ajustarla

    // Incrementamos el tiempo transcurrido
    timeElapsed += deltaTime;

    // Factor de interpolación basado en el tiempo transcurrido
    // Oscilamos entre 0 y 1 utilizando la función seno para obtener un movimiento suave
    float t = (sin(timeElapsed * moveSpeed) + 1.0f) * 0.5f;  // [0, 1] a partir de la función seno

    // Mover cada vértice de forma continua hacia su posición intermedia
    for (size_t i = 0; i < vertices.size(); ++i) {
        // Interpolamos entre la posición original y la animada
        vertices[i].x = originalVertices[i].x + (sin(timeElapsed + i) * 30.0f) * t;  // Movimiento suave en X
        vertices[i].y = originalVertices[i].y + (cos(timeElapsed + i) * 30.0f) * t;  // Movimiento suave en Y
    }
}

void RenderSystem::PaintGameInterface(){
    Vector2 joyaTargetPos = {100, 120};
    // Dibujo de los huecos para joyas (más grandes ahora)
    for (int i = 0; i < 5; ++i) {
        // Dibujo del recuadro más grande para las joyas
        Rectangle slot = { joyaTargetPos.x - 25, joyaTargetPos.y - 25 + i * 100, 50, 50 }; // Ajusta el tamaño de los recuadros
        DrawRectangleRounded(slot, 0.2f, 4, Fade(GRAY, 0.25f));
    }
}