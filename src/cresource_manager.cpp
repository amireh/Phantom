#include "cresource_manager.hpp"
#include "Puppet.h"
#include "Spell.h"
#include "Unit.h"

using std::vector;
using std::string;
namespace Pixy {
namespace Net {
	CResourceManager::CResourceManager() {
    mLog->infoStream() << "up";
	}

	CResourceManager::~CResourceManager() {
	}

/*
 * Format is:
 * [puppets];nrPuppets\n
 * $puppet1-name;race;level;intelligence;vitality;talent1,talent2,...,talentN;\n
 * ...
 * $puppetN\n
 * [decks];nrDecks\n
 * $puppet-name;deck-name;use-count;spell1,spell2,...,spell16;\n
 * ...
 * $deckN\n
 * /elementum/
 */
  std::list<Puppet*> CResourceManager::puppetsFromStream(std::istringstream& inStream) {
    using std::string;
    using std::vector;

    try {
      string lLine;
      int lSection;
      vector<string> lElements;
      mPuppets.clear();

      while (!inStream.eof()) {
        getline(inStream, lLine);
        //std::cout << lLine;

        if (lLine[0] == '[') {

          // find out which section it is
          if (lLine[1] == 'p') { // puppets
            lSection = 0;
          } else if (lLine[1] == 'd') { // decks
            lSection = 1;
          }
          // how many entries it contains
          int nrEntries = atoi(Utility::split(lLine, ';').back().c_str());

          parsePuppetsSection(inStream, lSection, nrEntries);
        }

        fflush(stdout);
      }

    } catch (std::exception& e) {

    } catch (...) {

    }

    return mPuppets;
  }


  void CResourceManager::parsePuppetsSection(std::istringstream &stream, int section, int nrEntries) {
    using std::vector;
    using std::string;

    //std::cout << "-- parsing section " << section << " containing " << nrEntries << " entries\n";

    vector<string> lEntries;

    char c;
    string tmp;
    for (int i=0; i < nrEntries; ++i) {
      //std::cout << "\n+ processing entry << " << i << ", stream @ " << stream.tellg() << " +\n";

      string tmp;
      bool fdf = false; // first delimiter found (the $)
      bool done = false;
      while (true) {
        c = (char)stream.get();
        switch (c) {
          case '$': // start/end of entry
            if (fdf) { // we're done
              done = true;
            } else {
              fdf = true;
              continue;
            }
            break;
          case '\n': // EOL
          case '/': // EOF
          case '[': // end of section
            done = true;
            break;
        }

        if (done)
          break;

        tmp.push_back(c);
      }
      //std::cout << "parsed entry: " << tmp << "\n";
      lEntries.push_back(tmp);
    }

    switch(section) {
      case 0:
        parsePuppetsStats(lEntries);
      break;
      case 1:
        //parsePuppetsDecks(lEntries);
      break;
    }
  }

  void CResourceManager::parsePuppetsStats(std::vector<std::string>& entries) {
    Puppet* lPuppet = 0;
    vector<string>::iterator itr;
    for (itr = entries.begin(); itr != entries.end(); ++itr) {
      lPuppet = new Puppet();
      vector<string> elements = Utility::split((*itr).c_str(), ';');
      assert(elements.size() == 7);

      lPuppet->setName(elements[0]);
      lPuppet->setUID(atoi(elements[1].c_str()));
      lPuppet->setRace((RACE)atoi(elements[2].c_str()));
      lPuppet->setLevel(atoi(elements[3].c_str()));
      lPuppet->setIntelligence(atoi(elements[4].c_str()));
      lPuppet->setVitality(atoi(elements[5].c_str()));
      this->assignTalents(*lPuppet, elements[6]);

      mPuppets.push_back(lPuppet);
    }
  }

  void CResourceManager::assignTalents(Puppet& inPuppet, std::string inTalents) {
    vector<string> names = Utility::split(inTalents,',');
    vector<string>::iterator name;
    for (name = names.begin(); name != names.end(); ++name) {
      std::cout << "assigning talent named " << *name << " to puppet " << inPuppet.getName() << "\n";
      inPuppet.addTalent(getTalent(*name, inPuppet.getRace()));
    }

    //mLog->debugStream() << "puppet has " << inPuppet.getTalents().size() << " talents";
  }

