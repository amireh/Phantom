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
    PuppetNotFound
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
      void (db_manager::*f)(std::string, Puppet&, boost::tuple<Callback>)
        = &db_manager::do_load_puppet<Callback>;

      strand_.post( boost::bind(f, this, inName, boost::ref(inPuppet), boost::make_tuple(callback)) );
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
		bool createPuppet(Player const* inPlayer, Puppet const& inProfile);

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
      std::cout << "Username: " << inUsername << "\n";
      std::cout << "Password: " << inMD5Password << "\n";
      std::ostringstream _condition;
      _condition << "username='" << inUsername << "'";
      _condition << " AND password='" << inMD5Password << "'";

      std::cout << "Condition: " << _condition.str() << "\n";

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
    void do_load_puppet(std::string inProfileName, Puppet& inPuppet, boost::tuple<Callback> callback)
    {
#if 0 // __DISABLED__ waiting until server & res mgr are migrated to new API
      //pqxx::result result_;
      std::ostringstream _condition;
      _condition << "puppets.name=" << escape(inProfileName);
      get_records(result_, "puppets", "*", _condition.str().c_str());

      if (result_.empty())
        throw std::runtime_error("ERROR! Profile does not exist in database for loading!");

      inPuppet.setName(result_[0]["name"].c_str());
      inPuppet.setRace((RACE)convertTo<int>(result_[0]["race"].c_str()));
      inPuppet.setLevel(convertTo<int>(result_[0]["level"].c_str()));
      inPuppet.setIntelligence(convertTo<int>(result_[0]["intelligence"].c_str()));
      inPuppet.setVitality(convertTo<int>(result_[0]["vitality"].c_str()));
      Server::getSingleton().getResMgr()._assignTalents(inPuppet, result_[0]["talents"].c_str());

      // get the decks
      result_.clear();
      _condition.str("");
      _condition << "decks.puppet=" << escape(inPuppet.getName());
      get_records(result_, "decks", "name,spells,use_count", _condition.str().c_str());
      pqxx::result::const_iterator lDeck;
      for (lDeck = result_.begin(); lDeck != result_.end(); ++lDeck) {
        Server::getSingleton().getResMgr()._assignDeck(
          inPuppet,
          (*lDeck)["name"].c_str(),
          (*lDeck)["spells"].c_str(),
          convertTo<int>((*lDeck)["use_count"].c_str()));
      }
#endif

      boost::get<0>(callback)(db_result::Ok);
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
