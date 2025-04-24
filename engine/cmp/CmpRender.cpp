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

CmpRender::CmpRender(int sides, float radius, float variation)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> distSides(100, 200);        // [65, 164]
    std::uniform_real_distribution<float> distRadius(30.f, 130.f); // [30.0, 130.0]
    std::uniform_real_distribution<float> distVar(10.f, 50.f);     // [10.0, 50.0]
    std::uniform_real_distribution<float> what(10.f, 50.f); 
    if (sides == 0 && radius == 0 && variation == 0) {
        sides = distSides(rng);
        radius = distRadius(rng);
        variation = distVar(rng);
    }
    spriteE.sides = sides;
    spriteE.variation = variation;
    spriteE.radius = radius;
    spriteE.color = GenerateNeonColor(rng);
    spriteE.spritePoli = CreateDemonShape(sides,radius,variation);
    spriteE.spritePoliUpdated = spriteE.spritePoli;
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
    shape.size = 30; // Tamaño 30
    shape.hasBorder = rng() % 2 == 0; // Aleatoriamente decide si tiene borde
    shape.borderColor = GenerateNeonColor(rng); // Color de borde
    shape.hasInnerShape = rng() % 2 == 0; // Aleatoriamente decide si tiene forma interna
    shape.innerColor = GenerateNeonColor(rng); // Color de forma interna

    return shape;
}

// Función para calcular el producto cruzado
float CrossProduct(const Vector2& a, const Vector2& b, const Vector2& c) {
    float abX = b.x - a.x;
    float abY = b.y - a.y;
    float bcX = c.x - b.x;
    float bcY = c.y - b.y;
    return abX * bcY - abY * bcX;
}

