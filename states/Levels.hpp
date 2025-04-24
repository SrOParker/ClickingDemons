#pragma once
#include "raylib.h"
#include <string>
#include <vector>
#include "../engine/utils/types.hpp"
#include "../engine/sys/RenderSystem.hpp"
#include "../engine/sys/PhysicSystem.hpp"
#include "../engine/sys/InputSystem.hpp"
#include "../engine/sys/InformationSystem.hpp"
#include <algorithm>

static Entity* activeEnemy;

// Estructura para el cuadro de texto con el efecto de escritura
struct TextBox {
    std::vector<std::string> lines;
    int currentLineIndex = 0;
    int currentChar = 0;
    float timer = 0.0f;
    float baseDelay = 0.05f;
    int fontSize = 24;
    bool lineFinished = false;
    bool allFinished = false;

    // Comienza con las líneas y los parámetros correspondientes
    void Start(const std::vector<std::string>& newLines, float delay, int size) {
        lines = newLines;
        baseDelay = delay;
        fontSize = size;
        currentLineIndex = 0;
        currentChar = 0;
        timer = 0.0f;
        lineFinished = false;
        allFinished = false;
    }

    // Actualiza el cuadro de texto y dibuja las líneas poco a poco
    void Update(float delta) {
        if (allFinished) return;

        float speedMultiplier = (IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_LEFT_BUTTON)) ? 0.25f : 1.0f;
        timer += delta;

        float adjustedDelay = baseDelay * speedMultiplier;

        if (!lineFinished && timer >= adjustedDelay) {
            timer -= adjustedDelay;
            currentChar++;

            // Si la línea ha terminado, marcarla como terminada
            if (currentChar >= (int)lines[currentLineIndex].size()) {
                currentChar = lines[currentLineIndex].size();
                lineFinished = true;
            }
        }

        // Avanzar a la siguiente línea cuando el texto esté completo y se presione espacio o clic
        if (lineFinished && (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
            currentLineIndex++;
            if (currentLineIndex >= (int)lines.size()) {
                allFinished = true;
            } else {
                currentChar = 0;
                timer = 0.0f;
                lineFinished = false;
            }
        }
    }

    // Dibuja el cuadro de texto y el texto
    void Draw(Color boxColor, Color borderColor, Color textColor) {
        if (allFinished) return;

        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        int margin = 100;
        int padding = 20;

        std::string visibleLine = lines[currentLineIndex].substr(0, currentChar);

        // Tamaño del cuadro de texto (una sola línea)
        int boxHeight = fontSize + padding * 2;
        int boxY = screenHeight - boxHeight - 40;

        // Dibujar fondo y borde del cuadro de texto
        DrawRectangle(margin, boxY, screenWidth - margin * 2, boxHeight, boxColor);
        DrawRectangleLinesEx({(float)margin, (float)boxY, (float)(screenWidth - margin * 2), (float)boxHeight}, 4, borderColor);

        // Dibujar el texto
        DrawText(visibleLine.c_str(), margin + padding, boxY + padding, fontSize, textColor);

        // Dibujar el texto de indicación "Espacio o clic" dentro de la caja
        if (lineFinished) {
            const char* hint = "[Presiona espacio o clic]";
            int hintWidth = MeasureText(hint, fontSize - 6);
            int hintX = screenWidth - hintWidth - 150;
            int hintY = boxY + boxHeight - fontSize;
            DrawText(hint, hintX, hintY, fontSize - 6, Fade(RAYWHITE, 0.5f));
        }
    }

    bool Finished() const { return allFinished; }
};
struct Tutorial {
    enum Phase {
        INTRO = 0,
        BUTTON,
        AFTER_JOYA,
        MOVE_JOYA,
        ATTACK,
        DONE
    };
    int idJewelTuto{};
    Phase phase = INTRO;
    size_t paragraphIndex = 0;

