#include "PhysicSystem.hpp"

PhysicSystem::PhysicSystem(){}

void PhysicSystem::update(Manentity_type& EM){
    auto update_one_entity = [&](Entity& e, CmpRender& crend, CmpPhysics& phy)
    { 
        ChangePosition(crend, phy);
    };
    EM.foreach<SYSCMPs, SYSTAGs>(update_one_entity);
    
}

void PhysicSystem::ChangePosition(CmpRender& rend, CmpPhysics& phy){
    rend.sprite.position.x = phy.x;
    rend.sprite.position.y = phy.y;
    rend.spriteE.position.x = phy.x;
    rend.spriteE.position.y = phy.y;
}