#pragma once

#include "../utils/types.hpp"

struct PhysicSystem {
    public:
    using SYSCMPs = Typelist<CmpRender, CmpPhysics>; 
	using SYSTAGs = Typelist<>;
    PhysicSystem();
    void update(Manentity_type& EM);

    private:
    void ChangePosition(Entity& e, CmpRender& rend, CmpPhysics& phy); 
};
