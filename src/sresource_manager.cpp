#include "sresource_manager.hpp"
#include "server.hpp"
#include "db_manager.hpp"

using std::vector;
using std::string;
namespace Pixy {
namespace Net {
	sresource_manager::sresource_manager() {
		mLog = new log4cpp::FixedContextCategory(PIXY_LOG_CATEGORY, "ResourceMgr");
		mLog->infoStream() << "up, dumping data to " << dump_path_;
	}

	sresource_manager::~sresource_manager() {
    //~ game_data_.Reset();
    mLog->infoStream() << "shutting down";

    for (int i=0; i < 4; ++i) {
      while (!mSpells[i].empty()) {
        // delete only the spells that aren't minion abilities
        //~ if (mSpells[i].back()->getCaster() == 0)
          delete mSpells[i].back();

        mSpells[i].pop_back();
      }

      while (!mUnits[i].empty()) {
        delete mUnits[i].back();
        mUnits[i].pop_back();
      }

      while (!mTalents[i].empty()) {
        delete mTalents[i].back();
        mTalents[i].pop_back();
      }
    }

		if (mLog) {
			delete mLog;
			mLog = NULL;
		}
	}


  void sresource_manager::populate(std::istringstream& lDump) {
    using std::string;

    //std::ifstream lDump(mDataPath.string().c_str());
    /*std::ifstream lDump("foobar");
    if (!lDump.is_open() || !lDump.good())
      return;*/

    try {
      string lLine;
      int lSection;
      std::vector<string> lElements;

      while (!lDump.eof()) {
        getline(lDump, lLine);
        //std::cout << lLine;

        if (lLine[0] == '[') {

          // find out which section it is
          if (lLine[1] == 's') { // spells
            lSection = 0;
          } else if (lLine[1] == 'm' && lLine[7] == 's') { // minions
            lSection = 1;
          } else if (lLine[1] == 't') { // talents
            lSection = 3;
          } else { // minion spells
            lSection = 2;
          }
          // how many entries it contains
          int nrEntries = atoi(Utility::split(lLine, ';').back().c_str());

          parseSection(lDump, lSection, nrEntries);
        }

        fflush(stdout);
      }

    } catch (std::exception& e) {

    } catch (...) {

    }

    //lDump.close();
  }

  void sresource_manager::parseSection(std::istringstream &stream, int section, int nrEntries) {
    using std::vector;
    using std::string;

    //~ std::cout << "-- parsing section " << section << " containing " << nrEntries << " entries\n";

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
      //~ std::cout << "parsed entry: " << tmp << "\n";
      lEntries.push_back(tmp);
    }

    switch(section) {
      case 0:
        parseSpells(lEntries);
      break;
      case 1:
        parseMinions(lEntries);
      break;
      case 2:
        parseMinionSpells(lEntries);
      break;
      case 3:
        parseTalents(lEntries);
      break;
    }
  }

