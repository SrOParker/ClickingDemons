
#pragma once


#include "../cmp/tags.hpp"

#include "../cmp/CmpRender.hpp"
#include "../cmp/CmpPhysics.hpp"
#include "../cmp/CmpInformation.hpp"
#include "../entitymanager.hpp"


//ADD +1
//TAGS : 6
//CMPS : 3 
using TagList = 
Typelist<TPlayer
        , TEnemy
        , THealth
        , TRenderizable
        , TJewel
        , TActiveJewel
        >;
using ComponentList = 
Typelist<CmpRender
        , CmpPhysics
        , CmpInformation
        >;

using Manentity_type = ManEntity<ComponentList, TagList, 100>;
using Entity = Manentity_type::Entity;

