/*
 *  db_manager.h
 *  Elementum
 *
 *  Created by Ahmad Amireh on 2/20/10.
 *  Copyright 2010 JU. All rights reserved.
 *
 */

#ifndef H_db_manager_H
#define H_db_manager_H

#include <iostream>
#include <boost/thread.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "pqxx/connection.hxx"
#include "pqxx/transactor.hxx"
//#include "md5/md5.hh"
#include "Puppet.h"
#include "server.hpp"
#include "player.hpp"
#include "PixyUtility.h"

#define	DB_HOST		"127.0.0.1"
#define	DB_PORT		"5432"
#define	DB_DBNAME	"elementum"
#define	DB_USER		"elementum"

namespace Pixy {
namespace Net {


  enum class db_result : unsigned char {
    Unassigned = 0,

    Ok, // the operation was succesful

    // to check for an error generally: (db_result > db_result::Ok)
    InvalidCredentials,
    AlreadyLoggedIn,
    InvalidName,
    InvalidOwner,
    PuppetTaken,
    PuppetNotFound,

    Error
  };

  /*!
   * @brief
   * Provides access to the Postgres database. All operations are thread-safe
   * and asynchronous. The result of every operation is passed to the provided
   * callback handler.
   *
   * @warning do not create more than ONE instance of this class in a
   * multi-threaded environment, as the database connection objects themselves
   * are inherently un-safe
   */
	class db_manager {
	public:
		db_manager(boost::asio::io_service&);
		~db_manager();

    bool is_connected() const;

		/*! \brief
		 *	Validates user account credentials for connecting to the game and
     *  flags account as online in the DB.
     *
		 *	@param
		 *		inUsername	unique identifier chosen by the player upon registration
		 *	@param
		 *		inPassword MD5-encrypted password token
		 *
		 *	@return
		 *		true	if user record exists and password matches
		 */
    template <typename Callback>
		void login(std::string inUsername, std::string inPassword, Callback callback) {

      void (db_manager::*f)(std::string,std::string, boost::tuple<Callback>)
        = &db_manager::do_login<Callback>;

      strand_.post( boost::bind(f, this, inUsername, inPassword, boost::make_tuple(callback)) );
    }

		/*! \brief
		 *	Flags account with inUsername as offline in the DB.
		 */
		void logout(std::string inUsername) {
      strand_.post( boost::bind(&db_manager::do_logout, this, inUsername) );
    }

		/*! \brief
		 *	Loads a puppet into inPuppet.
     * @return
     *  false if no such puppet exists, true otherwise
		 */
    template <typename Callback>
		bool load_puppet(std::string inName, Puppet& inPuppet, Callback callback) {
      void (db_manager::*f)(std::string, Puppet&, boost::tuple<Callback>, bool)
        = &db_manager::do_load_puppet<Callback>;

      strand_.post( boost::bind(f, this, inName, boost::ref(inPuppet), boost::make_tuple(callback), true) );
    }

    template <typename Callback>
    void sync_puppets(std::string inUsername, std::list<Puppet*>* inPuppets, Callback callback)
    {
      void (db_manager::*f)(std::string,std::list<Puppet*>*, boost::tuple<Callback>)
        = &db_manager::do_sync_puppets<Callback>;

      strand_.post( boost::bind(f, this, inUsername, inPuppets, boost::make_tuple(callback)) );
    }

    template <typename Callback>
    void create_puppet(std::string inUsername, const Puppet* inPuppet, Callback callback)
    {
      void (db_manager::*f)(std::string, const Puppet*, boost::tuple<Callback>)
        = &db_manager::do_create_puppet<Callback>;

      strand_.post( boost::bind(f, this, inUsername, inPuppet, boost::make_tuple(callback)) );
    }