    std::vector<std::string> paragraphs_intro = {
        "Bienvenido cazador. A partir de este momento, te adentrarás en una dimensión donde los demonios han tomado el poder.", 
        "Tu misión es combatir el mal hasta la muerte.",
        "Necesitarás un gran poder, y para ello tendrás que usar las joyas purificadas.",
        "Las puedes conseguir en el generador de joyas, el cual te permito usar por esta vez."
    };

    std::vector<std::string> paragraphs_after = {
        "Ajá. Esa será tu primera joya. Reluciente y fea.",
        "Esta joya te ayudará a conseguir grandes logros.",
        "Podrás usar este generador más adelante, cuando puedas pagarme su uso."
    };

    std::vector<std::string> paragraphs_attack = {
        "¡Cuidado, nos atacan!",
        "Clica en el enemigo para derrotarlo",
        "A partir de ahora te tocará descubrir y aprender por ti mismo. Ya eres mayorcito.",
        "Acaba con este mequetrefe"
    };

    TextBox textBox;
    bool readyForNext = false;
    bool joyaGenerada = false;
    bool joyaInPlace = false;

    Vector2 joyaCurrentPos = {0, 0};
    Vector2 joyaTargetPos = {100, 120};
    float joyaMoveTimer = 0.0f;

    Entity joyaEntity;

    void Init() {
        textBox.Start(paragraphs_intro, 0.05f, 24);
    }

    void generarJoya(Manentity_type& GE) {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        auto& joya = GE.createEntity_withCMPS<CmpRender, CmpPhysics, CmpInformation>();
        idJewelTuto = joya.getID();
        joya.addTag<TRenderizable>();
        joya.addTag<TJewel>();
        joya.addTag<TTutorial>();
        GE.defineCMP<CmpPhysics>(joya, CmpPhysics{(float)(screenWidth) / 2, (float)(screenHeight) / 2});
        GE.defineCMP<CmpRender>(joya, CmpRender{(float)(screenWidth) / 2, (float)(screenHeight) / 2});
        GE.defineCMP<CmpInformation>(joya, CmpInformation{5,0,0,0,0,"Attack +5",0,0});
        joyaEntity = Entity(joya);
    }

    float Lerp(float a, float b, float t) {
        return a + (b - a) * t;
    }

