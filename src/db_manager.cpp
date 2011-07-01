/*
 *  db_manager.cpp
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

	//db_manager* db_manager::mDBMgr = NULL;
	db_manager::db_manager(boost::asio::io_service& io_service)
  : strand_(io_service),
    conn_(0),
    log_(new log4cpp::FixedContextCategory(PIXY_LOG_CATEGORY, "db_manager"))
  {
	}
	db_manager::~db_manager() {

		if (conn_) {
			delete conn_;
			conn_ = NULL;
		}

    log_->infoStream() << "shutting down";

		if (log_) {
			delete log_;
			log_ = NULL;
		}

	}
	/*
	db_manager* db_manager::getSingletonPtr() {
		if (!mDBMgr)
			mDBMgr = new db_manager();

		return mDBMgr;
	}
	*/

	/* ----------------------------------------------- *
	 *	CONNECTIVITY
	 * ----------------------------------------------- */
	bool db_manager::connect() {


		if (conn_ && conn_->is_open()) {
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
			log_->infoStream() << "connecting to backend server " << DB_USER << "@" << DB_HOST << ":" << DB_PORT << "/" << DB_DBNAME << "\n";
			conn_ = new pqxx::connection(_connInfo.str());
			//return true;
		} catch (std::runtime_error& e ) {
			log_->errorStream() << "connection to backend failed! << " << e.what() << "\n" ;
		} catch (std::exception& e ) {
			log_->errorStream() << "unexpected error! " << e.what() << "\n" ;
		}
    fflush(stderr);

		assert(conn_ != NULL);

    std::cout << "connected to PostgreSQL database!\n";

		return false;
	}

	void db_manager::disconnect() {
		log_->infoStream() << "disconnecting";
		if (conn_->is_open())
			conn_->disconnect();
	}

  bool db_manager::is_connected() const {
    return (conn_ && conn_->is_open());
  }


	/* ----------------------------------------------- *
	 *	RECORD MANIPULATION
	 * ----------------------------------------------- */

	bool
	db_manager::get_records(pqxx::result& out,
    std::string inTable,
		std::string inFields,
		std::string inCondition,
		std::string inOrder)
	{
		// construct our query
    //pqxx::result result_;
		std::ostringstream _sql;
		_sql << "SELECT ";
		_sql << (inFields.empty() ? "*" : inFields);
		_sql << " FROM " << inTable;
		if (!inCondition.empty())
      _sql << " WHERE " << inCondition;
		if (!inOrder.empty())
      _sql << " ORDER BY " << inOrder << " ASC";

    std::cout << "SQL condition: " << inCondition << "\n";

		// execute query and save result
		return exec_query(_sql.str().c_str(), &out);

		// done!
		//return result_;
	}

	bool db_manager::create_record(const char* inTable,
								 const char* inFields,
								 const char* inValues)
	{
    //pqxx::result result_;
    result_.clear();
		std::ostringstream _sql;
		_sql	<< "INSERT INTO "
				<< inTable << " (" << inFields << ")"
			    << " VALUES (" << inValues << ")";

		exec_query(_sql.str().c_str(), &result_);

		return (result_.affected_rows() > 0);
	}

	bool db_manager::delete_records(const char* inTable,
								  const char* inCondition)
	{
    //pqxx::result result_;
    result_.clear();
		std::ostringstream _sql;
		_sql << "DELETE FROM " << inTable << " WHERE " << inCondition;

		exec_query(_sql.str().c_str(), &result_);

		return (result_.affected_rows() > 1);
	}

	bool db_manager::update_record(const char* inTable,
								  const char* inField,
								  const char* inCondition,
								  const char* inNewValue)
	{
    //pqxx::result result_;
    result_.clear();
		std::ostringstream _sql;
		_sql << "UPDATE "
		<< inTable
		<< " SET " << inField
		<< "='" << inNewValue
		<< "' WHERE " << inCondition;

		exec_query(_sql.str().c_str(), &result_);

		return (result_.affected_rows() > 0);
	}

	/* ----------------------------------------------- *
	 *	AUTHENTICATION & STATUS
	 * ----------------------------------------------- */

	bool db_manager::is_logged(const char* inUsername)
	{
    pqxx::result result_;
		std::ostringstream _condition;
		_condition << "username='" << inUsername << "' AND is_online=TRUE";
    get_records(result_, "accounts", "count(*)", _condition.str().c_str());
		return (convertTo<int>(result_[0]["count"].c_str()) > 0);
	}

	/* ----------------------------------------------- *
	 *	USERS
	 * ----------------------------------------------- */