  void sresource_manager::parseSpells(std::vector<std::string>& entries) {
    Spell* lSpell;
    vector<string>::iterator itr;
    for (itr = entries.begin(); itr != entries.end(); ++itr) {
      lSpell = new Spell();
      vector<string> elements = Utility::split((*itr).c_str(), ';');
      //~ std::cout << " elements size: " << elements.size() << ", registering: " << elements[0] << "\n";
      assert(elements.size() == 12);

      /* 0 name,
      * 1 race,
      * 2 duration,
      * 3 cost_wp,
      * 4 cost_hp,
      * 5 aspect,
      * 6 is_dispellable,
      * 7 requires_target,
      * 8 phase,
      * 9 cost_channels,
      * 10 requires_enemy_target,
      * 11 description */
      lSpell->setName(elements[0]);
      lSpell->setRace(atoi(elements[1].c_str()));
      lSpell->setDuration(atoi(elements[2].c_str()));
      lSpell->setCostWP(atoi(elements[3].c_str()));
      lSpell->setCostHP(atoi(elements[4].c_str()));
      lSpell->setAspect(elements[5] == "0" ? Spell::Matter : Spell::Mind);
      lSpell->setDispellable(elements[6] == "t");
      lSpell->setRequiresTarget(elements[7] == "t");
      lSpell->setPhase(atoi(elements[8].c_str()));
      lSpell->setCostChannels(convertTo<int>(elements[9]));
      lSpell->setRequiresEnemyTarget(elements[10] == "t");
      lSpell->setDescription(elements[11]);


      std::cout << "\tRegistered spell " << lSpell->getName()
        << ", cost: " << lSpell->getCostWP() << ":" << lSpell->getCostHP() << ":"
        << lSpell->getCostChannels() << "\n";

      mSpells[lSpell->getRace()].push_back(lSpell);

      lSpell = 0;
    }

    std::cout << "Registered " << entries.size() << " spells\n";

  }
  void sresource_manager::parseMinions(std::vector<std::string>& entries) {
    Unit* lUnit;
    vector<string>::iterator itr;
    for (itr = entries.begin(); itr != entries.end(); ++itr) {
      lUnit = new Unit();
      vector<string> elements = Utility::split((*itr).c_str(), ';');
      assert(elements.size() == 10);

      lUnit->_setName(elements[0]);
      lUnit->_setRace(atoi(elements[1].c_str()));
      //~ lUnit->setFaction(elements[2].c_str());
      lUnit->setBaseAP(atoi(elements[2].c_str()));
      lUnit->setAP(atoi(elements[2].c_str()));
      lUnit->setBaseHP(atoi(elements[3].c_str()));
      lUnit->setHP(atoi(elements[3].c_str()));
      //~ lUnit->setUpkeep(atoi(elements[4].c_str()));

      //~ lUnit->setIsTeamAttacker(elements[6] == "t");
      lUnit->setIsUnblockable(elements[4] == "t");
      lUnit->setIsRestless(elements[5] == "t");
      lUnit->setIsFlying(elements[6] == "t");
      lUnit->setIsTrampling(elements[7] == "t");
      lUnit->setHasFirstStrike(elements[8] == "t");
      lUnit->setHasLifetap(elements[9] == "t");

      //~ lUnit->setDescription(elements[10]);

      mUnits[lUnit->getRace()].push_back(lUnit);

      std::cout << "\tRegistered minion: " << lUnit->getName() << "\n";

      lUnit = 0;
    }

    std::cout << "Registered " << entries.size() << " minions\n";
  }
  void sresource_manager::parseMinionSpells(std::vector<std::string>& entries) {
   vector<string>::iterator itr;
    for (itr = entries.begin(); itr != entries.end(); ++itr) {
      vector<string> elements = Utility::split((*itr).c_str(), ';');
      assert(elements.size() == 2);

      string lUnitId = elements[0];
      string lSpellId = elements[1];

      Unit* lUnit = getModelUnit(lUnitId);
      Spell* lSpell = getSpell(lSpellId);

      assert (lUnit && lSpell);

      lUnit->attachSpell(lSpell);

      //std::cout << "Unit " << lUnit->getName() << " has an ability: " << lSpell->getName() << "\n";

      lUnit = 0;
      lSpell = 0;
    }

    std::cout << "Registered " << entries.size() << " minion spells\n";
  }

