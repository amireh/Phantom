#include "bresource_manager.hpp"
#include "Puppet.h"
#include "Spell.h"
#include "Unit.h"

using std::vector;
using std::string;
namespace Pixy {
namespace Net {
	CResourceManager::CResourceManager() {
    //mLog->infoStream() << "up";
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
          case '^': // EOF
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

      lPuppet->_setName(elements[0]);
      lPuppet->_setUID(atoi(elements[1].c_str()));
      lPuppet->_setRace(atoi(elements[2].c_str()));
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
      inPuppet.attachTalent(getTalent(*name, inPuppet.getRace()));
    }

    //mLog->debugStream() << "puppet has " << inPuppet.getTalents().size() << " talents";
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

} // namespace Net
} // namespace Pixy