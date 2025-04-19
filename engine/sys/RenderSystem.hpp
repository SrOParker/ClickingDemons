/*************************************************
**************** RENDER SYSTEM *******************
**************************************************/

#pragma once

#include "../utils/types.hpp"

struct RenderSystem {
    public:
    using SYSCMPs = Typelist<CmpRender, CmpPhysics>; 
	using SYSTAGs = Typelist<>;
    RenderSystem();
    void update(Manentity_type& EM);
    void DrawShape(CmpRender& rend); 
    void DrawDemonShape(const std::vector<Vector2>& vertices, Vector2 position, Color color, const std::vector<Vector2>& eyes, const std::vector<Vector2>& mouth);
    private:
    void UpdateDemonShapeWithReturn(std::vector<Vector2>& vertices, const std::vector<Vector2>& originalVertices, float deltaTime) ;
};
