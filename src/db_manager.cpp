/*
 *  DBManager.cpp
 *  Elementum
 *
 *  Created by Ahmad Amireh on 2/20/10.
 *  Copyright 2010 JU. All rights reserved.
 *
 */

#include "db_manager.hpp"
#include "PixyUtility.h"
#include "server.hpp"

namespace Pixy {
namespace Net {

	//DBManager* DBManager::mDBMgr = NULL;
	DBManager::DBManager(boost::asio::io_service& io_service)
  : strand_(io_service),
    mConn(0),
    mLog(new log4cpp::FixedContextCategory(PIXY_LOG_CATEGORY, "DBManager"))
  {
	}
	DBManager::~DBManager() {

		if (mConn) {
			delete mConn;
			mConn = NULL;
		}

    mLog->infoStream() << "shutting down";

		if (mLog) {
			delete mLog;
			mLog = NULL;
		}

	}
	/*
	DBManager* DBManager::getSingletonPtr() {
		if (!mDBMgr)
			mDBMgr = new DBManager();

		return mDBMgr;
	}
	*/

	/* ----------------------------------------------- *
	 *	CONNECTIVITY
	 * ----------------------------------------------- */
	bool DBManager::connect() {


		if (mConn && mConn->is_open()) {
			std::cerr << "already connected!";
			return true;
		}

		// prompt password
		std::string inPW = "tuonela";
		/*
		 std::cout << "| Enter database pw: ";
		std::cin >> inPW;
		*/
		try {
			std::ostringstream _connInfo;
			_connInfo << "hostaddr=" << DB_HOST
			<< " port=" << DB_PORT
			<< " dbname=" << DB_DBNAME
			<< " user=" << DB_USER
			<< " password=" << inPW;
			std::cerr << "connecting to backend server " << DB_USER << "@" << DB_HOST << ":" << DB_PORT << "/" << DB_DBNAME << "\n";
			mConn = new pqxx::connection(_connInfo.str());
			//return true;
		} catch (std::runtime_error& e ) {
			std::cerr << "connection to backend failed! << " << e.what() << "\n" ;
		} catch (std::exception& e ) {
			std::cerr << "unexpected error! " << e.what() << "\n" ;
		}
    fflush(stderr);

		assert(mConn != NULL);

    std::cout << "DB: connected!\n";

		return false;
	}

	void DBManager::disconnect() {
		mLog->infoStream() << "disconnecting";
		if (mConn->is_open())
			mConn->disconnect();
	}

  bool DBManager::isConnected() const {
    return (mConn && mConn->is_open());
  }


	/* ----------------------------------------------- *
	 *	RECORD MANIPULATION
	 * ----------------------------------------------- */

	void
	DBManager::getRecords(pqxx::result& out,
    const char* inTable,
		const char* inFields,
		const char* inCondition,
		const char* inOrder)
	{
		// construct our query
    //pqxx::result mResult;
		std::ostringstream _sql;
		_sql << "SELECT ";
		(inFields == "") ? _sql << "*" : _sql << inFields;
		_sql << " FROM " << inTable;
		(inCondition == "") ? :	_sql << " WHERE " << inCondition;
		(inOrder == "") ? : _sql << " ORDER BY " << inOrder << " ASC";

		// execute query and save result
		execQuery(_sql.str().c_str(), &out);

		// done!
		//return mResult;
	}

	bool DBManager::createRecord(const char* inTable,
								 const char* inFields,
								 const char* inValues)
	{
    //pqxx::result mResult;
    mResult.clear();
		std::ostringstream _sql;
		_sql	<< "INSERT INTO "
				<< inTable << " (" << inFields << ")"
			    << " VALUES (" << inValues << ")";

		execQuery(_sql.str().c_str(), &mResult);

		return (mResult.affected_rows() > 0);
	}

	bool DBManager::deleteRecords(const char* inTable,
								  const char* inCondition)
	{
    //pqxx::result mResult;
    mResult.clear();
		std::ostringstream _sql;
		_sql << "DELETE FROM " << inTable << " WHERE " << inCondition;

		execQuery(_sql.str().c_str(), &mResult);

		return (mResult.affected_rows() > 1);
	}

