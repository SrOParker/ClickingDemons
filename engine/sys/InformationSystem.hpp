#pragma once

#include "../utils/types.hpp"

struct InformationSystem {
    public:
    using SYSCMPs = Typelist<CmpInformation>; 
	using SYSTAGs = Typelist<>;
    InformationSystem();
    void update(Manentity_type& EM, Entity& activeEnemy, int& actualLvl);

    private:
    void deleteEnemy(Manentity_type& EM, Entity& e);
    void createEnemy(Manentity_type& EM, Entity& activeEnemy);
};
