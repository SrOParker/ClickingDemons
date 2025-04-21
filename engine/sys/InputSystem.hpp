#pragma once

#include "../utils/types.hpp"

struct DamageAnimation {
    Vector2 startPos;
    Vector2 velocity;
    float time;
    float lifespan;
    int damageAmount;
    bool active;
};

struct InputSystem {
    public:
    using SYSCMPs = Typelist<CmpInformation>; 
	using SYSTAGs = Typelist<TPlayer>;
    InputSystem();
    void update(Manentity_type& EM, Entity& activeEnemy);

    private:
    void HitAnEnemy(CmpInformation& cinfoPlayer, CmpInformation& cinfoEnemy, const CmpPhysics enemyPos);
    std::vector<DamageAnimation> damageAnimations;
};
