/****************************************************************************
*	@ 	  
*	@desc	数据连接
*	@ 	 
*	@file	db/DBConnector.cpp
*	@ 	null
******************************************************************************/

#include "DBConnector.h"



DBConnector* DBConnector::_instance = nullptr;

DBConnector::DBConnector(){
}
DBConnector::~DBConnector(){
}

DBConnector* DBConnector::getInstance(){
	if (!_instance){
		_instance = new DBConnector();
		_instance->init();
	}
	return _instance;
}
void DBConnector::destroyInstance(){
    if (_instance)
    {
        CC_SAFE_DELETE(_instance);
    }
}

void DBConnector::init(){
	
}
std::string DBConnector::getKey(std::string key){
	return key;
}
DBSqlite* DBConnector::open(std::string name, bool new_link, bool oTeam){
	if (name == "") name = DBSqlite::DB_NAME;
	if (new_link)
		return new DBSqlite(name);
	DBSqlite* db;
	//使用连接池
	std::string key = this->getKey(name);
	if (_dbPool.existKey(name)) {
		db = _dbPool[name];
		return db;
	}
	db = new DBSqlite(name);
	_dbPool.add(name, db);
	return db;
}