// Algoritmo de Envolvente Convexa (Convex Hull)
std::vector<Vector2> GetConvexHull(std::vector<Vector2>& points) {
    if (points.size() <= 1) return points;

    // Ordenar los puntos por coordenadas X, luego por Y
    std::sort(points.begin(), points.end(), [](const Vector2& a, const Vector2& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });

    std::vector<Vector2> hull;

    // Construir la parte inferior de la envolvente
    for (const auto& point : points) {
        while (hull.size() >= 2 && CrossProduct(hull[hull.size() - 2], hull[hull.size() - 1], point) <= 0) {
            hull.pop_back();
        }
        hull.push_back(point);
    }

    // Construir la parte superior de la envolvente
    size_t t = hull.size() + 1;
    for (size_t i = points.size() - 1; i != SIZE_MAX; --i) {
        while (hull.size() >= t && CrossProduct(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    hull.pop_back();  // El primer punto se repite al final, lo eliminamos

    return hull;
}

// Generar un ángulo aleatorio
float GetRandomAngle(float minAngle, float maxAngle) {
    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_real_distribution<float> angleDist(minAngle, maxAngle);
    return angleDist(rng);
}

// Generar un radio aleatorio dentro de un rango
float GetRandomRadius(float minRadius, float maxRadius) {
    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_real_distribution<float> radiusDist(minRadius, maxRadius);
    return radiusDist(rng);
}

// Generar un vértice de un polígono aleatorio
Vector2 GenerateVertex(float radius, float angle) {
    return { (float)(radius * cos(angle)), (float)(radius * sin(angle)) };
}


// Función para generar los ojos, posicionados en base a un centro
std::vector<Vector2> GenerateEyes(float headRadius, Vector2 center) {
    std::vector<Vector2> eyes;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> distEyeOffset(-headRadius * 0.2f, headRadius * 0.2f);  // Variación en la posición X y Y
    std::uniform_real_distribution<float> distEyeSize(0.1f, 0.2f); // Tamaño de los ojos (relativo al radio de la cabeza)
    
    // Variaciones aleatorias
    float eyeOffsetY = distEyeOffset(rng); // Variación aleatoria en la posición Y (siendo negativa más cerca de la parte superior de la cabeza)
    float eyeOffsetX = distEyeOffset(rng); // Variación aleatoria en la posición X
    float eyeSize = distEyeSize(rng); // Variación aleatoria en el tamaño de los ojos
    
    // Posiciones relativas a 'center' con variación
    Vector2 leftEye = { center.x - eyeOffsetX, center.y + eyeOffsetY - headRadius * 0.2f };  // Los ojos estarán sobre la boca
    Vector2 rightEye = { center.x + eyeOffsetX, center.y + eyeOffsetY - headRadius * 0.2f };

    eyes.push_back(leftEye);
    eyes.push_back(rightEye);

    return eyes;
}

std::vector<Vector2> GenerateMouth(float headRadius, Vector2 center) {
    std::vector<Vector2> mouthPoints;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> distMouthSize(0.1f, 0.25f);  // Tamaño de la boca
    std::uniform_real_distribution<float> distMouthOpeness(0.1f, 0.3f); // Apertura de la boca
    std::uniform_int_distribution<int> distMouthPoints(8, 16);  // Cantidad de puntos de la boca (más o menos resolución)
    
    float mouthRadius = headRadius * distMouthSize(rng);  // Variación en tamaño
    float mouthOpeness = distMouthOpeness(rng);  // Variación en la apertura de la boca
    int mouthPointsCount = distMouthPoints(rng); // Número aleatorio de puntos para la boca
    
    // Centro de la boca (debajo del centro de la cabeza)
    Vector2 mouthCenter = { center.x, center.y + headRadius * 0.2f + mouthOpeness };

    for (int i = 0; i <= mouthPointsCount; ++i) {
        float t = (float)i / mouthPointsCount;
        float angle = PI + (t * PI);  // Semicírculo inferior
        float x = mouthCenter.x + cosf(angle) * mouthRadius;
        float y = mouthCenter.y + sinf(angle) * mouthRadius;
        mouthPoints.push_back({ x, y });
    }

    return mouthPoints;
}

// Función principal para crear la figura demoníaca
std::vector<Vector2> CmpRender::CreateDemonShape(int sides, float baseRadius, float chaos) {
    std::vector<Vector2> vertices;

    if (sides < 12) sides = 12;  // Aseguramos que haya al menos 12 vértices

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> noise(-chaos, chaos);

    // Generar la cabeza (parte superior)
    float headRadius = baseRadius * 1.2f;
    for (int i = 0; i < sides / 8; ++i) {
        float angle = GetRandomAngle(0, 2 * PI);
        float radius = headRadius + noise(rng) * 0.3f;
        vertices.push_back(GenerateVertex(radius, angle));
    }

    // Generar los cuernos (curvados, en dos partes)
    float hornRadius = baseRadius * 1.8f;
    for (int i = 0; i < 2; ++i) {
        float angleStart = GetRandomAngle(-PI / 4, PI / 4);
        for (int j = 0; j < 4; ++j) {
            float angle = angleStart + j * PI / 8;
            float radius = hornRadius + noise(rng) * 0.5f;
            vertices.push_back(GenerateVertex(radius, angle));
        }
    }

    // Generar el torso (cuerpo humanoide)
    for (int i = 0; i < sides / 4; ++i) {
        float angle = (i * PI) / (sides / 4);  // Dividir en ángulos para el torso
        float radius = baseRadius + noise(rng) * 0.6f;
        vertices.push_back(GenerateVertex(radius, angle));
    }

    // Extremidades (piernas)
    float legRadius = baseRadius * 0.6f;
    for (int i = 0; i < sides / 6; ++i) {
        float angle = GetRandomAngle(-PI / 2, PI / 2);
        float radius = legRadius + noise(rng) * 0.4f;
        vertices.push_back(GenerateVertex(radius, angle));
    }

    // Organizar los puntos para evitar cruces de líneas
    std::vector<Vector2> orderedVertices = GetConvexHull(vertices);

    // Cerrar el polígono (unir el último punto con el primero)
    orderedVertices.push_back(orderedVertices[0]);
    spriteE.eyes = GenerateEyes(spriteE.radius, spriteE.position);
    spriteE.mouth = GenerateMouth(spriteE.radius, spriteE.position);
    return orderedVertices;
}
