#include "CmpInformation.hpp"

CmpInformation::CmpInformation() {
    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_int_distribution<int> distHealth(50, 300);
    std::uniform_int_distribution<int> distArmor(0, 100);
    std::uniform_int_distribution<int> distMoney(1, 5);
    std::uniform_real_distribution<float> distCrit(0.0f, 1.0f);
    std::uniform_int_distribution<int> distPower(5, 50);

    this->health = distHealth(rng);
    this->basicArmor = distArmor(rng);
    this->magicArmor = distArmor(rng);
    this->money = distMoney(rng);
    this->criticalStrikeChance = distCrit(rng);
    this->powerAttack = distPower(rng);
    
    // Si quieres una descripción aleatoria simple:
    this->description = "Enemigo básico";
}

CmpInformation::CmpInformation( std::size_t powerAttack, 
    std::size_t criticalStrikeChance, 
    std::size_t health, 
    std::size_t money, 
    std::string description,
    std::size_t magicArmor,
    std::size_t basicArmor){
    this->basicArmor = basicArmor;
    this->criticalStrikeChance = criticalStrikeChance;
    this->description = description;
    this->health = health;
    this->magicArmor= magicArmor;
    this->money = money;
    this->powerAttack = powerAttack;
    }