#if 0 // __DISABLED__ account management will not be done in-game
	bool
  db_manager::createAccount(
    const char* inUsername,
		const char* inMD5Password) {

		std::ostringstream _values;
		_values << "'" << inUsername << "', '" << inMD5Password << "'";
		bool _success = false;
		try {
			_success = create_record("accounts", "username, password", _values.str().c_str());
		} catch (...) {
			log_->errorStream() << "couldn't create user, probably exists!";
		}
		return _success;
	}

	bool db_manager::deleteAccount(const char* inUsername)
	{
		std::string _condition = "username='";
		_condition += inUsername;
		_condition += "'";
		return delete_records("accounts", _condition.c_str());
	}


	/* ----------------------------------------------- *
	 *	PROFILES
	 * ----------------------------------------------- */
	bool db_manager::createPuppet(Player const* inPlayer, const Puppet& inPuppet)
	{
		std::ostringstream _fields, _values, _delCondition;
    _fields << "account, name, race, intelligence, vitality, talents";
		_values
    << escape(stringify(inPlayer->get_userid())) << ", "
    << escape(inPuppet.getName()) << ", "
    << inPuppet.getRace() << ", "
    << escape(stringify<int>(inPuppet.getIntelligence()))	<< ", "
    << escape(stringify<int>(inPuppet.getVitality())) << ", "
    << "{\"\"}";

		// create profile
		return create_record("puppets", _fields.str().c_str(), _values.str().c_str());
	}

  bool db_manager::updatePuppet(Player const* inPlayer, const Puppet& inPuppet) {
    pqxx::result result_;
		std::ostringstream _sql;
		_sql
    << "UPDATE puppets SET "
    << "intelligence=" << escape(stringify<int>(inPuppet.getIntelligence()))
    << ", vitality=" << escape(stringify<int>(inPuppet.getVitality()))
		<< " WHERE " << "puppets.name=" << escape(inPuppet.getName());

		exec_query(_sql.str().c_str(), &result_);

		return (result_.affected_rows() > 0);
  }

	bool db_manager::deletePuppet(const Player& inPlayer, const char* inPuppet)
	{
		std::ostringstream _condition;
		_condition << "name=" << escape(inPuppet) << " AND account=" << inPlayer.get_userid();
		return delete_records("puppets", _condition.str().c_str());
	}
	void db_manager::loadAllProfiles(const Player& inPlayer,
									std::vector<Puppet*>& inContainer)
	{
		/*std::ostringstream _condition;
		_condition << "id=" << escape(stringify<int>(get_userid(inUsername)));
		pqxx::result result_;
    get_records(result_, "puppets", "*", _condition.str().c_str());

		if (!inContainer.empty())
			inContainer.clear();

		Puppet *_puppet;
		std::cout << "| loadAllProfiles() : found " << result_.size() << " puppets.";
		for (pqxx::result::size_type i=0; i<result_.size(); ++i)
		{
			_puppet = new Puppet();
			loadPuppet(result_[i]["name"].c_str(), *_puppet);
			log_->infoStream() << "| Adding profile named " << _puppet->getName() ;
			inContainer.push_back(new Puppet(*_puppet));
			delete _puppet;
		}*/
	}
