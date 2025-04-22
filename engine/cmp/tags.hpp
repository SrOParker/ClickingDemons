#pragma once
////////////////// TAG LIST
//
//


// PLAYER TAG --> PLAYER ACTIONS, PLAYER DIFFERENTIATION
struct TPlayer{};

// ENEMY TAG  --> ENEMY ACTIONS, ENEMY DIFFERENTIATION
struct TEnemy{};

// HEALTH TAG --> NOT MUCH USE, ENTITY HEALTH YES OR NOT
struct THealth{};

// TAG PARA SABER SI DIBUJAR O NO UN OBJETO
struct TRenderizable{};

// TAG PARA SABER SI ES UNA JOYA
struct TJewel{};
struct TActiveJewel{};

// TAG PARA RENDERIZAR EN EL CENTRO LA JOYA DEL TUTORIAL
struct TTutorial {};