    template <typename Callback>
    void remove_puppet(std::string const& inUsername, const Puppet* inPuppet, Callback callback)
    {
      void (db_manager::*f)(std::string const&, const Puppet*, boost::tuple<Callback>)
        = &db_manager::do_remove_puppet<Callback>;

      strand_.post( boost::bind(f, this, inUsername, inPuppet, boost::make_tuple(callback)) );
    }

    template <typename Callback>
    void create_template_deck(Puppet* inPuppet, std::string inOriginalDeck, Callback callback)
    {
      void (db_manager::*f)(Puppet*, std::string, boost::tuple<Callback>)
        = &db_manager::do_create_template_deck<Callback>;

      strand_.post( boost::bind(f, this, inPuppet, inOriginalDeck, boost::make_tuple(callback)) );
    }

    template <typename Callback>
    void create_or_update_deck(Puppet* inPuppet, std::string const& inDeckName, std::string inSpells, Callback callback)
    {
      void (db_manager::*f)(Puppet*, std::string const&, std::string, boost::tuple<Callback>)
        = &db_manager::do_create_or_update_deck<Callback>;

      strand_.post( boost::bind(f, this, inPuppet, inDeckName, inSpells, boost::make_tuple(callback)) );
    }

    template <typename Callback>
    void remove_deck(std::string const& inPuppet, std::string const& inDeck, Callback callback)
    {
      void (db_manager::*f)(std::string const&,std::string const&, boost::tuple<Callback>)
        = &db_manager::do_remove_deck<Callback>;

      strand_.post( boost::bind(f, this, inPuppet, inDeck, boost::make_tuple(callback)) );
    }

#if 0 // __DISABLED__
		/*! \brief
		 *	Creates or updates if exists a user Profile.
		 *
		 *	Subjects calling this method must make sure
		 *	the passed Puppet object has all the required,
		 *	and validated data. This method provides little,
		 *	if any, input sanitizing.
		 *
		 *	@param
		 *		inUsername: owner of this profile
		 *	@param
		 *		inProfile: a valid Puppet object, must have the
		 *		following attributes set:
		 *			# name
		 *			# race
		 *			# intelligence
		 *			# vitality
		 *			# alignment
		 *			# deck (container of Spell UIDs)
		 *			# talents (container of Talent UIDs)
		 *
		 *	@return
		 *		true if the profile was properly saved or updated
		 *
		 */
		bool create_puppet(Player const* inPlayer, Puppet const& inProfile);

    bool updatePuppet(Player const* inPlayer, Puppet const& inPuppet);

		/*! \brief
		 *	Removes an existing Puppet belong to inPlayer, if found.
		 */
		bool deletePuppet(const Player& inPlayer, const char* inPuppet);

		/*! \brief
		 *	Used in intro state, retrieves all Puppets registered	to the player.
		 */
		void loadAllProfiles(const Player& inPlayer,
							 std::vector<Puppet*>& inContainer);

#endif // __DISABLED__

	protected:

    /// helpers for retrieving user id's
		int get_userid(const char* inUsername);
		std::string get_username(const int inUserId);

    // used by login()
    template <typename Callback>
		void do_login(std::string inUsername, std::string inMD5Password, boost::tuple<Callback> callback) {
      db_result dbr = db_result::Unassigned;

      //pqxx::result result_;
      //~ std::cout << "Username: " << inUsername << "\n";
      //~ std::cout << "Password: " << inMD5Password << "\n";
      std::ostringstream _condition;
      _condition << "username='" << inUsername << "'";
      _condition << " AND password='" << inMD5Password << "'";

      //~ std::cout << "Condition: " << _condition.str() << "\n";

      result_.clear();
      bool success = false;
      if ( get_records(result_, "accounts", "COUNT(username)", _condition.str()) )
        success = convertTo<int>(result_[0]["count"].c_str()) == 1;

      if (success) {
        //_condition.str("");
        //_condition << "username='" << inUsername << "'";
        update_record("accounts", "is_online", _condition.str().c_str(), "true");
        update_record("accounts", "last_login_at", _condition.str().c_str(), "now()");

        dbr = db_result::Ok;
      } else
        dbr = db_result::InvalidCredentials;

      boost::get<0>(callback)(dbr, inUsername);
    }