#endif // __DISABLED__
	/* ----------------------------------------------- *
	 *	HELPERS / UTILITY
	 * ----------------------------------------------- */
	bool
	db_manager::exec_query(const char* inQuery, pqxx::result* outResult)
	{
    bool success = false;
		try {
      //lock();
			//mWorker = new pqxx::work(*conn_, "Transaction");
      pqxx::work mWorker(*conn_, "Transaction");
      //unlock();

			// attach query to our worker
			//log_->infoStream() << "attaching query for execution:\n\t`" << inQuery << "`";

			if (outResult)
				*outResult = mWorker.exec(inQuery);
			else
				mWorker.exec(inQuery);

			// perform query
      //lock();
			mWorker.commit();
      success = true;
      //unlock();
		} catch (const pqxx::argument_error& e) {
			// in case of an exception thrown
			// we're responsible for aborting the transaction
			log_->errorStream() << "invalid arguments for operation! '" << e.what() << "'";
		} catch (const pqxx::unique_violation& e) {
			log_->errorStream() << "record violates a unique constraint, record with same PK exists! '" << e.what() << "'";
		} catch (const pqxx::integrity_constraint_violation& e) {
			log_->errorStream() << "record violates a constraint! '" << e.what() << "'";
		} catch (const pqxx::sql_error& e) {
			log_->errorStream() << "an SQL error occured during executing a transaction! '" << e.what() << "'";
      log_->errorStream() << "query: " << inQuery;
    } catch (const std::exception& e) {
      log_->errorStream() << "an unknown error occured during executing a transaction! '" << e.what() << "'";
      log_->errorStream() << "query: " << inQuery;
		} catch (...) {
			log_->errorStream() << "an unknown exception has been caught during executing a transaction! degrading gracefully..";
		}

    //log_->debugStream() << "executed query: " << inQuery << ", result =" << (success ? "succ" : "failed");

    return success;

    //delete mWorker; // we're done, clean up
	}

	std::string db_manager::escape(std::string inString)
	{
		std::ostringstream _escaped;
		_escaped << "'" << inString << "'";
		return _escaped.str();
	}

	int db_manager::get_userid(const char* inUsername)
	{
		std::ostringstream _getCondition;
    pqxx::result result_;
		_getCondition<< "username=" << escape(inUsername);
    get_records(result_, "accounts", "id", _getCondition.str().c_str());
		return convertTo<int>(result_[0]["id"].c_str());
	}

	std::string db_manager::get_username(const int inUserId) {
		std::ostringstream _getCondition;
    pqxx::result result_;
		_getCondition<< "id=" << inUserId;
    get_records(result_, "accounts", "username", _getCondition.str().c_str());
		return result_[0]["username"].c_str();
	}

