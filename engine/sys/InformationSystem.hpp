#pragma once

#include "../utils/types.hpp"

struct InformationSystem {
    public:
    using SYSCMPs = Typelist<CmpInformation>; 
	using SYSTAGs = Typelist<>;
    InformationSystem();
    void update(Manentity_type& EM);

    private:

};
