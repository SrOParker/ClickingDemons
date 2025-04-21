#include "PhysicSystem.hpp"

PhysicSystem::PhysicSystem(){}

static int nextSlot {0};
void PhysicSystem::update(Manentity_type& EM){
    auto update_one_entity = [&](Entity& e, CmpRender& crend, CmpPhysics& phy)
    { 
        ChangePosition(e, crend, phy);
    };
    EM.foreach<SYSCMPs, SYSTAGs>(update_one_entity);
    nextSlot = 0;
}

void PhysicSystem::ChangePosition(Entity& e, CmpRender& rend, CmpPhysics& phy){
    if(e.hasTag<TActiveJewel>()){
        Vector2 joyaTargetPos = {100, 120};
        phy.x = joyaTargetPos.x;
        phy.y= joyaTargetPos.y + nextSlot * 100;
        nextSlot++;
    }
    
    rend.sprite.position.x = phy.x;
    rend.sprite.position.y = phy.y;
    rend.spriteE.position.x = phy.x;
    rend.spriteE.position.y = phy.y;
}