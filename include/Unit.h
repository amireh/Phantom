/*
 * Copyright (C) Shroom Studios, Inc - All Rights Reserved
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Ahmad Amireh <ahmad@shroom-studios.com>, September 2011
 */

#ifndef H_PhantomUnit_H
#define H_PhantomUnit_H

#include "Entity.h"
#include "BaseUnit.h"

namespace Pixy
{
  /*! \class Unit "Unit.h" "include/Unit.h"
  *
  *  \brief
  *  Represents Unit GameObjects, their actions and attributes.
  *
  */
  struct Event;
  class Puppet;
  class Unit : public Entity, public BaseUnit
  {
    public:

    Unit();
    Unit(const Unit& src);
    Unit& operator=(const Unit& rhs);
    virtual ~Unit();

    virtual bool attack(Puppet* inTarget);
    virtual bool attack(Unit* inTarget, bool doBlock = false);

    virtual void serialize(Event& inEvt);
    virtual void deserialize(const Event& inEvt);

    protected:

    void copyFrom(const Unit& src);

  }; // end of class Unit
} // end of Pixy namespace

#endif // P_Unit_H
