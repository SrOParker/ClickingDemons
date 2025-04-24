#include "InformationSystem.hpp"

InformationSystem::InformationSystem(){}

std::vector<Entity> getActiveJewels(Manentity_type& EM){
    std::vector<Entity> activeJewels{};
    for (Entity& e : EM.getEntityArray()){
        if (e.hasTag<TActiveJewel>()){
            activeJewels.push_back(e);
        }
    }
    return activeJewels;
}

void InformationSystem::update(Manentity_type& EM, Entity& activeEnemy, int& actualLvl){
    auto update_one_entity = [&](Entity& e, CmpInformation& cinfo)
    { 
        if (e.hasTag<TPlayer>()){
            //Sumamos las diferentes joyas
            std::vector<Entity> jewels = getActiveJewels(EM);
            std::size_t damage=0;
            
            if (!jewels.empty() && jewels.size()>0){
                for (Entity& j : jewels){
                    damage += EM.getEntityCMP<CmpInformation>(j).powerAttack;
                }
            }
            cinfo.powerAttack = 1 + damage;
        }
        if (e.hasTag<TEnemy>()){
            //Si la vida es 0, eliminamos la entidad y generamos una nueva.
            if (cinfo.health <= 0){
                deleteEnemy(EM,e);
                createEnemy(EM, activeEnemy);
                actualLvl +=1;
            }
        }
    };
    EM.foreach<SYSCMPs, SYSTAGs>(update_one_entity);
    
}

void InformationSystem::deleteEnemy(Manentity_type& EM, Entity& e){
    EM.eraseEntity<CmpInformation, CmpRender, CmpPhysics>(e);
}

void InformationSystem::createEnemy(Manentity_type& GE, Entity& activeEnemy){
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    auto& enemy = GE.createEntity_withCMPS<CmpRender, CmpPhysics, CmpInformation>();
    enemy.addTag<TRenderizable>();enemy.addTag<TEnemy>();
    GE.defineCMP<CmpPhysics>(enemy, CmpPhysics{(float)(screenWidth) / 2, (float)(screenHeight) / 2});
    GE.defineCMP<CmpRender>(enemy, CmpRender{0,0,0});
    GE.defineCMP<CmpInformation>(enemy, CmpInformation{});
    activeEnemy = enemy;
}


//AQUI FALTA :
//  - Intentar hacer que el enemigo al recibir daño parpadé
//  - Sistema de monedas
//  - Sistema para conseguir nuevas joyas
//  - Biblioteca para ver todas las joyas que tenemos. (Posibilidad de mejorarlas)
//  - Joyas diferentes, no solo ataque