  void sresource_manager::parseTalents(std::vector<std::string>& entries) {
    //std::cout << "parsing talents: " << entries.back() << "\n";

    typedef std::map<Talent*, std::string> prereqs_t;
    prereqs_t mPrereqs;

    Talent* lTalent;
    vector<string>::iterator itr;
    for (itr = entries.begin(); itr != entries.end(); ++itr) {
      lTalent = new Talent();
      vector<string> elements = Utility::split((*itr).c_str(), ';');
      assert(elements.size() == 5);

      lTalent->setName(elements[0]);
      lTalent->setRace(atoi(elements[1].c_str()));
      lTalent->setTier(atoi(elements[2].c_str()));
      //lUnit->setPrereqs(atoi(elements[3].c_str()));
      lTalent->setDescription(elements[4]);

      mTalents[lTalent->getRace()].push_back(lTalent);

      if (elements[3] != "")
        mPrereqs.insert(std::make_pair(lTalent, elements[3]));

      std::cout << "\tRegistered talent: " << lTalent->getName() << "\n";

      lTalent = 0;
    }

    // process talent dependencies
    prereqs_t::const_iterator _itr;
    lTalent = 0;
    std::vector<std::string>::iterator talent_name;
    for (_itr = mPrereqs.begin(); _itr != mPrereqs.end(); ++_itr) {

      lTalent = _itr->first;
      std::string tmp(_itr->second);
      std::vector<std::string> names = Utility::split(tmp.erase(0,1).erase(tmp.size()-2,1), ',');// remove the { }

      for (talent_name = names.begin(); talent_name != names.end(); ++talent_name) {
        // strip out the enclosing " ", get the talent by its name, and add it as a prereq
        lTalent->addPrereq(
          getTalent(
            (*talent_name).erase(0,1).erase((*talent_name).size()-2,1),
            lTalent->getRace()));
      }
    }

    lTalent = 0;

    std::cout << "Registered " << entries.size() << " talents\n";
  }

  Spell* const sresource_manager::getSpell(std::string inName) {
    Spell* lSpell = 0;
    for (int i=0; i < 4; ++i) {
      lSpell = getSpell(inName, i);
      if (lSpell)
        return lSpell;
    }

    std::cerr << "couldnt find a spell named " << inName << "\n";
    return 0;
  }
  Spell* const sresource_manager::getSpell(std::string inName, char inRace) {
    spells_t::const_iterator itr;
    spells_t* lSpells = &mSpells[inRace];

    for (itr = (*lSpells).begin(); itr != (*lSpells).end(); ++itr)
      if ((*itr)->getName() == inName)
        return new Spell((**itr));

    return 0;
  }
  Unit* const  sresource_manager::getUnit(std::string inName) {
    Unit* lUnit = 0;
    for (int i=0; i < 4; ++i) {
      lUnit = getUnit(inName, i);
      if (lUnit)
        return lUnit;
    }

    return 0;
  }
  Unit* const  sresource_manager::getUnit(std::string inName, char inRace) {
    units_t::const_iterator itr;
    units_t* lUnits = &mUnits[inRace];

    for (itr = (*lUnits).begin(); itr != (*lUnits).end(); ++itr)
      if ((*itr)->getName() == inName)
        return new Unit((**itr));

    std::cerr << "couldnt find a unit named " << inName << "\n";
    return NULL;
  }


  Spell* sresource_manager::getModelSpell(std::string inName) {
    spells_t::const_iterator itr;

    for (int i=0; i < 4; ++i)
      for (itr = mSpells[i].begin(); itr != mSpells[i].end(); ++itr)
        if ((*itr)->getName() == inName)
          return (*itr);

    std::cerr << "couldnt find a spell named " << inName << "\n";
    return NULL;
  }

  Unit* sresource_manager::getModelUnit(std::string inName) {
    units_t::const_iterator itr;

    for (int i=0; i < 4; ++i)
      for (itr = mUnits[i].begin(); itr != mUnits[i].end(); ++itr)
        if ((*itr)->getName() == inName)
          return (*itr);

    std::cerr << "couldnt find a unit named " << inName << "\n";
    return NULL;
  }

  Talent const* const sresource_manager::getTalent(std::string inName, char inRace) {
    talents_t::const_iterator itr;
    for (itr = mTalents[inRace].begin(); itr != mTalents[inRace].end(); ++itr)
        if ((*itr)->getName() == inName)
          return (*itr);

    mLog->errorStream() << "couldnt find a talent named " << inName << "for race " << inRace;
    return 0;
  }

