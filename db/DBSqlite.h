/****************************************************************************
*	@ 	  
*	@desc	数据库
*	@file	db/DBSqlite.h
*	@ 	null
*	@detail
*	1.2增加了更新csv数据表的功能
*	@exmple	
*	DBSqlite* db = new DBSqlite(name);
*	bool res = db->query(sql);
******************************************************************************/
#ifndef __DB_SQLITE_H__
#define __DB_SQLITE_H__

#include "cocos2d.h"
//#include "sqlite3/include/sqlite3.h"
//#pragma comment(lib,"sqlite3.lib")
#include "core/sqlite/sqlite3.h"


class DBSqlite {
public:
	/**
	 *	构造
	 */
	DBSqlite();
	DBSqlite(std::string dbName);
	/**
	 *	析构函数，要清除有指针的内容
	 */
	virtual ~DBSqlite();
	/**
	 *	初始化
	 */
	void init();
	/**
	 *	设置数据库
	 *	@param	dbName	数据库名字
	 */
	bool setDB(std::string dbName);
	bool setDB();
	/**
	 *	判断表存在
	 *	@param	tableName	表名
	 */
	bool exist(std::string tableName);
	/**
	 *	升级表
	 *	@param	tableName	表名
	 *	@param	tableSql	建表sql
	 *	@param	dataCSV	默认数据
	 */
	void upgrade(std::string tableName, std::string tableSql);
	void upgrade(std::string tableName, std::string tableSql, std::string dataSql);
	/**
	 *	导入数据
	 */
	bool import(std::string str);
	/**
	 *	执行sql
	 *	@param	sql
	 *	@return	执行结果
	 */
	bool query(std::string sql);
	/**
	 *	获取刚刚插入的id
	 */
	int insertID();
	/**
	 *	获取单独数据
	 *	@param	sql	
	 *	@return	返回数据
	 */
	cocos2d::Value getOne(std::string sql);
	/**
	 *	获取一行数据
	 *	@param	sql
	 *	@return	返回数据
	 */
	cocos2d::Value getRow(std::string sql);
	/**
	 *	获取数组数据
	 *	@param	sql
	 *	@return	返回数据
	 */
	cocos2d::Value getAll(std::string sql);
	/**
	 *	数据库名字
	 */
	static const std::string DB_NAME;

	/**
	 *	数据库错误
	 */
	static const std::string DB_ERR;
	/**
	 *	数据库执行错误
	 */
	static const std::string DB_QUERY_ERR;
	/**
	 *	数据库查询错误
	 */
	static const std::string DB_SELECT_ERR;
private:
	/**
	 *	版本号
	 */
	float VERSION = 1.2;
	/**
	 *	数据库
	 */
	sqlite3* _pdb = NULL;
	/**
	 *	数据库名
	 */
	std::string dbName;
};

#endif // __DB_SQLITE_H__
