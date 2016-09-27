/****************************************************************************
*	@ 	  
*	@desc	自定义记忆体
*	@date	2015-5-21
*	@author	110101
*	@ 	 
*	@file	custom/Caches.h
*	@ 	null
******************************************************************************/
#ifndef  __CUSTOM_CACHES_H__
#define  __CUSTOM_CACHES_H__

#include "cocos2d.h"
namespace custom{
	class Caches
	{
	public:
		Caches();
		virtual ~Caches();
		/**
		 *	获取实体
		 */
		static Caches* getInstance();
		/**
		 *	设置过期时间
		 */
		void setExpire(int time);
	protected:
		/**
		 *	获取记忆
		 *	@param	key	int格式会改成string格式
		 */
		void* getMem(int key);
		void* getMem(std::string key);
		/**
		 *	设置记忆
		 *	@param	key	int格式会改成string格式
		 *	@param	val	值
		 */
		void setMem(int key, void* val);
		void setMem(std::string key, void* val);
	private:
		static Caches* _instance;
		/**
		 *	载体
		 */
		std::map<std::string, void*> _carrier;
		/**
		 *	过期时间
		 */
		int _expire;
		/**
		 *	标记key
		 */
		const std::string MARK_UP_KEY = "cachesMarkUp";
		/**
		 *	默认过期事件
		 */
		const int DEFAULT_EXPRIRE = 3600;
	};
}

#endif // __CUSTOM_CACHES_H__