    void do_logout(std::string inUsername) {
      std::ostringstream _condition;
      _condition << "username='" << inUsername << "'";
      update_record("accounts", "is_online", _condition.str().c_str(), "false");
    }

    template <typename Callback>
    void do_load_puppet(std::string inProfileName, Puppet& inPuppet, boost::tuple<Callback> callback, bool full=true)
    {
      //pqxx::result result_;
      std::ostringstream _condition, _fields;
      _condition << "puppets.name=" << escape(inProfileName);
      _fields << "name, race, level, intelligence, vitality";
      if (full)
        _fields << ", talents";

      get_records(result_, "puppets", _fields.str().c_str(), _condition.str().c_str());

      if (result_.empty())
        throw std::runtime_error("ERROR! Profile does not exist in database for loading!");

      inPuppet.setName(result_[0]["name"].c_str());
      inPuppet.setRace((RACE)convertTo<int>(result_[0]["race"].c_str()));
      inPuppet.setLevel(convertTo<int>(result_[0]["level"].c_str()));
      inPuppet.setIntelligence(convertTo<int>(result_[0]["intelligence"].c_str()));
      inPuppet.setVitality(convertTo<int>(result_[0]["vitality"].c_str()));

      if (full)
      {
        server::singleton().get_resmgr()._assign_talents(inPuppet, result_[0]["talents"].c_str());

        // get the decks
        result_.clear();
        _condition.str("");
        _condition << "decks.puppet=" << escape(inPuppet.getName());
        get_records(result_, "decks", "name,spells,use_count", _condition.str().c_str());
        pqxx::result::const_iterator lDeck;
        for (lDeck = result_.begin(); lDeck != result_.end(); ++lDeck) {
          server::singleton().get_resmgr()._assign_deck(
            inPuppet,
            (*lDeck)["name"].c_str(),
            (*lDeck)["spells"].c_str(),
            convertTo<int>((*lDeck)["use_count"].c_str()));
        }
      }

      boost::get<0>(callback)(db_result::Ok);
    }

    template <typename Callback>
    void do_sync_puppets(std::string inUsername, std::list<Puppet*>* inPuppets, boost::tuple<Callback> callback)
    {
      std::ostringstream _condition;
      _condition << "puppets.account=" << escape(inUsername);

      pqxx::result my_result;
      get_records(my_result, "puppets", "name", _condition.str().c_str());

      if (my_result.size() == 0)
      {
        return boost::get<0>(callback)(db_result::Error);
      }

      std::cout<<"\tfound " << my_result.size() << " puppets\n";
      pqxx::result::const_iterator record;
      for (record = my_result.begin(); record != my_result.end(); ++record)
      {
        Puppet* _tmp = new Puppet();

        do_load_puppet(
          (*record)["name"].c_str(),
          *_tmp,
          boost::make_tuple([&, _tmp](db_result ec) -> void {
            std::cout << "\tdone loading puppet " << _tmp->getName() << "\n";
          }),
          false /* don't do a full load */);

        inPuppets->push_back(_tmp);
        _tmp = 0;
      }

      std::cout << "\t done loading puppets for player " << inUsername << "\n";
      boost::get<0>(callback)(db_result::Ok);
    }

    template <typename Callback>
    void do_create_puppet(std::string inUsername, const Puppet* inPuppet, boost::tuple<Callback> callback)
    {
      std::ostringstream _fields, _values;
      _fields << "account, name, race, intelligence, vitality, talents";
      _values
        << escape(inUsername) << ", "
        << escape(inPuppet->getName()) << ", "
        << inPuppet->getRace() << ", "
        << inPuppet->getIntelligence()	<< ", "
        << inPuppet->getVitality() << ", "
        << escape("{\"\"}");

      // create profile
      bool result = create_record("puppets", _fields.str().c_str(), _values.str().c_str());
      boost::get<0>(callback)(result ? db_result::Ok : db_result::Error);
    }

