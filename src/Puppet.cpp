/*
 * Copyright (C) Shroom Studios, Inc - All Rights Reserved
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Ahmad Amireh <ahmad@shroom-studios.com>, September 2011
 */

#include "Puppet.h"
#include "Unit.h"
#include "PixyUtility.h"

namespace Pixy
{

  Puppet::Puppet()
  {
    setRank(Rank::Puppet);
  }

  Puppet::~Puppet()
  {
    while (!mUnits.empty())
    {
      this->detachUnit(mUnits.front()->getUID(), true);
    }
  };


/* ------------------------
	 * UNITS
	 * ------------------------ */
  void Puppet::attachUnit(unit_t* inUnit) {
    mUnits.push_back(inUnit);
    inUnit->_setOwner(this);

    //std::cout
    //  << mUID << " attached a unit to my control: "
    //  << inUnit->getName() << "#" << inUnit->getUID() << "\n";
  }
  void Puppet::detachUnit(int inUID, bool remove) {
    unit_t* lUnit = 0;
		units_t::iterator it;
		for(it = mUnits.begin(); it != mUnits.end(); ++it)
			if ((*it)->getUID() == inUID)
			{
        lUnit = *it;
				mUnits.erase(it);
				break;
			}

    if (remove)
      delete lUnit;
  }
  Puppet::unit_t* Puppet::getUnit(int inUID) {
    units_t::iterator lUnit;
		for (lUnit = mUnits.begin(); lUnit != mUnits.end(); ++lUnit)
			if ((*lUnit)->getUID() == inUID)
				return *lUnit;

    throw invalid_uid("couldn't find a unit with uid" + stringify(inUID));
  }
  Puppet::units_t const& Puppet::getUnits() {
    return mUnits;
  }
} // end of Pixy namespace