#if 0
	// tests functionality of this class
	void db_manager::runTests()
	{
		std::string _userName, _pw, _encPW;
		std::ostringstream _createValues;
		bool _success;
		Puppet *_puppet;

		log_->debugStream() << "**** RUNNING TESTS ****";

		_userName = "testUser";
		_pw = "testPassword";
		_encPW = MD5((unsigned char*)_pw.c_str()).hex_digest();

		log_->debugStream() << "Data: \n"
		<< "Username: " << _userName << "\n"
		<< "Password: " << _pw;

		/* ----------------------------------------------- *
		 *	USERS
		 * ----------------------------------------------- */
		log_->debugStream() << "----- USERS -----";

		log_->debugStream() << "creating user";
		createAccount(_userName.c_str(), MD5((unsigned char*)_pw.c_str()).hex_digest());
		log_->debugStream() << "creating user that exists";
		createAccount(_userName.c_str(), MD5((unsigned char*)_pw.c_str()).hex_digest());
		log_->debugStream() << "deleting user";
		deleteAccount(_userName.c_str());
		log_->debugStream() << "creating user that does not exist";
		deleteAccount("NONEXISTENT");
		log_->debugStream() << "creating user to carry on";
		createAccount(_userName.c_str(), MD5((unsigned char*)_pw.c_str()).hex_digest());


		/* ----------------------------------------------- *
		 *	RECORDS
		 * ----------------------------------------------- */

		/* !! SELECT RECORDS !! */
		/*
		 log_->debugStream() << "----- RECORDS -----";
		pqxx::result _res = get_records("users");

		for (pqxx::result::size_type i=0; i<_res.size(); ++i ) {
			log_->debugStream() << _res[i]["username"] << " : " << _res[i]["password"] ;
		}
		*/

		/* !! CREATE RECORD !! */
		/*
		log_->debugStream() << "creating record";

		 log_->debugStream() << "MD5 Encrypted version of " << _pw << " : " << _encPW ;
		 std::ostringstream _values;
		 _values << "'kandie', '" << _encPW << "'";
		 _success = create_record("users", "username, password", _values.str());
		 if (_success)
			 log_->debugStream() << "Insertion was successfull";
		 else
			 log_->debugStream() << "Insertion failed";
		*/

		/* !! DELETE RECORD !! */
		/*
		log_->debugStream() << "deleting record";
		 _success = delete_records("users", "username='kandie'");
		 if (_success)
		 log_->debugStream() << "| Deletion was successfull! PEWPEW";
		*/
/*
		// -----------------------------------------------
		 //	AUTHENTICATION
		 // -----------------------------------------------

		log_->debugStream() << "----- AUTHENTICATION -----";
		// !! AUTHENTICATE !!

		// std::string _userName = "kandie";
		 _success = authenticate(_userName.c_str(), _encPW.c_str());
		 if (_success)
			 log_->debugStream() << "| User authenticated.";
		 else
			 log_->debugStream() << "| User authentication failed.";


		// !! LOGIN && LOGOUT !!
		log_->debugStream() << "logging in";
		 login("kandie");
		log_->debugStream() << "logging out";
		 logout("kandie");


		// -----------------------------------------------
		 //	PROFILES
		 // -----------------------------------------------

		log_->debugStream() << "----- PROFILES -----";
		// !! CREATE / UPDATE PROFILE !!

		log_->debugStream() << "creating profile";
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
		log_->debugStream() << "loading profile";
		 _puppet = new Puppet();
		 loadPuppet(_profileName.c_str(), *_puppet);
		 //_puppet->printInfo();
		 log_->debugStream() << "Name : " << _puppet->getName() ;
		 log_->debugStream() << "Race : " << raceToString(_puppet->getRace()) ;
		 log_->debugStream() << "Intelligence : " << _puppet->getIntelligence() ;
		 log_->debugStream() << "Vitality : " << _puppet->getVitality() ;
		 log_->debugStream() << "Alignment : " << alignmentToString(_puppet->getAlignment()) ;
		 log_->debugStream() << "Wins : " << _puppet->getWins() ;
		 log_->debugStream() << "Losses : " << _puppet->getLosses() ;
		 delete _puppet;


		// !! LOAD ALL PROFILES !!
		log_->debugStream() << "loading all profiles for use";
		std::vector<Puppet*> mPuppets;
		loadAllProfiles("kandie", mPuppets);
		log_->debugStream() << "user has " << mPuppets.size() << " puppets.";
		for (std::vector<Puppet*>::const_iterator _itr = mPuppets.begin();
			 _itr != mPuppets.end();
			 ++_itr)
		{
			log_->debugStream() << " **** ------- **** ";
			_puppet = *_itr;
			log_->debugStream() << "Name : " << _puppet->getName() ;

			//log_->debugStream() << "Race : " << raceToString(_puppet->getRace()) ;
			//log_->debugStream() << "Intelligence : " << _puppet->getIntelligence() ;
			//log_->debugStream() << "Vitality : " << _puppet->getVitality() ;
			//log_->debugStream() << "Alignment : " << alignmentToString(_puppet->getAlignment()) ;
			//log_->debugStream() << "Wins : " << _puppet->getWins() ;
			//log_->debugStream() << "Losses : " << _puppet->getLosses() ;

			_puppet = NULL;
		}
		// clear up
		mPuppets.clear();
    */
	}
#endif
}
}
