/// \file LibDate.cpp
/// DateTime类实现文件

#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <climits>
#include "LibDate.h"

using namespace std;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#define LOCAL_TIME(_A_ , _B_) localtime_s(_A_, _B_)
#define GMTIME(_A_, _B_) gmtime_s(_A_, _B_);

#else 

#define LOCAL_TIME(_A_ , _B_) localtime_r(_B_, _A_)
#define GMTIME(_A_, _B_) gmtime_r(_B_, _A_);

#endif

// WEB Application Develop Library namaspace
namespace devlib
{
	/// 以当前时间设置对象
	void LibDate::set()
	{
		_time = ::time( 0 );
        LOCAL_TIME(&_tm, &_time);
	}
	
	/// 以 time_t 参数设置对象
	/// \param tt time_t类型参数
	void LibDate::set( const time_t &tt )
	{
		time_t _tt = tt;
		if ( tt < 0 ) _tt = 0;
		if ( tt > LONG_MAX ) _tt = LONG_MAX;
		
		_time = _tt;
        LOCAL_TIME(&_tm, &_time);
	}
	
	/// 以 tm 结构参数设置对象
	/// \param st struct tm类型参数
	void LibDate::set(const tm &st)
	{
		_tm.tm_year = st.tm_year;
		_tm.tm_mon = st.tm_mon;
		_tm.tm_mday = st.tm_mday;
		_tm.tm_hour = st.tm_hour;
		_tm.tm_min = st.tm_min;
		_tm.tm_sec = st.tm_sec;
		_tm.tm_isdst = -1;
		_time = mktime( &_tm );
	}
	
	/// 以指定时间设置对象
	/// 若参数不是有效日期时间,则调整为最接近的日期时间值
	/// \param year 年
	/// \param mon 月
	/// \param mday 日
	/// \param hour 时,默认为0
	/// \param min 分,默认为0
	/// \param src 秒,默认为0
	void LibDate::set( const int year, const int mon, const int mday, 
					const int hour, const int min, const int sec )
	{
		int _year = year;
		int _mon = mon;
		int _mday = mday;
		int _hour = hour;
		int _min = min;
		int _sec = sec;

		// confirm
		if ( _year < 1 ) 	_year = 1;
		if ( _year > 2038 ) _year = 2038;
		if ( _mon < 1 ) 	_mon = 1;
		if ( _mon > 12 ) 	_mon = 12;
		if ( _mday < 1 ) 	_mday = 1;
		if ( _mday > 31 ) 	_mday = 31;
		if ( _hour < 0 ) 	_hour = 0;
		if ( _hour > 23 ) 	_hour = 23;
		if ( _min < 0 ) 	_min = 0;
		if ( _min > 59 ) 	_min = 59;
		if ( _sec < 0 ) 	_sec = 0;
		if ( _sec > 59 ) 	_sec = 59;
		
		_tm.tm_year = _year-1900;
		_tm.tm_mon = _mon-1;
		_tm.tm_mday = _mday;
		_tm.tm_hour = _hour;
		_tm.tm_min = _min;
		_tm.tm_sec = _sec;
		_tm.tm_isdst = -1;
		_time = mktime( &_tm );
	}

	/// 以 Date 参数设置对象
	/// \param date Date类型参数
	void LibDate::set(const LibDate &date)
	{
		set( date.value() );
	}
	
	/// 以"YYYY-MM-DD HH:MM:SS"格式字符串设置对象
	/// 若日期时间字符串格式错误则(尽量)设置为当前时间,
	/// 若日期时间值错误则(尽量)设置为最接近的时间
	/// \param datetime "YYYY-MM-DD HH:MM:SS"格式日期时间字符串
	/// \param datemark 日期分隔字符,默认为"-"
	/// \param dtmark 日期时间分隔字符,默认为" "
	/// \param timemark 时间分隔字符,默认为":"
	void LibDate::set( const string &datetime,
					const string &datemark, 
					const string &dtmark,
					const string &timemark )
	{
		string date, time;
		
		if ( dtmark != "" )
		{
			date = datetime.substr( 0, datetime.find(dtmark) );
			time = datetime.substr( datetime.find(dtmark)+dtmark.length() );
		}
		
		if ( date!="" && time!="" )
		{
			string year, mon, mday, hour, min, sec;
			string::size_type pos1=0, pos2=0, offset=0;
			
			// parse date string
			if ( (pos1=date.find(datemark)) != date.npos )
			{
				offset = datemark.length();
				pos2 = date.find( datemark, pos1+offset );
			}
				
			if ( pos1!=date.npos && pos2!=date.npos && offset>0 )
			{
				
				year = date.substr( 0, pos1 );
				mon = date.substr( pos1+offset, pos2-pos1-offset );
				mday = date.substr( pos2+offset );
			}

			// parse time string
			if ( (pos1=time.find(timemark)) != time.npos )
			{
				offset = timemark.length();
				pos2 = time.find( timemark, pos1+offset );
			}

			if ( pos1!=time.npos && pos2!=time.npos && offset>0 )
			{
				hour = time.substr( 0, pos1 );
				min = time.substr( pos1+offset, pos2-pos1-offset );
				sec = time.substr( pos2+offset );
			}
			
			// set datetime
			if ( year!="" && mon!="" && mday!="" && 
				 hour!="" && min!="" && sec!="" )
			{
				set( atoi(year.c_str()), atoi(mon.c_str()), 
					 atoi(mday.c_str()), atoi(hour.c_str()), 
					 atoi(min.c_str()), atoi(sec.c_str()) );
				return;
			}
		}
		
		// format error
		set();
	}

