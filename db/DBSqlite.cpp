/****************************************************************************
*	@ 	  
*	@desc	数据库
*	@ 	 
*	@file	db/DBSqlite.cpp
*	@ 	null
******************************************************************************/

#include "DBSqlite.h"
#include "core/CSVParser/CsvParser.h"
#include "core/devlib/LibString.h"

USING_NS_CC;

const std::string DBSqlite::DB_NAME = "save.db";
const std::string DBSqlite::DB_ERR = "databaseError";

DBSqlite::DBSqlite(){
	
}
DBSqlite::DBSqlite(std::string dbName){
	this->setDB(dbName);
}
DBSqlite::~DBSqlite(){

}
bool DBSqlite::setDB(std::string dbName){
    std::string path = FileUtils::getInstance()->getWritablePath() + dbName;
	char* errMsg;
	int res = sqlite3_open(path.c_str(), &_pdb);
	if (res != SQLITE_OK){
		CCLOG("open database failed,  number%d", res);
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(DBSqlite::DB_ERR, &res);
		return false;
	}
	return true;
}
bool DBSqlite::setDB(){
	return this->setDB(DBSqlite::DB_NAME);
}
int isExisted(void * para, int n_column, char ** column_value, char ** column_name)
{
	bool *isExisted_ = (bool*)para;
	*isExisted_ = (**column_value) != '0';
	return 0;
}
bool DBSqlite::exist(std::string tableName){
	bool tableIsExisted;
	char* errMsg;
	std::stringstream sql;
	sql << "select count(type) from sqlite_master where type='table' and name ='" << tableName << "'";
	int res = sqlite3_exec(_pdb, sql.str().c_str(), isExisted, &tableIsExisted, &errMsg);
	return tableIsExisted;
}
void DBSqlite::upgrade(std::string tableName, std::string tableSql){
	std::stringstream sql;
	bool res = false;
	sql << "SELECT TRIM(GROUP_CONCAT(sql, ';')) AS `sql` FROM `sqlite_master` WHERE `tbl_name` = '" << tableName << "' ORDER BY type='index', type='table' LIMIT 1";
	Value one = getOne(sql.str());
	std::string oldSql = one.asString();
	std::string newSql = tableSql;
	if (oldSql == ""){
		query(tableSql);
		return;
	}
	devlib::trim(oldSql);
	devlib::trim(newSql);
	if (oldSql != newSql){
		sql.clear();
		sql.str("");
		sql << "SELECT * FROM `" << tableName << "` LIMIT 100";
		Value all = getAll(sql.str());
		sql.clear();
		sql.str("");
		sql << "DROP TABLE `" << tableName << "`";
		res = query(sql.str());
		res = query(newSql);
		for (Value row : all.asValueVector()){
			sql.clear();
			sql.str("");
			std::stringstream keys;
			std::stringstream values;
			bool first = true;
			for (std::pair<std::string, Value> v : row.asValueMap()){
				if (first){
					first = false;
				}
				else{
					keys << ",";
					values << ",";
				}
				keys << "`" << v.first << "`";
				values << "'" << v.second.asString() << "'";
			}
			sql << "INSERT INTO`" << tableName << "` (" << keys.str() << ") VALUES (" << values.str() << ")";
			res = query(sql.str());
		}
		CCLOG("upgrade success:%s", tableName.c_str());
	}
	sql.clear();
	sql.str("");
}
void DBSqlite::upgrade(std::string tableName, std::string tableSql, std::string dataSql){
	std::stringstream sql;
	bool res = false;
	sql << "SELECT TRIM(GROUP_CONCAT(sql, ';')) AS `sql` FROM `sqlite_master` WHERE `tbl_name` = '" << tableName << "' ORDER BY type='index', type='table' LIMIT 1";
	Value one = getOne(sql.str());
	std::string oldSql = one.asString();
	std::string newSql = tableSql;
	if (oldSql == ""){
		query(tableSql);
		res = query(dataSql);
		return;
	}
	devlib::trim(oldSql);
	devlib::trim(newSql);
	if (oldSql != newSql){
		sql.clear();
		sql.str("");
		sql << "DROP TABLE `" << tableName << "`";
		res = query(sql.str());
		res = query(newSql);
		res = query(dataSql);
		CCLOG("upgrade %s success with data", tableName.c_str());
		return;
	}
	sql.clear();
	sql.str("");
	sql << "SELECT COUNT(*) AS `num` FROM `" << tableName << "` LIMIT 1";
	int num = getOne(sql.str()).asInt();
	int count = 0;
	std::string pattern = "\r\n";
	int pos = 0;
	while (true){
		pos = dataSql.find(pattern, pos);
		if (pos == std::string::npos) break;
		count++;
		pos += (pattern.size() + 1);

	}
	if (num < count){
		res = query(dataSql);
		CCLOG("upgrade %s success with data num diff", tableName.c_str());
	}
}
bool DBSqlite::import(std::string str){
	return true;
}
bool DBSqlite::query(std::string sql){
	char *errMsg;
	int res = sqlite3_exec(_pdb, sql.c_str(), NULL, NULL, &errMsg);
	if (res != SQLITE_OK){
		CCLOG("query failed, number:%d, msg:%s, sql:%s", res, errMsg, sql.c_str());
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(DBSqlite::DB_ERR, &res);
		return false;
	}
	return true;
}
int DBSqlite::insertID(){
	std::string sql = "select last_insert_rowid()";
	int id = this->getOne(sql).asInt();
	return id;
}
Value DBSqlite::getAll(std::string sql){
	char **re;
	int r, c, res;
	char* errMsg;
	ValueVector all;
	res = sqlite3_get_table(_pdb, sql.c_str(), &re, &r, &c, &errMsg);
	if (res != SQLITE_OK){
		CCLOG("select table all failed, number:%d, msg:%s, sql:%s", res, errMsg, sql.c_str());
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(DBSqlite::DB_ERR, &res);
		return Value(all);
	}
	int i, j;
	ValueVector vec;
	for (i = 1; i <= r; i++){
		ValueMap map;
		for (j = 0; j<c; j++){
			std::string k(re[j]);
			Value v(re[i * c + j]);
			map[k] = v;
		}
		vec.push_back(Value(map));
	}
	sqlite3_free_table(re);
	all = vec;
	return Value(all);
}
Value DBSqlite::getRow(std::string sql){
	char **re;
	int r, c, res;
	char* errMsg;
	Value row;
	res = sqlite3_get_table(_pdb, sql.c_str(), &re, &r, &c, &errMsg);
	if (res != SQLITE_OK){
		CCLOG("select table row failed, number:%d, msg:%s, sql:%s", res, errMsg, sql.c_str());
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(DBSqlite::DB_ERR, &res);
		return row;
	}
	ValueMap map;
	for (int j = 0; j<c; j++){
		std::string k(re[j]);
		Value v(re[c + j]);
		map[k] = v;
	}
	row = Value(map);
	return row;
}
Value DBSqlite::getOne(std::string sql){
	char **re;
	int r, c, res;
	char* errMsg;
	res = sqlite3_get_table(_pdb, sql.c_str(), &re, &r, &c, &errMsg);
	Value colmun;
	if (res != SQLITE_OK){
		CCLOG("select table one failed, number:%d, msg:%s, sql:%s", res, errMsg, sql.c_str());
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(DBSqlite::DB_ERR, &res);
		return colmun;
	}
	if (r > 0 && c > 0){
		if(re[1])
			colmun = re[1];
		else
			colmun = "";
	}else{
		colmun = "";
	}
	sqlite3_free_table(re);
	return colmun;
}
