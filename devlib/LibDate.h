/// \file LibDate.h
/// DateTime类头文件
/// 日期时间运算

#ifndef _WEBDEVLIB_DATE_H_
#define _WEBDEVLIB_DATE_H_ 

#include <string>
#include <ctime>
#include <map>
#include "cocos2d.h"

using namespace std;

// WEB Application Develop Library namaspace
namespace devlib
{
	/// \page vars 全局变量列表
	/// \var <b>one_sec</b> 时长定义一秒钟
	const time_t one_sec = 1;

	/// \page vars 全局变量列表
	/// \var <b>one_min</b> 时长定义一分钟
	const time_t one_min = 60;

	/// \page vars 全局变量列表
	/// \var <b>one_hour</b> 时长定义一小时
	const time_t one_hour = 60*60;

	/// \page vars 全局变量列表
	/// \var <b>one_day</b> 时长定义一天
	const time_t one_day = 60*60*24;

	/// \page vars 全局变量列表
	/// \var <b>one_week</b> 时长定义一周
	const time_t one_week = 60*60*24*7;


	/// DateTime类
	class LibDate
	{
		private:
		time_t _time;
		struct tm _tm;

		
		
		public:
		
		/// 默认构造函数,以当前时间构造对象
		LibDate()
		{
			set();
		}
		
		/// 参数为 time_t 的构造函数
		LibDate(const time_t &tt)
		{
			set( tt );
		}
		
		/// 参数为 tm 结构的构造函数
		LibDate(const tm &st)
		{
			set( st );
		}
		
		/// 参数为指定时间的构造函数
		LibDate(const int year, const int mon, const int mday,
			  const int hour=0, const int min=0, const int sec=0 )
		{
			set( year, mon, mday, hour, min, sec );
		}
		
		/// 参数为"YYYY-MM-DD HH:MM:SS"格式字符串的构造函数
		LibDate(const string &datetime,
			  const string &datemark = "-", 
			  const string &dtmark = " ",
			  const string &timemark = ":" )
		{
			set( datetime, datemark, dtmark, timemark );
		}

		/// 拷贝构造函数
		LibDate(const LibDate &date)
		{
			set( date );
		}
			  
		/// 析构函数
		~LibDate() {}
		
		/// 赋值操作
		LibDate& operator=(const LibDate &date);
		/// 赋值操作
		LibDate& operator=(const time_t &tt);

		/// 递增操作
		LibDate& operator+=(const LibDate &date);
		/// 递增操作
		LibDate& operator+=(const time_t &tt);
		
		/// 递减操作
		LibDate& operator-=(const LibDate &date);
		/// 递减操作
		LibDate& operator-=(const time_t &tt);

		/// 返回四位数年份
		inline int year() const {return _tm.tm_year+1900;}
		/// 返回月份
		inline int month() const {return _tm.tm_mon+1;}
		/// 返回当月天数
		inline int m_day() const {return _tm.tm_mday;}
		/// 返回当周天数
		inline int w_day() const {return _tm.tm_wday;}
		/// 返回当年天数
		inline int y_day() const {return _tm.tm_yday;}
		/// 返回小时
		inline int hour() const {return _tm.tm_hour;}
		/// 返回分钟
		inline int min() const {return _tm.tm_min;}
		/// 返回秒数
		inline int sec() const {return _tm.tm_sec;}
		
		/// 返回 1970-1-1 0:0:0 以来的秒数
		inline long secs() const {return _time;}
		/// 返回 1970-1-1 0:0:0 以来的分钟数
		inline long mins() const {return ( _time/one_min );}
		/// 返回 1970-1-1 0:0:0 以来的小时数
		inline long hours() const {return ( _time/one_hour );}
		/// 返回 1970-1-1 0:0:0 以来的天数
		inline long days() const {return ( _time/one_day );}
		/// 返回 1970-1-1 0:0:0 以来的周数
		inline long weeks() const {return ( _time/one_week );}
		
		/// 以当前时间设置对象
		void set();
		/// 以 time_t 参数设置对象
		void set( const time_t &tt );
		/// 以 tm 结构参数设置对象
		void set( const tm &st );
		/// 以指定时间设置对象
		void set( const int year, const int mon, const int mday, 
				  const int hour=0, const int min=0, const int sec=0 );
		/// 以 Date 参数设置对象
		void set(const LibDate &date);
		/// 以"YYYY-MM-DD HH:MM:SS"格式字符串设置对象
		void set( const string &datetime,
				  const string &datemark = "-", 
				  const string &dtmark = " ",
				  const string &timemark = ":" );
		
		/// 返回 time_t 类型的对象值
		inline time_t value() const {return secs();}
		/// 返回 struct tm 类型的对象值
		inline tm struct_tm() const {return _tm;}
		
		/// 输出日期字符串
		string date( const string &datemark = "-", 
					 const bool leadingzero = true ) const;
					 