	bool DBManager::updateRecord(const char* inTable,
								  const char* inField,
								  const char* inCondition,
								  const char* inNewValue)
	{
    //pqxx::result mResult;
    mResult.clear();
		std::ostringstream _sql;
		_sql << "UPDATE "
		<< inTable
		<< " SET " << inField
		<< "='" << inNewValue
		<< "' WHERE " << inCondition;

		execQuery(_sql.str().c_str(), &mResult);

		return (mResult.affected_rows() > 0);
	}

	/* ----------------------------------------------- *
	 *	AUTHENTICATION & STATUS
	 * ----------------------------------------------- */

	bool DBManager::isLogged(const char* inUsername)
	{
    pqxx::result mResult;
		std::ostringstream _condition;
		_condition << "username='" << inUsername << "' AND is_online=TRUE";
    getRecords(mResult, "accounts", "count(*)", _condition.str().c_str());
		return (convertTo<int>(mResult[0]["count"].c_str()) > 0);
	}

	/* ----------------------------------------------- *
	 *	USERS
	 * ----------------------------------------------- */
#if 0 // __DISABLED__ account management will not be done in-game
	bool
  DBManager::createAccount(
    const char* inUsername,
		const char* inMD5Password) {

		std::ostringstream _values;
		_values << "'" << inUsername << "', '" << inMD5Password << "'";
		bool _success = false;
		try {
			_success = createRecord("accounts", "username, password", _values.str().c_str());
		} catch (...) {
			mLog->errorStream() << "couldn't create user, probably exists!";
		}
		return _success;
	}

	bool DBManager::deleteAccount(const char* inUsername)
	{
		std::string _condition = "username='";
		_condition += inUsername;
		_condition += "'";
		return deleteRecords("accounts", _condition.c_str());
	}
#endif

	/* ----------------------------------------------- *
	 *	PROFILES
	 * ----------------------------------------------- */
	bool DBManager::createPuppet(Player const* inPlayer, const Puppet& inPuppet)
	{
		std::ostringstream _fields, _values, _delCondition;
    _fields << "account, name, race, intelligence, vitality, talents";
		_values
    << escape(stringify(inPlayer->getUserId())) << ", "
    << escape(inPuppet.getName()) << ", "
    << inPuppet.getRace() << ", "
    << escape(stringify<int>(inPuppet.getIntelligence()))	<< ", "
    << escape(stringify<int>(inPuppet.getVitality())) << ", "
    << "{\"\"}";

		// create profile
		return createRecord("puppets", _fields.str().c_str(), _values.str().c_str());
	}

  bool DBManager::updatePuppet(Player const* inPlayer, const Puppet& inPuppet) {
    pqxx::result mResult;
		std::ostringstream _sql;
		_sql
    << "UPDATE puppets SET "
    << "intelligence=" << escape(stringify<int>(inPuppet.getIntelligence()))
    << ", vitality=" << escape(stringify<int>(inPuppet.getVitality()))
		<< " WHERE " << "puppets.name=" << escape(inPuppet.getName());

		execQuery(_sql.str().c_str(), &mResult);

		return (mResult.affected_rows() > 0);
  }

	bool DBManager::deletePuppet(const Player& inPlayer, const char* inPuppet)
	{
		std::ostringstream _condition;
		_condition << "name=" << escape(inPuppet) << " AND account=" << inPlayer.getUserId();
		return deleteRecords("puppets", _condition.str().c_str());
	}
	void DBManager::loadAllProfiles(const Player& inPlayer,
									std::vector<Puppet*>& inContainer)
	{
		/*std::ostringstream _condition;
		_condition << "id=" << escape(stringify<int>(getUserId(inUsername)));
		pqxx::result mResult;
    getRecords(mResult, "puppets", "*", _condition.str().c_str());

		if (!inContainer.empty())
			inContainer.clear();

		Puppet *_puppet;
		std::cout << "| loadAllProfiles() : found " << mResult.size() << " puppets.";
		for (pqxx::result::size_type i=0; i<mResult.size(); ++i)
		{
			_puppet = new Puppet();
			loadPuppet(mResult[i]["name"].c_str(), *_puppet);
			mLog->infoStream() << "| Adding profile named " << _puppet->getName() ;
			inContainer.push_back(new Puppet(*_puppet));
			delete _puppet;
		}*/
	}