  void sresource_manager::dump(path in_path) {
    /*
     * 1) dump spells
     * 2) dump minions
     * 3) dump minion spells
     * 4) dump talents
     */

    dump_path_ = in_path;
    std::ofstream lDump(dump_path_.string().c_str());
    if (!lDump.is_open() || !lDump.good()) {
      mLog->errorStream() << "couldn't open resource file @ " << dump_path_.string();
      return;
    }
    try {

      std::ostringstream lConnInfo;
      lConnInfo
      << "hostaddr=" << DB_HOST
      << " port=" << DB_PORT
      << " dbname=" << DB_DBNAME
      << " user=" << DB_USER
      << " password=tuonela";

      pqxx::connection conn(lConnInfo.str());
      pqxx::work txn(conn);

      //lDump << "\\elementum\\\n";

      // spells
      pqxx::result lRecords = txn.exec(
        "select name, race, duration, cost_wp, cost_hp, aspect, \
        is_dispellable, requires_target, phase, cost_channels, \
        requires_enemy_target, description \
        from spells");
      pqxx::result::const_iterator lRecord;
      {
        lDump << "[spells];" << lRecords.size() << "\n";
        int nrFields = 12;
        std::string lFields[] =
        {"name", "race", "duration", "cost_wp", "cost_hp", "aspect",
         "is_dispellable", "requires_target", "phase", "cost_channels",
         "requires_enemy_target", "description"};
        for (lRecord = lRecords.begin(); lRecord != lRecords.end(); ++lRecord) {
          lDump << "$";
          for (int i=0; i < nrFields; ++i)
            lDump << (*lRecord)[lFields[i]] << ";";

          lDump << "\n";
        }
      }

      // minions
      lRecords = txn.exec(
        "select name,race, ap, hp, \
        is_unblockable, is_restless, is_flying, \
        is_trampling, has_first_strike, has_lifetap \
        from minions");
      {
        lDump << "[minions];" << lRecords.size() << "\n";
        int nrFields = 10;
        std::string lFields[] =
        {"name", "race", "ap", "hp", /*"upkeep",*/
         "is_unblockable", "is_restless", "is_flying",
         "is_trampling", "has_first_strike", "has_lifetap"/*,"description"*/};
        for (lRecord = lRecords.begin(); lRecord != lRecords.end(); ++lRecord) {
          lDump << "$";
          for (int i=0; i < nrFields; ++i)
            lDump << (*lRecord)[lFields[i]] << ";";

          lDump << "\n";
        }
      }

      // minion spells
      lRecords = txn.exec("SELECT * FROM minion_spells");
      {
        lDump << "[minion spells];" << lRecords.size() << "\n";
        int nrFields = 2;
        std::string lFields[] = {"minion", "spell"};
        for (lRecord = lRecords.begin(); lRecord != lRecords.end(); ++lRecord) {
          lDump << "$";
          for (int i=0; i < nrFields; ++i)
            lDump << (*lRecord)[lFields[i]] << ";";

          lDump << "\n";
        }
      }

      // talents
      lRecords = txn.exec("SELECT * FROM talents");
      {
        lDump << "[talents]; " << lRecords.size() << "\n";
        int nrFields = 5;
        std::string lFields[] = {"name", "race", "tier", "prereqs", "description"};
        for (lRecord = lRecords.begin(); lRecord != lRecords.end(); ++lRecord) {
          lDump << "$";
          for (int i=0; i < nrFields; ++i)
            lDump << (*lRecord)[lFields[i]] << ";";

          lDump << "\n";
        }
      }

      lDump << "^";
      lDump.close();

    } catch (std::exception& e) {

    }

    // compress the dump
    using std::string;
    using std::vector;

    // 1) load the raw data into buffer
    string sraw;
    {
      std::ifstream is;
      is.open (dump_path_.string().c_str(), ios::binary );
      is.seekg (0, ios::end);
      int length = is.tellg();
      is.seekg (0, ios::beg);
      char *buffer = new char [length];
      is.read (buffer,length);
      is.close();
      sraw = string(buffer, length);
      delete[] buffer;
      buffer = 0;
    }

    vector<unsigned char> vraw(sraw.begin(), sraw.end());
    vector<unsigned char> venc;

    int res = Archiver::encodeLzma(venc, vraw);
    /*if (res != 0) {
      mLog->errorStream() << "couldn't compress dump data, aborting";
      return;
    }*/

    string senc(venc.begin(), venc.end());
    int rawSize = sraw.size();

    venc.clear();
    vraw.clear();

    /*const char* src = dump_path_.string().c_str();
    std::string dest = path(dump_path_.string() + ".dat").string();

    std::cout << "compressing to " << dest << "\n";
    UInt64 srcSize, destSize;
    int res = Archiver::encodeLzma(src, dest.c_str(), &srcSize, &destSize);

    if (res != 0) {
      mLog->errorStream() << "couldn't compress dump data, aborting";
      return;
    }

    std::cout << "compressed file size= " << destSize << "\n";
    char* buf = (char*)malloc(sizeof(char) * destSize);
    FILE* fh = fopen(dest.c_str(), "r");
    if (!fh) {
      mLog->errorStream() << "couldn't open compressed file @ " << dest << " for reading, aborting";
      free(buf);
      return;
    }

    int bytesRead = fread(buf,1,destSize,fh);
    if (bytesRead != destSize) {
      mLog->errorStream() << "error reading compressed file into buffer, aborting";
      free(buf);
      return;
    }

    //buf[fileSize] = '\0';
    */

    std::string sum = MD5((unsigned char*)senc.c_str()).hex_digest();
    std::cout << "size of sum : " << sum.size() << "\n";
    std::cout << "size of raw data: " << rawSize << "\n";
    std::cout << "size of compressed data: " << senc.size() << "\n";
    std::cout << "sum: " << sum << "\n";

		//~ game_data_.Reset();
		//~ game_data_.AssertStreamEmpty();
    //~ game_data_.Write((unsigned char)ID_FETCH_GAME_DATA);
    //~ //game_data_.Write<int>(strlen(sum));
    //~ game_data_.Write(sum, strlen(sum));
    //~ game_data_.Write<int>(rawSize);
    //~ game_data_.Write(senc.c_str(), senc.size());
    game_data_ = "";
    //game_data_ += strlen(sum);
    //game_data_ += sum;
    //game_data_ += " ";
    //game_data_ += (int)rawSize;
    //game_data_ += " ";
    game_data_ += senc;

    raw_game_data_size_ = rawSize;

    std::cout << "game data stream size is: " << game_data_.size() << "b \n";

    //std::cout << game_data_;

    // populate our pool
    std::istringstream datastream(sraw);
    populate(datastream);
    datastream.clear();

    sraw.clear();

    mLog->infoStream() << "data dumped and compressed, stream ready for broadcasting";
  }