		/// 输出时间字符串
		string time( const string &timemark = ":", 
					 const bool leadingzero = true ) const;
					 
		/// 输出日期时间字符串
		string datetime( const string &datemark = "-", 
						 const string &dtmark = " ",
						 const string &timemark = ":",
						 const bool leadingzero = true ) const;
						 
		/// 输出 GMT 格式日期时间字符串
		string gmt_datetime() const;

		//-------------------------- 执行时间 ---------------------------------
		private:
			/**
			*	起始时间
			*/
			static clock_t _startTime;
			static std::map<int, clock_t> _startTimeMap;
		public:
			/**
			*	起始
			*/
			static void startRunTime(int key);
			static void startRunTime(){
				startRunTime(0);
			}
			/**
			*	获取执行时间
			*/
			static clock_t getRunTime(int key);
			static clock_t getRunTime(){
				return getRunTime(0);
			}
	};
	
	// operators
	
	/*!
	\page fns 全局函数列表
	<hr>Date 操作<br><br>
	\fn <b>Date operator+( const Date &date1, const Date &date2 )</b><br>
	时间相加<br><br>
	\fn <b>Date operator+( const Date &date, const time_t &tt )</b><br>
	时间相加<br><br>
	\fn <b>Date operator-( const Date &date1, const Date &date2 )</b><br>
	时间相减<br><br>
	\fn <b>Date operator-( const Date &date, const time_t &tt )</b><br>
	时间相减<br><br>
	\fn <b>bool operator==( const Date &left, const Date &right )</b><br>
	时间相等比较<br><br>
	\fn <b>bool operator==( const Date &left, const time_t &right )</b><br>
	时间相等比较<br><br>
	\fn <b>bool operator!=( const Date &left, const Date &right )</b><br>
	时间不相等比较<br><br>
	\fn <b>bool operator!=( const Date &left, const time_t &right )</b><br>
	时间不相等比较<br><br>
	\fn <b>bool operator>( const Date &left, const Date &right )</b><br>
	时间大于比较<br><br>
	\fn <b>bool operator>( const Date &left, const time_t &right )</b><br>
	时间大于比较<br><br>
	\fn <b>bool operator<( const Date &left, const Date &right )</b><br>
	时间小于比较<br><br>
	\fn <b>bool operator<( const Date &left, const time_t &right )</b><br>
	时间小于比较<br><br>
	\fn <b>bool operator>=( const Date &left, const Date &right )</b><br>
	时间不小于比较<br><br>
	\fn <b>bool operator>=( const Date &left, const time_t &right )</b><br>
	时间不小于比较<br><br>
	\fn <b>bool operator<=( const Date &left, const Date &right )</b><br>
	时间不大于比较<br><br>
	\fn <b>bool operator<=( const Date &left, const time_t &right )</b><br> 
	时间不大于比较
	*/
	
	/// 相加操作
	LibDate operator+(const LibDate &date1, const LibDate &date2);
	/// 相加操作
	LibDate operator+(const LibDate &date, const time_t &tt);
	
	/// 相减操作
	LibDate operator-(const LibDate &date1, const LibDate &date2);
	/// 相减操作
	LibDate operator-(const LibDate &date, const time_t &tt);

	///	判断相等操作
	inline bool operator==(const LibDate &left, const LibDate &right)
	{
		return ( left.value() == right.value() );
	}
	/// 判断相等操作
	inline bool operator==(const LibDate &left, const time_t &right)
	{
		return ( left.value() == right );
	}

	///	判断不相等操作
	inline bool operator!=(const LibDate &left, const LibDate &right)
	{
		return ( left.value() != right.value() );
	}
	/// 判断不相等操作
	inline bool operator!=(const LibDate &left, const time_t &right)
	{
		return ( left.value() != right );
	}

	///	判断大于操作
	inline bool operator>(const LibDate &left, const LibDate &right)
	{
		return ( left.value() > right.value() );
	}
	/// 判断大于操作
	inline bool operator>(const LibDate &left, const time_t &right)
	{
		return ( left.value() > right );
	}

	///	判断小于操作
	inline bool operator<(const LibDate &left, const LibDate &right)
	{
		return ( left.value() < right.value() );
	}
	/// 判断小于操作
	inline bool operator<(const LibDate &left, const time_t &right)
	{
		return ( left.value() < right );
	}

	///	判断不小于操作
	inline bool operator>=(const LibDate &left, const LibDate &right)
	{
		return ( left.value() >= right.value() );
	}
	/// 判断不小于操作
	inline bool operator>=(const LibDate &left, const time_t &right)
	{
		return ( left.value() >= right );
	}

	///	判断不大于操作
	inline bool operator<=(const LibDate &left, const LibDate &right)
	{
		return ( left.value() <= right.value() );
	}
	/// 判断不大于操作
	inline bool operator<=(const LibDate &left, const time_t &right)
	{
		return ( left.value() <= right );
	}



}

#endif //_WEBDEVLIB_DATE_H_
