/****************************************************************************
 *	@ 	  
 *	@desc	公用功能
 *	@file	
 *  @ 	null
 ******************************************************************************/

#include "Utils.h"

namespace custom{

	cocos2d::Node* seekChildByName(cocos2d::Node* root, const std::string& name)
	{
		if (!root)
		{
			return nullptr;
		}
		if (root->getName() == name)
		{
			return root;
		}
		const auto& arrayRootChildren = root->getChildren();
		for (auto& subNode : arrayRootChildren)
		{
			auto child = dynamic_cast<cocos2d::Node*>(subNode);
			if (child)
			{
				auto res = seekChildByName(child, name);
				if (res != nullptr)
				{
					return res;
				}
			}
		}
		return nullptr;
	}

	cocos2d::Color3B hexToColor3B(std::string str)
	{
		std::string s1 = str.substr(1, 2);
		int r = (int)strtol(s1.c_str(), NULL, 16);
		s1 = str.substr(3, 2);
		int g = (int)strtol(s1.c_str(), NULL, 16);
		s1 = str.substr(5, 2);
		int b = (int)strtol(s1.c_str(), NULL, 16);
		cocos2d::Color3B color = cocos2d::Color3B(r, g, b);
		return color;
	}
	std::string getDaysAfterDay(int day)
	{
		time_t now;
		now = time(NULL);
		now = now + day * 24 * 60 * 60;
		struct tm *local;
		local = localtime(&now);
		char show_time[20];
		strftime(show_time, 20, "%Y%m%d", local);
		return show_time;
	}
}