    void Update(float delta, Manentity_type& GE, RenderSystem& RS, int& lvl) {
        switch (phase) {
            case INTRO:
                textBox.Update(delta);
                if (textBox.Finished() && (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
                    paragraphIndex++;
                    if (paragraphIndex < paragraphs_intro.size()-3) {
                        textBox.Start({paragraphs_intro[paragraphIndex]}, 0.05f, 24);
                    } else {
                        phase = BUTTON;
                    }
                }
                break;

            case BUTTON: {
                if (!joyaGenerada && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mouse = GetMousePosition();
                    Rectangle btn = GetCenteredButtonRect(300, 80);
                    if (CheckCollisionPointRec(mouse, btn)) {
                        generarJoya(GE);
                        RS.update(GE, lvl);
                        joyaGenerada = true;
                        phase = AFTER_JOYA;
                        paragraphIndex = 0;
                        textBox.Start(paragraphs_after, 0.05f, 24);
                    }
                }
                break;
            }

            case AFTER_JOYA:
                textBox.Update(delta);
                if (textBox.Finished() && (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
                    paragraphIndex++;
                    if (paragraphIndex < paragraphs_after.size()-2) {
                        textBox.Start({paragraphs_after[paragraphIndex]}, 0.05f, 24);
                    } else {
                        phase = MOVE_JOYA;
                        joyaMoveTimer = 0.0f;
                    }
                }
                break;

            case MOVE_JOYA:
                if (!joyaInPlace) {
                    joyaMoveTimer += delta;
                    float t = std::clamp(joyaMoveTimer / 1.0f, 0.0f, 1.0f);
                    
                    // Aseguramos que la joya esté dentro de los límites del recuadro de la interfaz
                    float targetX = joyaTargetPos.x;
                    float targetY = joyaTargetPos.y;
                    
                    // Lerp ajustado para que la joya no se mueva fuera del recuadro
                    joyaCurrentPos.x = Lerp((float)GetScreenWidth() / 2, targetX, t);
                    joyaCurrentPos.y = Lerp((float)GetScreenHeight() / 2, targetY, t);

                    // Actualizar la posición de la joya en la física
                    auto& phys = GE.getEntityCMP<CmpPhysics>(joyaEntity);
                    phys.x = joyaCurrentPos.x;
                    phys.y = joyaCurrentPos.y;

                    if (t >= 1.0f) {
                        joyaInPlace = true;
                        paragraphIndex = 0;
                        phase = ATTACK;
                        textBox.Start(paragraphs_attack, 0.05f, 24);
                    }
                }
                
                break;

            case ATTACK:
                
                textBox.Update(delta);
                if (textBox.Finished() && (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
                    paragraphIndex++;
                    if (paragraphIndex < paragraphs_attack.size()-3) {
                        textBox.Start({paragraphs_attack[paragraphIndex]}, 0.05f, 24);
                    } else {
                        phase = DONE;
                        GE.getEntityByID(idJewelTuto).addTag<TActiveJewel>();
                        GE.getEntityByID(idJewelTuto).eraseTag<TTutorial>();
                    }
                }
                break;

            case DONE:
                lvl = 1;
                break;
        }
    }

    void Draw() {
        switch (phase) {
            case INTRO:
            case AFTER_JOYA:
            case MOVE_JOYA:
            case ATTACK:
                textBox.Draw(DARKBLUE, SKYBLUE, RAYWHITE);
                break;

            case BUTTON: {
                Rectangle btn = GetCenteredButtonRect(300, 80);
                Vector2 mouse = GetMousePosition();
                bool hovered = CheckCollisionPointRec(mouse, btn);
                DrawCofreButtonFancy(btn, hovered);
                break;
            }

            case DONE:
                //pasar a nivel siguiente
                break;
        }
    }

    Rectangle GetCenteredButtonRect(int width, int height) {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        return { (float)(screenWidth - width) / 2, (float)(screenHeight - height) / 2, (float)width, (float)height };
    }

    void DrawCofreButtonFancy(Rectangle btn, bool hovered) {
        // Colores base
        Color baseColor = Color{160, 82, 45, 255};
        Color lidColor  = Color{85, 53, 20, 255};
        Color lockColor = GOLD;
        Color shadowColor = Color{20, 20, 20, 100};
        Color auraBase = Color{0, 255, 255, 80}; // Cian mágico
        Vector2 offset = { 4, 4 }; // Para la sombra
        float t = GetTime();
        static float hoverTimer = 0.0f; // Temporizador para la animación de hover
    
        // ------ Animación de Hover (aparece gradualmente en 1 segundo) ------
        if (hovered) {
            hoverTimer += GetFrameTime(); // Incrementar el temporizador cada frame
            if (hoverTimer > 1.0f) hoverTimer = 1.0f; // Limitar a 1 segundo
        } else {
            hoverTimer -= GetFrameTime(); // Decrementar el temporizador cuando el hover desaparece
            if (hoverTimer < 0.0f) hoverTimer = 0.0f; // No permitir que sea menor que 0
        }
    
        // ------ Tamaño del Círculo (Aura) ------
        float fadeIn = hoverTimer;  // El valor de fade-in depende del tiempo de hover
        float auraRadius = (btn.width / 2) + 10 + fadeIn * 40;  // El radio del aura crece con el fade-in
        float shrinkAuraRadius = (btn.width / 2) + 10 + (1 - fadeIn) * 40; // Desaparición del aura
    
        // ------ Aura Mágica con Cambio de Tamaño ------
        if (hovered) {
            Vector2 auraCenter = { btn.x + btn.width / 2, btn.y + btn.height / 2 };
            DrawCircleV(auraCenter, auraRadius, Fade(auraBase, 0.4f * fadeIn)); // Aura suave con fade-in
        } else {
            Vector2 auraCenter = { btn.x + btn.width / 2, btn.y + btn.height / 2 };
            DrawCircleV(auraCenter, shrinkAuraRadius, Fade(auraBase, 0.4f * fadeIn)); // Aura desapareciendo gradualmente
        }
    
        // ------ CHISPAS MÁS AJUSTADAS AL COFRE ------
        if (hovered) {
            int sparkCount = 12;
            for (int i = 0; i < sparkCount; ++i) {
                // Posiciones de las chispas siguiendo los bordes del cofre
                float angle = 2 * PI * i / sparkCount;
                float radius = btn.width * 0.65f + sinf(t * 2 + i) * 5;
                Vector2 spark = {
                    btn.x + btn.width / 2 + cosf(angle) * radius,
                    btn.y + btn.height / 2 + sinf(angle) * radius + sinf(t * 3 + i) * 3
                };
                Color sparkColor = Fade(Color{0, 255, 255, 255}, 0.6f + 0.4f * sinf(t * 4 + i) * fadeIn);
                DrawCircleV(spark, 2, sparkColor);
            }
        }
    
        // ------ CUERPO Y SOMBRA DEL COFRE ------
        Rectangle body = {
            btn.x + offset.x,
            btn.y + offset.y,
            btn.width,
            btn.height
        };
        DrawRectangleRounded(body, 0.25f, 8, shadowColor);
        DrawRectangleRounded(btn, 0.25f, 8, baseColor);
    
        // ------ TAPA DEL COFRE ------
        Rectangle lid = { btn.x - 4, btn.y - btn.height * 0.4f, btn.width + 8, btn.height * 0.5f };
        DrawRectangleRounded(lid, 0.35f, 10, lidColor);
    
        // ------ CANDADO ------
        float lockWidth = btn.width * 0.15f;
        float lockHeight = btn.height * 0.3f;
        Rectangle lock = {
            btn.x + btn.width / 2 - lockWidth / 2,
            btn.y + btn.height / 2 - lockHeight / 2,
            lockWidth,
            lockHeight
        };
    
        // Sombra del candado
        Rectangle lockShadow = lock;
        lockShadow.x += 2;
        lockShadow.y += 2;
        DrawRectangleRounded(lockShadow, 0.3f, 6, shadowColor);
        DrawRectangleRounded(lock, 0.3f, 6, lockColor);
    
        // ------ Efecto de Hover Suave (escala gradual) ------
        if (hovered) {
            float scale = 1.0f + fadeIn * 0.1f; // Escala suavizada para el hover
            DrawRectangleRounded(btn, 0.25f, 8, baseColor);
        }
    }
};


struct Level{
    bool playingLvl{true};
    Level(){};
    Level(Manentity_type& GE){
        GenerarEnemigo(GE);
    }
    void GenerarEnemigo(Manentity_type& GE){
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        auto& enemy = GE.createEntity_withCMPS<CmpRender, CmpPhysics, CmpInformation>();
        enemy.addTag<TRenderizable>();enemy.addTag<TEnemy>();
        GE.defineCMP<CmpPhysics>(enemy, CmpPhysics{(float)(screenWidth) / 2, (float)(screenHeight) / 2});
        GE.defineCMP<CmpRender>(enemy, CmpRender{0,0,0});
        GE.defineCMP<CmpInformation>(enemy, CmpInformation{});
        activeEnemy = &enemy;
    }
    void LvlPlay(Manentity_type& GE, RenderSystem& RS, PhysicSystem& PS, InputSystem& IS, InformationSystem& InfS, int& actualLvl){
            InfS.update(GE,*activeEnemy, actualLvl);
            IS.update(GE, *activeEnemy);
            PS.update(GE);
            RS.update(GE,actualLvl);
    }
};