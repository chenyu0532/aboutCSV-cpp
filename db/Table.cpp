/****************************************************************************
*	@ 	  
*	@desc	数据表
*	@ 	 
*	@file	db/Table.cpp
*	@ 	null
******************************************************************************/

#include "Table.h"
//#include "external/unzip/unzip.h"
//#include "base/ZipUtils.h"
#include "core/CSVParser/CsvParser.h"

USING_NS_CC;

Table* Table::_instance = nullptr;

Table::Table(){
}
Table::~Table(){

}
Table* Table::getInstance(){
	if (!_instance){
		_instance = new Table();
	}
	return _instance;
}
void Table::setXml(std::string xmlName){
	ValueVector sqlVec = FileUtils::getInstance()->getValueVectorFromFile(xmlName);
	_sqlMap = sqlVec.at(0).asValueMap();
}
void Table::setPrefix(std::string prefix){
	_prefix = prefix;
}
std::string Table::getTableByName(std::string name){
	if (_sqlMap.size() == 0){
		this->setXml(XML_FILE_NAME);
	}auto ttt = name;
	std::string a = _sqlMap.at(name.c_str()).asString();
	return _sqlMap.at(name.c_str()).asString();
}
std::string Table::getCSVByName(std::string name){
	if (_prefix.size() == 0){
		setPrefix(PREFIX);
	}
	std::stringstream path;
	path << _prefix << "/" << name << ".csv";
	std::string str = FileUtils::getInstance()->getStringFromFile(path.str());
	/*
	path << _prefix << "/" << CSV_ZIP_NAME;
	ssize_t size;
	name.append(".csv");
    std::string fullpath = FileUtils::getInstance()->fullPathForFilename(path.str());
	//unsigned char* buff = FileUtils::getInstance()->getFileDataFromZip(rpath, name, &size);
	ZipFile zip(fullpath);
	//zip.setFilter("assets/");
	unsigned char* buff = zip.getFileData(name, &size);
	std::string str;
	if (buff && size){
		str.assign((char*)buff, size);
	}
	*/
	return str;
}
std::string Table::getDataSqlByCSVName(std::string name){
	if (_prefix.size() == 0){
		setPrefix(PREFIX);
	}
	std::stringstream path;
	path << _prefix << "/" << name << ".csv";
	std::string str = FileUtils::getInstance()->getStringFromFile(path.str());
	csv::Parser file = csv::Parser(str, csv::DataType::ePURE);
	std::string dataSql = file.printSql(name);
	return dataSql;
}