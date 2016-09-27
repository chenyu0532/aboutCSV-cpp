/****************************************************************************
*	@ 	  
*	@desc	数据表
*	@ 	 
*	@file	db/Table.h
*	@ 	null
******************************************************************************/

#ifndef __DB_TABLE_H__
#define __DB_TABLE_H__

#include "cocos2d.h"

class Table {
public:
	/**
	 *	初始化
	 */
	Table();
	/**
	 *	析构函数，要清除有指针的内容
	 */
	virtual ~Table();
	/**
	 *	设置xml文件
	 */
	void setXml(std::string xmlName);
	/**
	 *	设置前缀
	 */
	void setPrefix(std::string prefix);
	/**
	 *	获取实体
	 */
	static Table* getInstance();
	/**
	 *	通过表名获取表
	 *	@param	name	表名
	 *	@return	建表的sql语句
	 */
	std::string getTableByName(std::string name);
	/**
	 *	通过表名获取数据
	 *	@param	name	表名
	 *	@return	字符串
	 */
	std::string getCSVByName(std::string name);
	/**
	 *	通过表名获取sql数据
	 *	@param	name	表名
	 *	@return	字符串
	 */
	std::string getDataSqlByCSVName(std::string name);
private:
	/**
	 *	自身
	 */
	static Table* _instance;
	/**
	 *	sqlmap
	 */
	cocos2d::ValueMap _sqlMap;
	/**
	 *	前缀
	 */
	std::string _prefix;
	/**
	 *	xml文件名
	 */
	const std::string XML_FILE_NAME = "sql/sql.xml";
	/**
	 *	压缩包
	 */
	const std::string CSV_ZIP_NAME = "csv.zip";
	/**
	 *	前缀
	 */
	const std::string PREFIX = "sql";
};

#endif // __DB_TABLE_H__
