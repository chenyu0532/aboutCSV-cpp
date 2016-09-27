/****************************************************************************
*	@ 	  
*	@desc	自定义记忆体
*	@ 	 
*	@file	custom/Caches.cpp
*	@ 	null
******************************************************************************/
#include "Caches.h"
USING_NS_CC;

namespace custom{
	Caches* Caches::_instance = nullptr;
	Caches::Caches(){

	}
	Caches::~Caches(){

	}
	Caches* Caches::getInstance()
	{
		if (!_instance) {
			_instance = new Caches();
		}
		return _instance;
	}
	void Caches::setExpire(int time){
		_expire = time;
	}
	void Caches::setMem(std::string key, void* val){
		_carrier[key] = val;
	}
	void Caches::setMem(int key, void* val){
		std::string strKey = Value(key).asString();
		setMem(strKey, val);
	}
	void* Caches::getMem(std::string key){
		void *val = _carrier[key];
		return val;
	}
	void* Caches::getMem(int key){
		std::string strKey = Value(key).asString();
		return getMem(strKey);
	}
	
}
