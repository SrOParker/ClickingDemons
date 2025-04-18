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
    private:

};
