/****************************************************************************
*	@ 	  
*	@desc	自定义记忆体
*	@ 	 
*	@file	custom/Memories.cpp
*	@ 	null
******************************************************************************/
#include "Memories.h"
USING_NS_CC;

namespace custom{
	Memories* Memories::_instance = nullptr;
	Memories::Memories(){

	}
	Memories::~Memories(){

	}
	Memories* Memories::getInstance()
	{
		if (!_instance) {
			_instance = new Memories();
		}
		return _instance;
	}
	void Memories::setExpire(int time){
		_memExpire = time;
	}
	void Memories::setMem(std::string key, cocos2d::Value val){
		val.asValueMap()[MARK_UP_KEY] = markUp();
		_memEntity[key] = val;
	}
	void Memories::setMem(int key, cocos2d::Value val){
		std::string strKey = Value(key).asString();
		setMem(strKey, val);
	}
	Value Memories::getMem(std::string key){
		Value &val = _memEntity[key];
		if (val.isNull()) 
			return val;
		ValueMap mark = markUp();
		if (mark["time"].asInt() - val.asValueMap()[MARK_UP_KEY].asValueMap()["time"].asInt() > DEFAULT_MEM_EXPRIRE){
			val = Value();
			return val;
		}
		return val;
	}
	Value Memories::getMem(int key){
		std::string strKey = Value(key).asString();
		return getMem(strKey);
	}
	void Memories::delMem(int key){
		delMem(Value(key).asString());
	}
	void Memories::delMem(std::string key){
		_memEntity.erase(key);
	}
	cocos2d::ValueMap Memories::markUp(){
		ValueMap val = ValueMapNull;
		val["time"] = (int)time(NULL);
		val["from"] = "mem";
		return val;
	}
	
}
