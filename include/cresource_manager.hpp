#ifndef H_CResourceManager_H
#define H_CResourceManager_H

#include "Pixy.h"
#include "ResourceManager.h"
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
	class CResourceManager : public ResourceManager {
    public:
      typedef std::list<Spell*> spells_t;
      typedef std::list<Unit*> units_t;

      CResourceManager();
      virtual ~CResourceManager();

      virtual Spell* const getSpell(std::string inName);
      virtual Spell* const getSpell(std::string inName, Pixy::RACE inRace);
      list<Puppet*> puppetsFromStream(istringstream &stream);
      list<Puppet*>& getPuppets();

    protected:

      virtual Spell* getModelSpell(std::string inName);
      virtual Unit* getModelUnit(std::string inName);

      void parsePuppetsSection(istringstream &stream, int section, int nrEntries);
      void parsePuppetsStats(vector<string>& entries);
      virtual void parseSpells(std::vector<std::string>& entries);
      virtual void parseMinions(std::vector<std::string>& entries);
      virtual void parseMinionSpells(std::vector<std::string>& entries);

      void assignTalents(Puppet& inPuppet, string inTalents);
      void assignDeck(Puppet& inPuppet, string inName, string inSpells, int inUseCount);

      Puppet* getPuppet(string inName, list<Puppet*>& inPuppets);

      spells_t mSpells[4];
      units_t mUnits[4];

      list<Puppet*> mPuppets;
  };
}
}
#endif
