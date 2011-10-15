#include "sresource_manager.hpp"
#include "server.hpp"
#include "db_manager.hpp"

using std::vector;
using std::string;
namespace Pixy {
namespace Net {
	sresource_manager::sresource_manager() {
    dump_path_ = path(server::singleton().get_data_path()) / "current";
		mLog->infoStream() << "up, dumping data to " << dump_path_;
	}

	sresource_manager::~sresource_manager() {
    //~ game_data_.Reset();
	}

  void sresource_manager::dump() {
    /*
     * 1) dump spells
     * 2) dump minions
     * 3) dump minion spells
     * 4) dump talents
     */

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
        "select name,race, ap, hp, upkeep, \
        is_unblockable, is_restless, is_flying, \
        is_trampling, has_first_strike, has_lifetap, description \
        from minions");
      {
        lDump << "[minions];" << lRecords.size() << "\n";
        int nrFields = 12;
        std::string lFields[] =
        {"name", "race", "ap", "hp", "upkeep",
         "is_unblockable", "is_restless", "is_flying",
         "is_trampling", "has_first_strike", "has_lifetap", "description"};
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
          const std::list<Deck const*> lDecks = inPuppet.getDecks();

          std::list<Deck const*>::const_iterator lDeck;
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

  void sresource_manager::_assign_talents(Puppet& inPuppet, std::string inTalents) {
    //~ mLog->debugStream() << "assigning talents to puppet " << inPuppet.getName() << ": " << inTalents;

    inTalents = inTalents.erase(0,1).erase(inTalents.size()-2,1); // remove { }
    vector<string> names = Utility::split(inTalents,',');
    vector<string>::iterator name;

    //~ mLog->debugStream() << "\tfound " << names.size() << " talents";
    if (names.empty() || (names.size() == 1 && names.front() == "\"\""))
      return;
    for (name = names.begin(); name != names.end(); ++name) {
      inPuppet.addTalent(getTalent((*name).erase(0,1).erase((*name).size()-2,1), inPuppet.getRace()));
    }

    //mLog->debugStream() << "puppet has " << inPuppet.getTalents().size() << " talents";
  }

  void sresource_manager::talentToStream(std::ostringstream& out, const Talent& inTalent) {
  }

  void sresource_manager::_assign_deck(Puppet& inPuppet, string inName, string inSpells, int inUseCount) {
    Deck* lDeck = 0;
    // does the puppet already have this deck? are we updating?
    inPuppet.removeDeck(inName);

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

    inPuppet.addDeck(lDeck);
    lDeck = 0;
  }
}
}