	/* ----------------------------------------------- *
	 *	HELPERS / UTILITY
	 * ----------------------------------------------- */
	void
	DBManager::execQuery(const char* inQuery, pqxx::result* outResult)
	{
		try {
      //lock();
			//mWorker = new pqxx::work(*mConn, "Transaction");
      pqxx::work mWorker(*mConn, "Transaction");
      //unlock();

			// attach query to our worker
			//mLog->infoStream() << "attaching query for execution:\n\t`" << inQuery << "`";

			if (outResult)
				*outResult = mWorker.exec(inQuery);
			else
				mWorker.exec(inQuery);

			// perform query
      //lock();
			mWorker.commit();
      //unlock();
		} catch (const pqxx::argument_error& e) {
			// in case of an exception thrown
			// we're responsible for aborting the transaction
			mLog->errorStream() << "invalid arguments for operation! '" << e.what() << "'";
		} catch (const pqxx::unique_violation& e) {
			mLog->errorStream() << "record violates a unique constraint, record with same PK exists! '" << e.what() << "'";
		} catch (const pqxx::integrity_constraint_violation& e) {
			mLog->errorStream() << "record violates a constraint! '" << e.what() << "'";
		} catch (const pqxx::sql_error& e) {
			mLog->errorStream() << "an unknown error occured during executing a transaction! '" << e.what() << "'";
    } catch (const std::exception& e) {
      mLog->errorStream() << "an unknown error occured during executing a transaction! '" << e.what() << "'";
		} catch (...) {
			mLog->errorStream() << "an unknown exception has been caught during executing a transaction! degrading gracefully..";
		}

    //delete mWorker; // we're done, clean up
	}

	std::string DBManager::escape(std::string inString)
	{
		std::ostringstream _escaped;
		_escaped << "'" << inString << "'";
		return _escaped.str();
	}

	int DBManager::getUserId(const char* inUsername)
	{
		std::ostringstream _getCondition;
    pqxx::result mResult;
		_getCondition<< "username=" << escape(inUsername);
    getRecords(mResult, "accounts", "id", _getCondition.str().c_str());
		return convertTo<int>(mResult[0]["id"].c_str());
	}