  std::string const& sresource_manager::get_game_data() {
    return game_data_;
  }

  int sresource_manager::get_raw_game_data_size() {
    return raw_game_data_size_;
  }

  void
  sresource_manager::puppets_to_stream(
    std::ostringstream& out,
    Puppet const* inPuppet,
    bool full)
  {
    std::list<const Puppet*> puppets;
    puppets.push_back(inPuppet);
    puppets_to_stream(out, puppets, full);
  }

  void
  sresource_manager::puppets_to_stream(
    std::ostringstream& out,
    const list<Puppet const*>& inPuppets,
    bool full)
  {
    std::ostringstream lDeckStream;
    int nrDecks = 0;

    list<Puppet const*>::const_iterator lPuppet;
    out << "[puppets];" << inPuppets.size() << "\n";
    for (lPuppet = inPuppets.begin(); lPuppet != inPuppets.end(); ++lPuppet)
    {
      // dump puppet to stream
      Puppet const& inPuppet = *(*lPuppet);
      out
        << "$" << inPuppet.getName() << ";"
        << inPuppet.getUID() << ";"
        << inPuppet.getRace() << ";"
        << inPuppet.getLevel() << ";"
        << inPuppet.getIntelligence() << ";"
        << inPuppet.getVitality() << ";";

      // providing a full dump? (talents + decks)
      if (full)
      {
        // fetch puppet talents, dump them to stream
        {
          const std::list<Talent const*> lTalents = inPuppet.getTalents();
          std::list<Talent const*>::const_iterator lTalent;
          std::string talent_names = "";
          for (lTalent = lTalents.begin(); lTalent != lTalents.end(); ++lTalent)
            talent_names += (*lTalent)->getName() + ",";
          if (lTalents.size() > 0)
            talent_names = talent_names.erase(talent_names.size()-1,1);
          out << talent_names << ";";
        } // talents

        {
          // dump puppet decks
          const Puppet::decks_t lDecks = inPuppet.getDecks();

          Puppet::decks_t::const_iterator lDeck;
          for (lDeck = lDecks.begin(); lDeck != lDecks.end(); ++lDeck) {
            lDeckStream
              << "$"
              << inPuppet.getName() << ";"
              << (*lDeck)->getName() << ";"
              << (*lDeck)->getUseCount() << ";";

            Deck::spells_t lSpells = (*lDeck)->_getSpells();
            Deck::spells_t::const_iterator lSpell;
            std::string spell_names = "";
            for (lSpell = lSpells.begin(); lSpell != lSpells.end(); ++lSpell)
              spell_names += (*lSpell)->getName() + ",";

            spell_names = spell_names.erase(spell_names.size()-1,1);
            lDeckStream << spell_names << ";\n";

            ++nrDecks;
          } // decks loop
        } // decks
      } // if full()
      out << "\n";
    } // puppet

    if (full) {
      out << "[decks];" << nrDecks << "\n";
      out << lDeckStream.str();
    }
    out << "^";
  }