    template <typename Callback>
    void do_remove_puppet(std::string const& inUsername, const Puppet* inPuppet, boost::tuple<Callback> callback)
    {
      std::ostringstream _condition;
      _condition << "name=" << escape(inPuppet->getName()) << " AND account=" << escape(inUsername);
      bool result = delete_records("puppets", _condition.str().c_str());

      if (!result)
      {
        return boost::get<0>(callback)(db_result::Error);
      }

      _condition.str("");
      _condition << "puppet=" << escape(inPuppet->getName());
      result = delete_records("decks", _condition.str().c_str());

      boost::get<0>(callback)(result ? db_result::Ok : db_result::Error);
    }

    template <typename Callback>
    void do_create_template_deck(Puppet* inPuppet, std::string inOriginalDeck, boost::tuple<Callback> callback)
    {
      std::ostringstream _condition;
      _condition << "decks.name=" << escape(inOriginalDeck);
      bool success = get_records(result_, "decks", "spells", _condition.str().c_str());

      assert(success && !result_.empty());

      server::singleton().get_resmgr()._assign_deck(
        *inPuppet,
        inOriginalDeck,
        result_[0]["spells"].c_str(),
        0);

      // create the new deck entry
      {
        std::ostringstream _fields, _values;
        _fields << "puppet, name, use_count, spells";
        _values
          << escape(inPuppet->getName()) << ", "
          << escape(inOriginalDeck) << ", "
          << 0 << ", "
          << escape(result_[0]["spells"].c_str());

        success = create_record("decks", _fields.str().c_str(), _values.str().c_str());
      }

      boost::get<0>(callback)(success ? db_result::Ok : db_result::Error);
    }

    template <typename Callback>
    void do_create_or_update_deck(
      Puppet* inPuppet,
      std::string const& inDeck,
      std::string inSpells,
      boost::tuple<Callback> callback)
    {
      pqxx::result result;

      std::ostringstream _condition;
      _condition << "decks.name=" << escape(inDeck) << " AND decks.puppet=" << escape(inPuppet->getName());
      bool success = get_records(result, "decks", "name, use_count", _condition.str().c_str());
      bool updating = !result.empty();

      if (success && !updating)
      {
        // create the deck
        std::ostringstream _fields, _values;
        _fields << "puppet, name, use_count, spells";
        _values
          << escape(inPuppet->getName()) << ", "
          << escape(inDeck) << ", "
          << 0 << ", "
          << escape(inSpells);

        bool success_ = create_record("decks", _fields.str().c_str(), _values.str().c_str());
        if (success_)
          // update the puppet object's deck
          server::singleton().get_resmgr().
            _assign_deck( *inPuppet, inDeck, inSpells, 0);

        return boost::get<0>(callback)(success_ ? db_result::Ok : db_result::Error);
      } else if (success && updating) {
        // update the deck
        bool success_ = update_record("decks", "spells", _condition.str().c_str(), escape(inSpells).c_str());

        if (success_)
          // update the puppet object's deck
          server::singleton().get_resmgr().
            _assign_deck( *inPuppet,  inDeck,  inSpells, convertTo<int>(result[0]["use_count"].c_str()) );

        return boost::get<0>(callback)(success ? db_result::Ok : db_result::Error);
      }

      // something went wrong
      return boost::get<0>(callback)(db_result::Error);
    }

