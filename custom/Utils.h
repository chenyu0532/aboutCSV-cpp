/****************************************************************************
  
 *	@desc	公用功能
     
 *	@file	
 *  @modify	null
 ******************************************************************************/

#ifndef __DEERUTILS__
#define ___DEERUTILS__
#include "cocos2d.h"
namespace custom{
	/**
	 *  在节点树中按名字来寻找节点
	 *
	 *  @param root 根节点
	 *  @param name 节点名称
	 *
	 *  @return 找到返回对应节点，找不到返回null
	 */
	cocos2d::Node* seekChildByName(cocos2d::Node* root, const std::string& name);

	template <class RET>
	RET seekChildByNameWithRetType(cocos2d::Node* root, const std::string& name)
	{
		auto ret = dynamic_cast<RET>(seekChildByName(root, name));

		CCASSERT(ret, "Can't get Object with the name or the type");

		return ret;
	}
	/**
	 *  根据16进制颜色值获取颜色值color3b
	 *
	 *  @param 16进制颜色值
	 *
	 *  @return 对应的color3b颜色值
	 */
	cocos2d::Color3B hexToColor3B(std::string str);
	/**
	*	计算当前日期多少天后的日期
	*/
	std::string getDaysAfterDay(int day);

}
#endif /* defined(__deer__DeerUtils__) */
