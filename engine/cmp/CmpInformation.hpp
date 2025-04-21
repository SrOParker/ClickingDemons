#pragma once
#include <iostream>
#include <random>

struct CmpInformation {
    std::size_t powerAttack;
    std::size_t criticalStrikeChance;
    std::size_t health;
    std::size_t money;
    std::string description;
    std::size_t magicArmor;
    std::size_t basicArmor;
    CmpInformation();
    CmpInformation( std::size_t powerAttack, 
                    std::size_t criticalStrikeChance, 
                    std::size_t health, 
                    std::size_t money, 
                    std::string description,
                    std::size_t magicArmor,
                    std::size_t basicArmor);
};