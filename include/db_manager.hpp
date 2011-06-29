/*
 *  DBManager.h
 *  Elementum
 *
 *  Created by Ahmad Amireh on 2/20/10.
 *  Copyright 2010 JU. All rights reserved.
 *
 */

#ifndef H_DBManager_H
#define H_DBManager_H

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


  enum class DBResult : unsigned char {
    Unassigned = 0,

    Ok, // the operation was succesful

    // to check for an error generally: (DBResult > DBResult::Ok)
    InvalidCredentials,
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
	class DBManager {
	public:
		DBManager(boost::asio::io_service&);
		~DBManager();

    bool isConnected() const;

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
		void login(const char* inUsername, const char* inPassword, Callback callback) {

      void (DBManager::*f)(const char*, const char*, boost::tuple<Callback>)
        = &DBManager::do_login<Callback>;

      strand_.post( boost::bind(f, this, inUsername, inPassword, boost::make_tuple(callback)) );
    }

		/*! \brief
		 *	Flags account with inUsername as offline in the DB.
		 */
    template <typename Callback>
		void logout(const char* inUsername, Callback callback) {
      void (DBManager::*f)(const char*, const char*, boost::tuple<Callback>)
        = &DBManager::do_logout<Callback>;

      strand_.post( boost::bind(f, this, inUsername, boost::make_tuple(callback)) );
    }

		/*! \brief
		 *	Loads a puppet into inPuppet.
     * @return
     *  false if no such puppet exists, true otherwise
		 */
    template <typename Callback>
		bool load_puppet(const char* inName, Puppet& inPuppet, Callback callback) {
      void (DBManager::*f)(const char*, Puppet&, boost::tuple<Callback>)
        = &DBManager::do_load_puppet<Callback>;

      strand_.post( boost::bind(f, this, inName, inPuppet, boost::make_tuple(callback)) );
    }

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


	protected:

    /// helpers for retrieving user id's
		int getUserId(const char* inUsername);
		std::string getUsername(const int inUserId);

    // used by login()
    template <typename Callback>
		void do_login(const char* inUsername, const char* inMD5Password, boost::tuple<Callback> callback) {
      DBResult dbr = DBResult::Unassigned;

      //pqxx::result mResult;
      std::ostringstream _condition;
      _condition	<< "username='" << inUsername << "'"
      << " AND password='" << inMD5Password << "'";

      mResult.clear();
      getRecords(mResult, "accounts", "COUNT(id)", _condition.str().c_str());

      bool success = convertTo<int>(mResult[0]["count"].c_str()) == 1;

      if (success) {
        std::ostringstream _condition;
        _condition << "username='" << inUsername << "'";
        updateRecord("accounts", "is_online", _condition.str().c_str(), "true");
        updateRecord("accounts", "last_login_at", _condition.str().c_str(), "now()");

        dbr = DBResult::Ok;
      }

      dbr = DBResult::InvalidCredentials;
      boost::get<0>(callback)(dbr);
    }

    template <typename Callback>
    void do_logout(const char* inUsername, boost::tuple<Callback> callback) {

      std::ostringstream _condition;
      _condition << "username='" << inUsername << "'";
      updateRecord("accounts", "is_online", _condition.str().c_str(), "false");

      boost::get<0>(callback)(DBResult::Ok);
    }

    template <typename Callback>
    void do_load_puppet(const char* inProfileName, Puppet& inPuppet, boost::tuple<Callback> callback)
    {
#if 0 // __DISABLED__ waiting until server & res mgr are migrated to new API
      //pqxx::result mResult;
      std::ostringstream _condition;
      _condition << "puppets.name=" << escape(inProfileName);
      getRecords(mResult, "puppets", "*", _condition.str().c_str());

      if (mResult.empty())
        throw std::runtime_error("ERROR! Profile does not exist in database for loading!");

      inPuppet.setName(mResult[0]["name"].c_str());
      inPuppet.setRace((RACE)convertTo<int>(mResult[0]["race"].c_str()));
      inPuppet.setLevel(convertTo<int>(mResult[0]["level"].c_str()));
      inPuppet.setIntelligence(convertTo<int>(mResult[0]["intelligence"].c_str()));
      inPuppet.setVitality(convertTo<int>(mResult[0]["vitality"].c_str()));
      Server::getSingleton().getResMgr()._assignTalents(inPuppet, mResult[0]["talents"].c_str());

      // get the decks
      mResult.clear();
      _condition.str("");
      _condition << "decks.puppet=" << escape(inPuppet.getName());
      getRecords(mResult, "decks", "name,spells,use_count", _condition.str().c_str());
      pqxx::result::const_iterator lDeck;
      for (lDeck = mResult.begin(); lDeck != mResult.end(); ++lDeck) {
        Server::getSingleton().getResMgr()._assignDeck(
          inPuppet,
          (*lDeck)["name"].c_str(),
          (*lDeck)["spells"].c_str(),
          convertTo<int>((*lDeck)["use_count"].c_str()));
      }
#endif

      boost::get<0>(callback)(DBResult::Ok);
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
		void
		getRecords(pqxx::result& out,
      const char* inTable,
			const char* inFields="*",
			const char* inCondition="",
			const char* inOrder="");

		/*! Inserts a record into inTable
		 *
		 *	@return
		 *		true if the insertion was successful, false if not
		 */
		bool
		createRecord(const char* inTable,
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
		deleteRecords(const char* inTable,
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
		updateRecord(const char* inTable,
					 const char* inField,
					 const char* inCondition,
					 const char* inNewValue);

		/*! \brief
		 *	Shows line status of a user.
		 */
		bool isLogged(const char* inUsername);

		pqxx::connection *mConn; /// our db connection handle
		//pqxx::work *mWorker; /// performs transactions

		/* since 1 transaction can be performed at any
		 * single time per instance, one result object alone
     * could be used for holding results
     */
		pqxx::result mResult;

		/// helper for firing up the transactor and  executing the sql queries
		void execQuery(const char* inQuery, pqxx::result* outResult);

		/// helper for escaping strings for use in queries
		std::string escape(std::string inString);



		void runTests();

		log4cpp::FixedContextCategory *mLog;

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
		DBManager(const DBManager& src) = delete;
		DBManager& operator=(const DBManager& rhs) = delete;
	};
}
}
#endif
