#pragma once
#include <iostream>
#include <random>

static int nextAmountOfHealth = -980;  // 100 ENTIDADES SE INICIALIZAN 1000 va a ser el primer valor, restar este valor hace empezar en 25
struct CmpInformation {
    std::size_t powerAttack;
    std::size_t criticalStrikeChance;
    std::size_t health;
    std::size_t maxhealth;
    std::size_t money;
    std::string description;
    std::size_t magicArmor;
    std::size_t basicArmor;
    CmpInformation();
    CmpInformation( std::size_t powerAttack, 
                    std::size_t criticalStrikeChance, 
                    std::size_t health, 
                    std::size_t maxhealth, 
                    std::size_t money, 
                    std::string description,
                    std::size_t magicArmor,
                    std::size_t basicArmor);
};