#ifndef H_CResourceManager_H
#define H_CResourceManager_H

#include "Pixy.h"
#include "sresource_manager.hpp"
#include "Puppet.h"
#include "Spell.h"
#include "Unit.h"
//#include "CDeck.h"

using std::string;
using std::vector;
using std::list;
using std::istringstream;
namespace Pixy {
namespace Net {

  //class Spell;
  //class Unit;
  class CDeck;
	class CResourceManager : public sresource_manager {
    public:
      //typedef std::list<Spell*> spells_t;
      //typedef std::list<Unit*> units_t;

      CResourceManager();
      virtual ~CResourceManager();

      list<Puppet*> puppetsFromStream(istringstream &stream);
      list<Puppet*>& getPuppets();

    protected:

      void parsePuppetsSection(istringstream &stream, int section, int nrEntries);
      void parsePuppetsStats(vector<string>& entries);

      void assignTalents(Puppet& inPuppet, string inTalents);
      void assignDeck(Puppet& inPuppet, string inName, string inSpells, int inUseCount);

      Puppet* getPuppet(string inName, list<Puppet*>& inPuppets);

      list<Puppet*> mPuppets;
  };
}
}
#endif