  /*void sresource_manager::_assign_talents(Puppet& inPuppet, std::string inTalents) {
    //~ mLog->debugStream() << "assigning talents to puppet " << inPuppet.getName() << ": " << inTalents;

    inTalents = inTalents.erase(0,1).erase(inTalents.size()-2,1); // remove { }
    vector<string> names = Utility::split(inTalents,',');
    vector<string>::iterator name;

    //~ mLog->debugStream() << "\tfound " << names.size() << " talents";
    if (names.empty() || (names.size() == 1 && names.front() == "\"\""))
      return;
    for (name = names.begin(); name != names.end(); ++name) {
      inPuppet.attachTalent(getTalent((*name).erase(0,1).erase((*name).size()-2,1), inPuppet.getRace()));
    }

    //mLog->debugStream() << "puppet has " << inPuppet.getTalents().size() << " talents";
  }

  void sresource_manager::talentToStream(std::ostringstream& out, const Talent& inTalent) {
  }*/

  void sresource_manager::_assign_deck(Puppet& inPuppet, string inName, string inSpells, int inUseCount) {
    Deck* lDeck = 0;
    // does the puppet already have this deck? are we updating?
    inPuppet.detachDeck(inName);

    lDeck = new Deck(&inPuppet);
    lDeck->setName(inName);
    lDeck->setUseCount(inUseCount);

    // parse the spells
    // expected format: {"Spell1 Name","...","Spell16 Name"}
    inSpells = inSpells.erase(0,1).erase(inSpells.size()-2,1); // strip out the {}
    vector<string> elements = Utility::split(inSpells, ',');
    assert(elements.size() == 16);
    vector<string>::iterator itr;
    for (itr = elements.begin(); itr != elements.end(); ++itr) {
      //~ std::cout << "assigning spell to deck " << (*itr) << "\n";
      // strip out the quotes
      std::string spellname = (*itr).erase(0,1).erase((*itr).size()-2,1);
      //(*itr).pop_front();
      lDeck->_assignSpell(getSpell(spellname));
    }

    inPuppet.attachDeck(lDeck);
    lDeck = 0;
  }
}
}