    template <typename Callback>
    void do_remove_deck(std::string const& inPuppet, std::string const& inDeck, boost::tuple<Callback> callback)
    {
      std::ostringstream _condition;
      _condition << "name=" << escape(inDeck) << " AND puppet=" << escape(inPuppet);
      bool result = delete_records("decks", _condition.str().c_str());

      boost::get<0>(callback)(result ? db_result::Ok : db_result::Error);
    }
		/*! Retrieves a set of record(s) according to the input
		 *
		 *	@param
		 *		inOP => Operation to be performed, check P_ID_SQL_OP enum for values
		 *	@param
		 *		inTable => Name of table on which the query will be performed
		 *	@param
		 *		inFields =>
		 *			Comma-delimited string of field names specifying the domain of the transaction.
		 *			Defaults to	'*', retrieves all records.
		 *			e.g "`username`" | "`username`,`password`" | "`table.username`"
		 *	@param
		 *		inCondition =>
		 *			Properly escaped string of a WHERE condition, with the 'WHERE'
		 *			clause stripped. e.g "username='foobar'"
		 *	@param
		 *		inOrder =>
		 *			Field name followed by a hyphen character and an ordering
		 *			specifier. eg "username-ASC" | "wins-DESC"
		 *
		 *	@return
		 *		pqxx::result container with a result::size() of nr of tuples retrieved.
		 */
		bool
		get_records(pqxx::result& out,
      std::string inTable,
			std::string inFields="*",
			std::string inCondition="",
			std::string inOrder="");

		/*! Inserts a record into inTable
		 *
		 *	@return
		 *		true if the insertion was successful, false if not
		 */
		bool
		create_record(const char* inTable,
					 const char* inFields,
					 const char* inValues);

		/*! Deletes record(s) that match the criteria inCondition
		 *
		 *	\note
		 *	If one of the deleted record's fields are FKeys and used
		 *	by other records in another table, or the PK is, these
		 *	matching records will also be deleted.
		 *
		 *	@return
		 *		true if 1 or more records were deleted
		 *		false if no records were deleted
		 */
		bool
		delete_records(const char* inTable,
					  const char* inCondition);

		/*! Modifies the value of a field for a matching record to a new value
		 *
		 *	@param
		 *		inNewValue => updating value for the tuple's matching field value
		 *
		 *	@return
		 *		true if a record was found and updated
		 *		false if no records were found matching the criteria
		 */
		bool
		update_record(const char* inTable,
					 const char* inField,
					 const char* inCondition,
					 const char* inNewValue);

		/*! \brief
		 *	Shows line status of a user.
		 */
		bool is_logged(const char* inUsername);

		pqxx::connection *conn_; /// our db connection handle
		//pqxx::work *mWorker; /// performs transactions

		/* since 1 transaction can be performed at any
		 * single time per instance, one result object alone
     * could be used for holding results
     */
		pqxx::result result_;

		/// helper for firing up the transactor and  executing the sql queries
		bool exec_query(const char* inQuery, pqxx::result* outResult);

		/// helper for escaping strings for use in queries
		std::string escape(std::string inString);



		void runTests();

		log4cpp::FixedContextCategory *log_;

    private:
    friend class server;

    boost::asio::strand strand_;

		/*! \brief
		 *	Initiates client connection to configured PostgreSQL server.
		 *
		 *	Connectivity provided by this method allows only read access.
		 *	This is used by players for retrieving profile information.
		 *
		 *	\note
		 *	If a connection is already open and alive, no action will be
		 *	taken here.
		 *
		 *	@return
		 *		true	on connection success, exceptions will be thrown on failure
		 */
		bool connect();

		/*! \brief
		 *	Initiates a privileged connection to the backend database server.
		 *
		 *	This is used by Pixy::Server for modifying player profiles,
		 *	updating match rankings, and registration.
		 */
		bool connect(const char* inDBUser, const char* inDBPassword);

		/*! \brief
		 *	Terminates active connection, if any.
		 */
		void disconnect();

	private:
		db_manager(const db_manager& src) = delete;
		db_manager& operator=(const db_manager& rhs) = delete;
	};
}
}
#endif
