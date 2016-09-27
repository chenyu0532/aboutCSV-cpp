/****************************************************************************
*	@ 	  
*	@desc	自定义事件
*	@ 	 
*	@file	custom/Event.h
*	@ 	null
******************************************************************************/
#ifndef  __CUSTOM_EVENT_H__
#define  __CUSTOM_EVENT_H__

#include "cocos2d.h"
namespace custom{
	class Event
	{
	public:
		Event();
		virtual ~Event();
		//----------------- 作为被调用 ----------------------
		/**
		*	添加事件
		*/
		cocos2d::EventListenerCustom* addListener(const std::string &eventName, const std::function<void(cocos2d::EventCustom*)>& callback);
		void addListener(cocos2d::EventListener *listener, int prority = 1);
		void addListener(cocos2d::EventListener *listener, cocos2d::Node* node);
		/*
		*	移除监听
		*/
		void removeListener(const std::string &eventName);
		void removeListener(cocos2d::EventListener *listener);
		//----------------- 作为调用 ----------------------
		/**
		 *	添加入事件池
		 */
		void addEventPool(cocos2d::EventListener* listener, custom::Event* e);
		/**
		 *	清空事件池
		 */
		void clearEventPool();
	protected:
		/**
		 *	发送事件
		 */
		void dispatch(const std::string &eventName, void* param);
	private:
		/**
		*	事件分发器
		*/
		cocos2d::EventDispatcher* _evt;
		/**
		 *	时间池
		 */
		std::map<cocos2d::EventListener*, Event*> _eventPool;
	};
}

#endif // __CUSTOM_EVENT_H__