	std::string DBManager::getUsername(const int inUserId) {
		std::ostringstream _getCondition;
    pqxx::result mResult;
		_getCondition<< "id=" << inUserId;
    getRecords(mResult, "accounts", "username", _getCondition.str().c_str());
		return mResult[0]["username"].c_str();
	}

#if 0
	// tests functionality of this class
	void DBManager::runTests()
	{
		std::string _userName, _pw, _encPW;
		std::ostringstream _createValues;
		bool _success;
		Puppet *_puppet;

		mLog->debugStream() << "**** RUNNING TESTS ****";

		_userName = "testUser";
		_pw = "testPassword";
		_encPW = MD5((unsigned char*)_pw.c_str()).hex_digest();

		mLog->debugStream() << "Data: \n"
		<< "Username: " << _userName << "\n"
		<< "Password: " << _pw;

		/* ----------------------------------------------- *
		 *	USERS
		 * ----------------------------------------------- */
		mLog->debugStream() << "----- USERS -----";

		mLog->debugStream() << "creating user";
		createAccount(_userName.c_str(), MD5((unsigned char*)_pw.c_str()).hex_digest());
		mLog->debugStream() << "creating user that exists";
		createAccount(_userName.c_str(), MD5((unsigned char*)_pw.c_str()).hex_digest());
		mLog->debugStream() << "deleting user";
		deleteAccount(_userName.c_str());
		mLog->debugStream() << "creating user that does not exist";
		deleteAccount("NONEXISTENT");
		mLog->debugStream() << "creating user to carry on";
		createAccount(_userName.c_str(), MD5((unsigned char*)_pw.c_str()).hex_digest());


		/* ----------------------------------------------- *
		 *	RECORDS
		 * ----------------------------------------------- */

		/* !! SELECT RECORDS !! */
		/*
		 mLog->debugStream() << "----- RECORDS -----";
		pqxx::result _res = getRecords("users");

		for (pqxx::result::size_type i=0; i<_res.size(); ++i ) {
			mLog->debugStream() << _res[i]["username"] << " : " << _res[i]["password"] ;
		}
		*/

		/* !! CREATE RECORD !! */
		/*
		mLog->debugStream() << "creating record";

		 mLog->debugStream() << "MD5 Encrypted version of " << _pw << " : " << _encPW ;
		 std::ostringstream _values;
		 _values << "'kandie', '" << _encPW << "'";
		 _success = createRecord("users", "username, password", _values.str());
		 if (_success)
			 mLog->debugStream() << "Insertion was successfull";
		 else
			 mLog->debugStream() << "Insertion failed";
		*/

		/* !! DELETE RECORD !! */
		/*
		mLog->debugStream() << "deleting record";
		 _success = deleteRecords("users", "username='kandie'");
		 if (_success)
		 mLog->debugStream() << "| Deletion was successfull! PEWPEW";
		*/
/*
		// -----------------------------------------------
		 //	AUTHENTICATION
		 // -----------------------------------------------

		mLog->debugStream() << "----- AUTHENTICATION -----";
		// !! AUTHENTICATE !!

		// std::string _userName = "kandie";
		 _success = authenticate(_userName.c_str(), _encPW.c_str());
		 if (_success)
			 mLog->debugStream() << "| User authenticated.";
		 else
			 mLog->debugStream() << "| User authentication failed.";


		// !! LOGIN && LOGOUT !!
		mLog->debugStream() << "logging in";
		 login("kandie");
		mLog->debugStream() << "logging out";
		 logout("kandie");


		// -----------------------------------------------
		 //	PROFILES
		 // -----------------------------------------------

		mLog->debugStream() << "----- PROFILES -----";
		// !! CREATE / UPDATE PROFILE !!

		mLog->debugStream() << "creating profile";
		std::string _profileName = "testProfile";
		 _puppet = new Puppet();
		 _puppet->setName(_profileName.c_str());
		 _puppet->setRace(FIRE);
		 _puppet->setIntelligence(10);
		 _puppet->setVitality(25);
		 _puppet->setAlignment(H_EVIL);
		 _puppet->setWins(3);
		 _puppet->setLosses(2);
		 savePuppet(_userName.c_str(), *_puppet);
		 delete _puppet;


		// !! LOAD PROFILE !!
		mLog->debugStream() << "loading profile";
		 _puppet = new Puppet();
		 loadPuppet(_profileName.c_str(), *_puppet);
		 //_puppet->printInfo();
		 mLog->debugStream() << "Name : " << _puppet->getName() ;
		 mLog->debugStream() << "Race : " << raceToString(_puppet->getRace()) ;
		 mLog->debugStream() << "Intelligence : " << _puppet->getIntelligence() ;
		 mLog->debugStream() << "Vitality : " << _puppet->getVitality() ;
		 mLog->debugStream() << "Alignment : " << alignmentToString(_puppet->getAlignment()) ;
		 mLog->debugStream() << "Wins : " << _puppet->getWins() ;
		 mLog->debugStream() << "Losses : " << _puppet->getLosses() ;
		 delete _puppet;


		// !! LOAD ALL PROFILES !!
		mLog->debugStream() << "loading all profiles for use";
		std::vector<Puppet*> mPuppets;
		loadAllProfiles("kandie", mPuppets);
		mLog->debugStream() << "user has " << mPuppets.size() << " puppets.";
		for (std::vector<Puppet*>::const_iterator _itr = mPuppets.begin();
			 _itr != mPuppets.end();
			 ++_itr)
		{
			mLog->debugStream() << " **** ------- **** ";
			_puppet = *_itr;
			mLog->debugStream() << "Name : " << _puppet->getName() ;

			//mLog->debugStream() << "Race : " << raceToString(_puppet->getRace()) ;
			//mLog->debugStream() << "Intelligence : " << _puppet->getIntelligence() ;
			//mLog->debugStream() << "Vitality : " << _puppet->getVitality() ;
			//mLog->debugStream() << "Alignment : " << alignmentToString(_puppet->getAlignment()) ;
			//mLog->debugStream() << "Wins : " << _puppet->getWins() ;
			//mLog->debugStream() << "Losses : " << _puppet->getLosses() ;

			_puppet = NULL;
		}
		// clear up
		mPuppets.clear();
    */
	}
#endif
}
}
