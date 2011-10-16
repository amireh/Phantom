/*
 * Copyright (C) Shroom Studios, Inc - All Rights Reserved
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Ahmad Amireh <ahmad@shroom-studios.com>, September 2011
 */

#include "Entity.h"
#include "BaseSpell.h"

namespace Pixy
{

  Entity::Entity()
  {
  }

  Entity::~Entity()
  {
  };


  Entity::Entity(const Entity& src)// : Caster(this)
  {
    copyFrom(src);
  }

  Entity& Entity::operator=(const Entity& rhs)
  {
    // check for self-assignment
    if (this == &rhs)
      return (*this);

    copyFrom(rhs);

    return (*this);
  }

  void Entity::copyFrom(const Entity& src)
  {
    BaseEntity::copyFrom(src);
    BaseCaster::copyFrom(src);
  }

  bool Entity::live()
  {
    fIsDead = false;
  }

  void Entity::die()
  {
    fIsDead = true;
  }

	void Entity::attachSpell(spell_t* inSpell)
	{
		BaseCaster::attachSpell(inSpell);
    mSpells.back()->setCaster(this);
	}

	void Entity::attachBuff(spell_t* inSpell)
	{
    BaseCaster::attachBuff(inSpell);
    mBuffs.back()->setTarget(this);
	}

} // end of Pixy namespace