	/// 输出日期字符串
	/// \param datemark 日期分隔字符,默认为"-"
	/// \param leadingzero 是否补充前置零,默认为是
	/// \return 输出指定格式的日期字符串
	string LibDate::date( const string &datemark, const bool leadingzero ) const
	{
		ostringstream date;
		
		if ( leadingzero )
			date << setfill('0') 
				 << setw(4) << year() << datemark 
				 << setw(2) << month() << datemark 
				 << setw(2) << m_day();
		else
			date << year() << datemark 
				 << month() << datemark 
				 << m_day();
				 
		return string( date.str() );
	}
	
	/// 输出时间字符串
	/// \param timemark 时间分隔字符,默认为":"
	/// \param leadingzero 是否补充前置零,默认为是
	/// \return 输出指定格式的时间字符串
	string LibDate::time( const string &timemark, const bool leadingzero ) const
	{
		ostringstream time;
		
		if ( leadingzero )
			time << setfill('0') 
				 << setw(2) << hour() << timemark 
				 << setw(2) << min() << timemark 
				 << setw(2) << sec();
		else
			time << hour() << timemark 
				 << min() << timemark 
				 << sec();
				 
		return string( time.str() );
	}
	
	/// 输出日期时间字符串
	/// \param datemark 日期分隔字符,默认为"-"
	/// \param dtmark 日期时间分隔字符,默认为" "
	/// \param timemark 时间分隔字符,默认为":"
	/// \param leadingzero 是否补充前置零,默认为是
	/// \return 输出指定格式的日期时间字符串
	string LibDate::datetime( const string &datemark, 
						   const string &dtmark,
						   const string &timemark,
						   const bool leadingzero ) const
	{
		ostringstream datetime;
		datetime << date(datemark,leadingzero) << dtmark 
				 << time(timemark,leadingzero);
		return string( datetime.str() );
	}
	
	/// 输出 GMT 格式日期时间字符串
	/// 主要用于设置 cookie 有效期
	string LibDate::gmt_datetime() const
	{
		char gmt[50];
		struct tm gmt_tm;
		
//		gmtime_s(&gmt_tm, &_time);
        GMTIME(&gmt_tm, &_time);
		strftime( gmt, 50, "%A,%d-%B-%Y %H:%M:%S GMT", &gmt_tm );
		return string( gmt );
	}

	/// 赋值操作
	LibDate& LibDate::operator=(const LibDate &date)
	{
		if ( this == &date ) return *this;
		set( date );
		return *this;	
	}
	/// 赋值操作
	LibDate& LibDate::operator=(const time_t &tt)
	{
		set( tt );
		return *this;
	}

	/// 递增操作
	LibDate& LibDate::operator+=(const LibDate &date)
	{
		set( value() + date.value() );
		return *this;
	}
	/// 递增操作
	LibDate& LibDate::operator+=(const time_t &tt)
	{
		set( value() + tt );
		return *this;
	}
	
	/// 递减操作
	LibDate& LibDate::operator-=(const LibDate &date)
	{
		set( value() - date.value() );
		return *this;
	}
	/// 递减操作
	LibDate& LibDate::operator-=(const time_t &tt)
	{
		set( value() - tt );
		return *this;
	}

	/// 相加操作
	LibDate operator+(const LibDate &date1, const LibDate &date2)
	{
		LibDate newdate;
		newdate.set( date1.value() + date2.value() );
		return newdate;
	}
	/// 相加操作
	LibDate operator+(const LibDate &date, const time_t &tt)
	{
		LibDate newdate;
		newdate.set( date.value() + tt );
		return newdate;
	}
	
	/// 相减操作
	LibDate operator-(const LibDate &date1, const LibDate &date2)
	{
		LibDate newdate;
		newdate.set( date1.value() - date2.value() );
		return newdate;
	}
	/// 相减操作
	LibDate operator-(const LibDate &date, const time_t &tt)
	{
		LibDate newdate;
		newdate.set( date.value() - tt );
		return newdate;
	}

	//--------------------------------------------------------------
	clock_t LibDate::_startTime = 0;
	std::map<int, clock_t> LibDate::_startTimeMap;
	void LibDate::startRunTime(int key){
		_startTimeMap[key] = clock();
	}
	clock_t LibDate::getRunTime(int key){
		clock_t time = clock() - _startTimeMap[key];
		return time;
	}


}
