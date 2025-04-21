#include "InformationSystem.hpp"

InformationSystem::InformationSystem(){}

void InformationSystem::update(Manentity_type& EM){
    auto update_one_entity = [&](Entity& e, CmpInformation& cinfo)
    { 
        if (e.hasTag<TPlayer>()){
            //Sumamos las diferentes joyas
        }
        if (e.hasTag<TEnemy>()){
            //Si la vida es 0, eliminamos la entidad y generamos una nueva.
        }
    };
    EM.foreach<SYSCMPs, SYSTAGs>(update_one_entity);
    
}


//AQUI FALTA :
//  - Recorrer joyas activas y sumarlas a las diferentes categorias, Pensar que las joyas tienen que tener cinfo tmb y no todas dan ataque.
//  - Controlar la muerte de los enemigos y su respawn
//OTROS CAMBIOS FUERA DE ESTE ARCHIVO:
//  - Mostrar barra de vida del enemigo
//  - Intentar hacer que el enemigo al recibir daño parpadé
//  - Sistema de monedas
//  - Sistema para conseguir nuevas joyas
//  - Biblioteca para ver todas las joyas que tenemos. (Posibilidad de mejorarlas)