  void CResourceManager::parseSpells(std::vector<std::string>& entries) {
    Spell* lSpell;
    vector<string>::iterator itr;
    for (itr = entries.begin(); itr != entries.end(); ++itr) {
      lSpell = new Spell();
      vector<string> elements = Utility::split((*itr).c_str(), ';');
      assert(elements.size() == 10);

      lSpell->setName(elements[0]);
      lSpell->setRace((RACE)atoi(elements[1].c_str()));
      lSpell->setDuration(atoi(elements[2].c_str()));
      lSpell->setCostWP(atoi(elements[3].c_str()));
      lSpell->setCostHP(atoi(elements[4].c_str()));
      lSpell->setAspect(elements[5] == "0" ? MATTER : MIND);
      lSpell->setDispellable(elements[6] == "t");
      lSpell->setRequiresTarget(elements[7] == "t");
      lSpell->setPhase((SPELL_PHASE)atoi(elements[8].c_str()));
      lSpell->setDescription(elements[9]);

      mSpells[lSpell->getRace()].push_back(lSpell);

      std::cout << "\tRegistered spell: " << lSpell->getName() << "\n";

      lSpell = 0;
    }

    std::cout << "Registered " << entries.size() << " spells\n";

  }
  void CResourceManager::parseMinions(std::vector<std::string>& entries) {
    Unit* lUnit;
    vector<string>::iterator itr;
    for (itr = entries.begin(); itr != entries.end(); ++itr) {
      lUnit = new Unit();
      vector<string> elements = Utility::split((*itr).c_str(), ';');
      assert(elements.size() == 12);

      lUnit->setName(elements[0]);
      lUnit->setRace((RACE)atoi(elements[1].c_str()));
      lUnit->setFaction(elements[2].c_str());
      lUnit->setBaseAP(atoi(elements[3].c_str()));
      lUnit->setAP(atoi(elements[3].c_str()));
      lUnit->setBaseHP(atoi(elements[4].c_str()));
      lUnit->setHP(atoi(elements[4].c_str()));
      lUnit->setUpkeep(atoi(elements[5].c_str()));

      lUnit->setIsTeamAttacker(elements[6] == "t");
      lUnit->setIsUnblockable(elements[7] == "t");
      lUnit->setIsRestless(elements[8] == "t");
      lUnit->setIsFlying(elements[9] == "t");
      lUnit->setIsTrampling(elements[10] == "t");

      lUnit->setDescription(elements[11]);

      mUnits[lUnit->getRace()].push_back(lUnit);

      std::cout << "\tRegistered minion: " << lUnit->getName() << "\n";

      lUnit = 0;
    }

    std::cout << "Registered " << entries.size() << " minions\n";
  }
  void CResourceManager::parseMinionSpells(std::vector<std::string>& entries) {
   vector<string>::iterator itr;
    for (itr = entries.begin(); itr != entries.end(); ++itr) {
      vector<string> elements = Utility::split((*itr).c_str(), ';');
      assert(elements.size() == 2);

      string lUnitId = elements[0];
      string lSpellId = elements[1];

      Unit* lUnit = getModelUnit(lUnitId);
      Spell* lSpell = getSpell(lSpellId);

      lUnit->attachSpell(lSpell);

      //std::cout << "Unit " << lUnit->getName() << " has an ability: " << lSpell->getName() << "\n";

      lUnit = 0;
      lSpell = 0;
    }

    std::cout << "Registered " << entries.size() << " minion spells\n";
  }

  Puppet* CResourceManager::getPuppet(string inName, list<Puppet*>& inPuppets) {
    list<Puppet*>::const_iterator itr;
    for (itr = inPuppets.begin(); itr != inPuppets.end(); ++itr)
      if ((*itr)->getName() == inName)
        return (*itr);

    return 0;
  }

  list<Puppet*>& CResourceManager::getPuppets() {
    return mPuppets;
  }

  Spell* const CResourceManager::getSpell(std::string inName) {
    Spell* lSpell = 0;
    for (int i=0; i < WATER; ++i) {
      lSpell = getSpell(inName, (RACE)i);
      if (lSpell)
        return lSpell;
    }

    mLog->errorStream() << "couldnt find a spell named " << inName;
    return 0;
  }
  Spell* const CResourceManager::getSpell(std::string inName, Pixy::RACE inRace) {
    spells_t::const_iterator itr;
    spells_t* lSpells = &mSpells[inRace];

    for (itr = (*lSpells).begin(); itr != (*lSpells).end(); ++itr)
      if ((*itr)->getName() == inName)
        return new Spell((**itr));

    return NULL;
  }


  Spell* CResourceManager::getModelSpell(std::string inName) {
    spells_t::const_iterator itr;

    for (int i=0; i < 4; ++i)
      for (itr = mSpells[i].begin(); itr != mSpells[i].end(); ++itr)
        if ((*itr)->getName() == inName)
          return (*itr);

    mLog->errorStream() << "couldnt find a spell named " << inName;
    return NULL;
  }

  Unit* CResourceManager::getModelUnit(std::string inName) {
    units_t::const_iterator itr;

    for (int i=0; i < 4; ++i)
      for (itr = mUnits[i].begin(); itr != mUnits[i].end(); ++itr)
        if ((*itr)->getName() == inName)
          return (*itr);

    mLog->errorStream() << "couldnt find a unit named " << inName;
    return NULL;
  }
} // namespace Net
} // namespace Pixy
