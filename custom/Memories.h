/****************************************************************************
*	@ 	  
*	@desc	自定义记忆体
*	@ 	 
*	@file	custom/Memories.h
*	@ 	null
******************************************************************************/
#ifndef  __MEMORIES_EVENT_H__
#define  __MEMORIES_EVENT_H__

#include "cocos2d.h"
namespace custom{
	class Memories
	{
	public:
		Memories();
		virtual ~Memories();
		/**
		 *	获取实体
		 */
		static Memories* getInstance();
		/**
		 *	设置过期时间
		 */
		void setExpire(int time);
	protected:
		/**
		 *	获取记忆
		 *	@param	key	int格式会改成string格式
		 */
		cocos2d::Value getMem(int key);
		cocos2d::Value getMem(std::string key);
		/**
		 *	设置记忆
		 *	@param	key	int格式会改成string格式
		 *	@param	val	值
		 */
		void setMem(int key, cocos2d::Value val);
		void setMem(std::string key, cocos2d::Value val);

		void delMem(int key);
		void delMem(std::string Key);
	private:
		static Memories* _instance;
		/**
		 *	记忆体
		 */
		cocos2d::ValueMap _memEntity;
		/**
		 *	过期时间
		 */
		int _memExpire;
		/**
		 *	标记
		 */
		cocos2d::ValueMap markUp();
		/**
		 *	标记key
		 */
		const std::string MARK_UP_KEY = "memoriesMarkUp";
		/**
		 *	默认过期事件
		 */
		const int DEFAULT_MEM_EXPRIRE = 3600;
	};
}

#endif // __MEMORIES_EVENT_H__