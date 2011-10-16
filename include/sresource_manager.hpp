#ifndef H_sresource_manager_H
#define H_sresource_manager_H

#include <vector>
#include <list>
#include <string>
#include <map>

#include "Pixy.h"
//~ #include "ResourceManager.h"
#include "Spell.h"
#include "Deck.h"
#include "Puppet.h"
#include "Unit.h"
#include "Talent.h"
#include "PixyUtility.h"

//#include "db_manager.hpp"

//~ #include "BitStream.h"
//~ #include "RakString.h"

#include "Archiver.h"
#include "lzma/Types.h"
#include "md5/md5.hh"

#include <boost/filesystem.hpp>

/*
 * TODO: parse Talents (done)
 */

//~ using RakNet::BitStream;
//~ using RakNet::RakString;
using boost::filesystem::path;
using std::string;
namespace Pixy {
namespace Net {

  class db_manager;
	class sresource_manager {// : public ResourceManager {
    public:
    typedef Spell spell_t;
    typedef Unit unit_t;

    typedef std::list<spell_t*> spells_t;
    typedef std::list<unit_t*> units_t;
    typedef std::list<Talent*> talents_t;

    sresource_manager();
    virtual ~sresource_manager();

    void dump(path);
    std::string const& get_game_data();
    int get_raw_game_data_size();

    //bool deletePuppet(Player* inPlayer, RakString inName);

    //Puppet* get_puppet(Player*, std::string);

    //std::vector<Puppet*> get_puppets(Player* inPlayer);

    void puppets_to_stream(std::ostringstream& out, const list<Puppet const*>& inPuppets, bool full=true);
    void puppets_to_stream(std::ostringstream& out, Puppet const* inPuppet, bool full=true);

    void populate(std::istringstream& lDump);

    spell_t* const getSpell(std::string inName);
    spell_t* const getSpell(std::string inName, char inRace);
    unit_t* const getUnit(std::string inName);
    unit_t* const getUnit(std::string inName, char inRace);
    Talent const* const getTalent(std::string inName, char inRace);

    protected:
    friend class db_manager;

    void _assign_talents(Puppet& inPuppet, string inTalents);
    void _assign_deck(Puppet& inPuppet, string inName, string inSpells, int inUseCount);

    spell_t* getModelSpell(std::string inName);
    unit_t* getModelUnit(std::string inName);

    void parseSection(std::istringstream &stream, int section, int nrEntries);
    void parseSpells(std::vector<std::string>& entries);
    void parseMinions(std::vector<std::string>& entries);
    void parseMinionSpells(std::vector<std::string>& entries);
    void parseTalents(std::vector<std::string>& entries);

    void talentToStream(std::ostringstream& out, const Talent& inTalent);
    //void puppetToStream(std::ostringstream& out, Puppet const& inPuppet);

    log4cpp::Category	*mLog;

    spells_t mSpells[4];
    units_t mUnits[4];
    talents_t mTalents[4];

    std::string game_data_;
    int raw_game_data_size_;
    path dump_path_;

    //DBManager* mDBMgr;

    //pqxx::connection* mDBC;


  };
}
}
#endif
