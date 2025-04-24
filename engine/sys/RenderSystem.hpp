/*************************************************
**************** RENDER SYSTEM *******************
**************************************************/

#pragma once

#include "../utils/types.hpp"

struct Star {
    Vector2 pos;
    float radius;
    float speed;
    float alpha;
};

struct RenderSystem {
    public:
    using SYSCMPs = Typelist<CmpRender, CmpPhysics, CmpInformation>;
	using SYSTAGs = Typelist<>;
    RenderSystem();
    void update(Manentity_type& EM, int& actualLvl);
    void DrawShape(CmpRender& rend); 
    void DrawDemonShape(const std::vector<Vector2>& vertices, Vector2 position, Color color, const std::vector<Vector2>& eyes, const std::vector<Vector2>& mouth);
    private:
    void UpdateDemonShapeWithReturn(std::vector<Vector2>& vertices, const std::vector<Vector2>& originalVertices, float deltaTime) ;
    void PaintGameInterface();
    void DrawEnemyHealthBar(int currentHealth, int maxHealth, int actualLvl);

    //Game Background
    std::vector<Star> stars;
    float pulseTime = 0.0f;
    const int NUM_STARS = 100;

    void InitDynamicBackground();
    void DrawDynamicBackground();
};
