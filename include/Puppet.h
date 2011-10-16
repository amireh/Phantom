/*
 * Copyright (C) Shroom Studios, Inc - All Rights Reserved
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Ahmad Amireh <ahmad@shroom-studios.com>, September 2011
 */

#ifndef H_PhantomPuppet_H
#define H_PhantomPuppet_H

#include "Entity.h"
#include "BasePuppet.h"
//~ #include "Unit.h"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace Pixy
{
  /*! \class Puppet Puppet.h "src/Puppet.h"
   *  \brief Represents Combat Puppet GameObject
   */
  //~ class Unit;
	class Puppet : public Entity, public BasePuppet, public boost::enable_shared_from_this<Puppet>
	{
  public:
    typedef Unit unit_t;
    typedef std::list<unit_t*> units_t;

		Puppet();
		virtual ~Puppet();

		virtual void attachUnit(unit_t* inUnit);
		virtual void detachUnit(int inUID, bool inRemove = true);
		virtual unit_t* getUnit(int inUID);
		virtual units_t const& getUnits();

	protected:

    std::list<Unit*> mUnits;

  private:
    // Puppets are NOT copiable
    Puppet(const Puppet& src);
		Puppet& operator=(const Puppet& rhs);
	};

  typedef boost::shared_ptr<Puppet> puppet_ptr;
  typedef boost::shared_ptr<const Puppet> puppet_cptr;
} // end of namespace
#endif
