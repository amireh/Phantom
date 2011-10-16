/*
 * Copyright (C) Shroom Studios, Inc - All Rights Reserved
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Ahmad Amireh <ahmad@shroom-studios.com>, September 2011
 */

#include "Unit.h"

namespace Pixy
{
  Unit::Unit()
  {
    setRank(Rank::Unit);
  }

  Unit::~Unit()
  {
  };

  Unit::Unit(const Unit& src)
  {
    Unit::copyFrom(src);
  }

  Unit& Unit::operator=(const Unit& rhs)
  {
    // check for self-assignment
    if (this == &rhs)
      return (*this);

    Unit::copyFrom(rhs);
    return (*this);
  }

  void Unit::copyFrom(const Unit& src)
  {
    BaseEntity::copyFrom(src);
    BaseCaster::copyFrom(src);
    BaseUnit::copyFrom(src);
  }

  bool Unit::attack(Puppet *inTarget)
  {
  }

  bool Unit::attack(Unit* inTarget, bool doBlock)
  {
  }

  void Unit::serialize(Event& inEvt) {
    BaseEntity::serialize(inEvt);
    BaseUnit::serialize(inEvt);
  }
  void Unit::deserialize(const Event& inEvt) {
    BaseEntity::deserialize(inEvt);
    BaseUnit::deserialize(inEvt);
  }
} // end of Pixy namespace
