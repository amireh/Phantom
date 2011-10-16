/*
 * Copyright (C) Shroom Studios, Inc - All Rights Reserved
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Ahmad Amireh <ahmad@shroom-studios.com>, September 2011
 */

#ifndef H_PhantomEntity_H
#define H_PhantomEntity_H

#include "BaseEntity.h"
#include "BaseCaster.h"

namespace Pixy
{
  class Entity : public BaseEntity, public BaseCaster
  {
    public:

    Entity();
    Entity(const Entity& src);
    Entity& operator=(const Entity& rhs);
    virtual ~Entity();

    virtual bool live();
		virtual void die();

		virtual void attachSpell(spell_t* inSpell);
		virtual void attachBuff(spell_t* inSpell);

    protected:

    void copyFrom(const Entity& src);

  }; // end of class Entity
} // end of Pixy namespace

#endif // P_Entity_H
