/****************************************************************************
*	@ 	  
*	@desc	自定义事件
*	@ 	 
*	@file	custom/Event.cpp
*	@ 	null
******************************************************************************/
#include "Event.h"
USING_NS_CC;

namespace custom{
	Event::Event(){
		_evt = new EventDispatcher();
		_evt->setEnabled(true);
	}
	Event::~Event(){
		_evt->release();
		clearEventPool();
	}
	cocos2d::EventListenerCustom* Event::addListener(const std::string &eventName, const std::function<void(EventCustom*)>& callback){
		return _evt->addCustomEventListener(eventName, callback);
	}
	void Event::addListener(cocos2d::EventListener *listener, int prority){
		_evt->addEventListenerWithFixedPriority(listener, prority);
	}
	void Event::addListener(cocos2d::EventListener *listener, Node* node){
		_evt->addEventListenerWithSceneGraphPriority(listener, node);
	}
	void Event::removeListener(const std::string &eventName){
		_evt->removeCustomEventListeners(eventName);
	}
	void Event::removeListener(EventListener* listener){
		if(_evt && listener) _evt->removeEventListener(listener);
	}
	void Event::dispatch(const std::string &eventName, void* param){
		if(_evt) _evt->dispatchCustomEvent(eventName, param);
	}

	void Event::addEventPool(EventListener* listener, Event* e){
		//_eventPool.push_back(listener);
		_eventPool[listener] = e;
	}
	void Event::clearEventPool(){
		for (auto pair : _eventPool){
			if (pair.second&&pair.first)
			{
				pair.second->removeListener(pair.first);
			}
		}
	}
}
