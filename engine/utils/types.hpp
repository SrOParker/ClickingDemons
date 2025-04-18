
#pragma once


#include "../cmp/tags.hpp"

#include "../cmp/CmpRender.hpp"
#include "../cmp/CmpPhysics.hpp"

#include "../entitymanager.hpp"


//ADD +1
//TAGS : 4
//CMPS : 2 
using TagList = 
Typelist<TPlayer
        , TEnemy
        , THealth
        , TRenderizable
        >;
using ComponentList = 
Typelist<CmpRender
        , CmpPhysics
        >;

using Manentity_type = ManEntity<ComponentList, TagList, 100>;
using Entity = Manentity_type::Entity;

