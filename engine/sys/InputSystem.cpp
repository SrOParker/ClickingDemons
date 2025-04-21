#include "InputSystem.hpp"

InputSystem::InputSystem(){}

void InputSystem::update(Manentity_type& EM, Entity& activeEnemy){
    auto update_one_entity = [&](Entity& e, CmpInformation& cinfo)
    { 
        auto& enemyinfo = EM.getEntityCMP<CmpInformation>(activeEnemy);
        auto& enemyPos  = EM.getEntityCMP<CmpPhysics>(activeEnemy);

        HitAnEnemy(cinfo, enemyinfo, enemyPos);
    };
    EM.foreach<SYSCMPs, SYSTAGs>(update_one_entity);
}

void InputSystem::HitAnEnemy(CmpInformation& cinfoPlayer, CmpInformation& cinfoEnemy, const CmpPhysics enemyPos) {
    // === Zona clicable ===
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    Rectangle centralZone = {
        screenWidth / 2 - 200,
        screenHeight / 2 - 200,
        400,
        400
    };
    DrawRectangleLinesEx(centralZone, 2, RED);

    // === Clic dentro de la zona ===
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, centralZone)) {
            size_t damage = cinfoPlayer.powerAttack;

            // Aplicar daño
            cinfoEnemy.health = (cinfoEnemy.health > damage) ? (cinfoEnemy.health - damage) : 0;

            // Preparar animación del daño
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_real_distribution<float> distX(-120.0f, 120.0f);  // Más amplio
            std::uniform_real_distribution<float> distY(-150.0f, -60.0f);  // Más alto

            damageAnimations.push_back({
                Vector2{enemyPos.x, enemyPos.y},
                { distX(rng), distY(rng) },
                0.0f,
                0.6f,  // Más rápida
                (int)damage,
                true
            });
        }
    }

    // === Dibujar todas las animaciones activas ===
    for (auto& anim : damageAnimations) {
        if (anim.active) {
            anim.time += GetFrameTime();
            if (anim.time < anim.lifespan) {
                float t = anim.time;
                float progress = t / anim.lifespan;

                Vector2 animPos = {
                    anim.startPos.x + anim.velocity.x * progress,
                    anim.startPos.y + anim.velocity.y * progress + (30 * sinf(progress * PI))
                };

                DrawText(TextFormat("%d", anim.damageAmount), animPos.x, animPos.y, 25, RED);
            } else {
                anim.active = false;
            }
        }
    }

    // Eliminar animaciones ya terminadas (opcional, limpieza)
    damageAnimations.erase(
        std::remove_if(damageAnimations.begin(), damageAnimations.end(), [](const DamageAnimation& a) {
            return !a.active;
        }),
        damageAnimations.end()
    );
}
