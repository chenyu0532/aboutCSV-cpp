/// \file LibString.cpp
/// String类实现文件

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <set>
#include <fstream>
#include "LibString.h"
#include "cocos2d.h"

using namespace std;

// WEB Application Develop Library namaspace
namespace devlib
{

	int custom_snprintf(char* _dest, size_t _count, const char* format, const long i)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		return _snprintf(_dest, _count, format, i);
#else
		return snprintf(_dest, _count, format, i);
#endif
	}
	/// \page libstring_functions String相关函数列表
	/// \fn <hr><b>string itos( const long i, const ios::fmtflags base )</b>
	/// 
	/// long int转换为string
	/// \param i long int或者int
	/// \param base 转换进制参数,默认为ios::dec(10进制),
	/// 还可以使用ios::oct(8进制),ios::hex(16进制)
	/// \return 返回结果string,转换失败返回"0"
	string itos( const long i, const ios::fmtflags base )
	{
		char format[] = "%ld";
		if ( base == ios::oct )
			strcpy( format, "%o" );
		else if ( base == ios::hex )
			strcpy( format, "%X" );

		// try
		int strlen = 32;
		char *buf = new char[strlen];
		memset( buf, 0, strlen );

		int size = custom_snprintf(buf, strlen, format, i);

		if ( size >= strlen )
		{
			// retry
			delete[] buf;
			buf = new char[size+1];
			memset( buf, 0, size+1 );

			custom_snprintf(buf, size + 1, format, i);
		}
		
		string result( buf );
		delete[] buf;
		return result;
	}

	/// \page libstring_functions String相关函数列表
	/// \fn <hr><b>int stoi( const string &s, const ios::fmtflags base )</b>
	///
	/// string转换为int
	/// \param s string
	/// \param base 转换进制参数,默认为ios::dec(10进制),
	/// 还可以使用ios::oct(8进制),ios::hex(16进制)
	/// \return 返回结果int,转换失败返回0
	int stoi( const string s, const ios::fmtflags base )
	{
		int ibase = 10;
		char *ep;
		
		if ( base == ios::hex )
			ibase = 16;
		else if ( base == ios::oct )
			ibase = 8;
			
		String ps = s;
		ps.trim();
		return strtol( ps.c_str(), &ep, ibase );
     }

	/// \page libstring_functions String相关函数列表
	/// \fn <hr><b>string ftos( const doule f, const int ndigit )</b>
	///
	/// double转换为string
	/// \param f double
	/// \param ndigit 小数点后保留位数,默认为2
	/// \return 转换成功返回string,否则返回"0"
	string ftos( const double f, const int ndigit )
	{
		int fmtlen = 10;
		int strlen = 64;
		int buflen;
		
		// create format string
		char *fmt = new char[fmtlen];
		memset( fmt, 0, fmtlen );

		buflen = custom_snprintf(fmt, fmtlen, "%%.%df", ndigit);

		if ( buflen >= fmtlen )
		{
			delete[] fmt;
			fmt = new char[buflen+1];
			memset( fmt, 0, buflen+1 );

			custom_snprintf(fmt, buflen + 1, "%%.%df", ndigit);

		}
		
		// convert
		char *str = new char[strlen];
		memset( str, 0, strlen );

		buflen = custom_snprintf(str, strlen, fmt, f);

		if ( buflen >= strlen )
		{
			delete[] str;
			str = new char[buflen+1];
			memset( str, 0, buflen+1 );

			custom_snprintf(str, buflen + 1, fmt, f);

		}

		string s = str;
		delete[] fmt;
		delete[] str;
		return s;
	}
	
	/// \page libstring_functions String相关函数列表
	/// \fn <hr><b>double stof( const string &s )</b>
	///
	/// string转换为double
	/// \param s string
	/// \return 转换成功返回double,否则返回0
	double stof( const string &s )
	{
		char *ep;
		return strtod( s.c_str(), &ep );
	}

	/// \page libstring_functions String相关函数列表
	/// \fn <hr><b>bool isgbk( const unsigned char c1, const unsigned char c2 )</b>
	///
	/// 判断一个双字节字符是否是GBK编码汉字
	/// \param c1 双字节字符1
	/// \param c2 双字节字符2
	/// \retval true 是
	/// \retval false 否
	bool isgbk( const unsigned char c1, const unsigned char c2 )
	{
		if ( (c1>=0x81&&c1<=0xFE) && ((c2>=0x40&&c2<=0x7E)||(c2>=0xA1&&c2<=0xFE)) )
			return true;
		else
			return false;
	}
	
	
	/// 清除两侧空白字符
	void trim( string &str )
	{
		String tmpstr = str;
		tmpstr.trim();
		str = tmpstr;
	}
	
	
	
	/// 返回字符数量，汉字算作一个字符
	string::size_type String::w_length() const
	{
		unsigned int wlen = 0;
		unsigned int len = length();
		
		for ( unsigned int i=0; i<len; ++i ) 
		{
			if ( i<(len-1) && isgbk(at(i),at(i+1)) ) 
				++i;
			++wlen;
		}
		
		return wlen;
	}
	
	/// 截取子字符串,避免出现半个汉字
	/// 
	/// 若截取结果的首尾为半个汉字则删除,删除半个汉字后结果可能为空字符串,
	/// 该函数避免在截取时将一个完整汉字分开,对字符串中原有的不完整汉字字符不作处理
	/// \param pos 起始位置,默认为0,单字节计数方式
	/// \param n 要截取的字符串长度,默认为到末尾,单字节计数方式
	/// \return 所截取的字符串
	String String::w_substr( const string::size_type pos, 
							 const string::size_type n ) const
	{
		unsigned int len = length();
		if ( len <= 0 ) return String( "" );
		
		unsigned int from = pos;
		unsigned int to = 0;
		if ( n == npos ) to = len-1;
		else to = pos+n-1;
		
		// location
		for ( unsigned int i=0; i<=to; ++i )
		{
			if ( i<(len-1) && isgbk(at(i),at(i+1)) ) 
			{
				if ( i == from-1 )
					++from;
				else if ( i == to )
					--to;
				++i;
			}
		}

		// substr
		if ( to > from )
			return String( substr(from,to-from+1) );
		else
			return String( "" );
	}
	
	/// 清除左侧空白字符
	/// \param blank 要过滤掉的空白字符列表,默认为webdevlib::blank_chs
	void String::trim_left( const string &blank )
	{
		while ( length()>0 && blank.find(at(0))!=npos )
			erase( 0, 1 );
	}
	
	/// 清除右侧空白字符
	/// \param blank 要过滤掉的空白字符列表,默认为webdevlib::blank_chs
	void String::trim_right( const string &blank )
	{
		while ( length()>0 && blank.find(at(length()-1))!=npos )
			erase( length()-1, 1 );
	}
	
	/// 清除两侧空白字符
	/// \param blank 要过滤掉的空白字符列表,默认为webdevlib::blank_chs
	void String::trim( const string &blank )
	{
		trim_left( blank );
		trim_right( blank );
	}
	
	/// 从左边截取指定长度子串
	/// \param n 要截取的字符串长度,若长度超出则返回原字符串
	/// \return 所截取的字符串
	String String::left( const string::size_type n ) const
	{
		String s = *this;
		unsigned int len = s.length();
		len = ( n>len ) ? len : n;
		return String( s.substr(0,len) );
	}

	/// 从中间截取指定长度子串
	/// \param pos 开始截取的位置
	/// \param n 要截取的字符串长度,若长度超出则返回原字符串,默认为到末尾
	/// \return 所截取的字符串
	String String::mid( const string::size_type pos, 
						const string::size_type n ) const
	{
		String s = *this;
		if ( pos > s.length() )	
			return String( "" );
		return String( s.substr(pos,n) );
	}		

	/// 从右边截取指定长度子串
	/// \param n 要截取的字符串长度,若长度超出则返回原字符串
	/// \return 所截取的字符串
	String String::right( const string::size_type n ) const
	{
		String s = *this;
		unsigned int len = s.length();
		len = ( n>len )? n : len;
		return String( s.substr(len-n,n) );
	}		
	
	/// 调整字符串长度
	/// \param n 新字符串长度,若小于当前长度则截断,若大于当前长度则补充空白字符
	void String::resize( const string::size_type n )
	{
		unsigned int len = length();
		if ( n < len )
		{
			substr( 0, n );
		}
		else if ( n > len )
		{
			for ( unsigned int i=0; i<(n-len); ++i )
				append( " " );
		}
	}

	/// 统计指定子串出现的次数
	/// \param str 要查找的子串
	/// \return 子串不重复出现的次数
	int String::count( const string &str ) const
	{
		string::size_type pos = 0;
		unsigned int count = 0;
		unsigned int step = str.length();
		
		while( (pos=find(str,pos)) != string::npos )
		{
			++count;
			pos += step;
		}
		
		return count;
	}
		
	/// 根据分割符分割字符串
	/// \param tag 分割标记字符串
	/// \param limit 分割次数限制,默认为0即不限制
	/// \param ret 结果返回模式,默认为String::SPLIT_ERASE_BLANK
	/// \param spl 切分模式,默认为String::SPLIT_SINGLE,
	/// 切分模式为SPLIT_SINGLE时将参数tag视为一个完整的切分依据字符串,
	/// 切分模式为SPLIT_MULTI时将参数tag视为一个切分字符串序列,
	/// 包含其中任何一个半角字符都将执行切分动作
	/// \return 分割结果字符串数组 vector<String>
	vector<String> String::split( const string &tag, 
								  const int limit, 
								  const split_resmode ret,
								  const split_splmode spl ) const
	{
		string src = *this;
		string curelm;
		vector<String> list;
		int count = 0;

		list.clear();
		if ( tag.length()>0 && src.length()>0 )
		{
			// how to split
			unsigned int pos = 0;
			if ( spl == SPLIT_SINGLE ) 
				pos = src.find( tag );			// single split
			else 
				pos = src.find_first_of( tag );	// multi split
			
			while ( pos < src.length() )
			{
				curelm = src.substr( 0, pos );
				
				// how to return
				if ( !(ret==SPLIT_ERASE_BLANK && curelm.length()==0) )
				{
					list.push_back( curelm );
					++count;
				}

				// how to split
				if ( spl == SPLIT_SINGLE )
				{
					// single split
					src = src.substr( pos + tag.length() );
					pos = src.find( tag );
				}
				else 
				{
					// multi split
					src = src.substr( pos+1 );
					pos = src.find_first_of( tag );
				}
				
				if ( limit>0 && count>=limit )
					break;
			}
			
			// how to return
			if ( !(ret==SPLIT_ERASE_BLANK && src.length()==0) )
				list.push_back( src );
		}
		
		return list;
	}
	
	/// 转换字符串为HASH结构(map<string,string>)
	/// \param itemtag 表达式之间的分隔符,默认为"&"
	/// \param exptag 表达式中变量名与变量值之间的分隔符,默认为"="
	/// \return 转换结果 map<string,string>
	map<string,string> String::tohash( const string &itemtag, 
									   const string &exptag ) const
	{
		map<string,string> hashmap;
		
		if ( itemtag!="" && exptag!="" )
		{
			vector<String> items = split( itemtag );
			string name, value;
			for ( unsigned int i=0; i< items.size(); ++i )
			{
				name = (items[i]).substr( 0, (items[i]).find(exptag) );
				value = (items[i]).substr( (items[i]).find(exptag)+exptag.length() );
				if ( name != "" )
					hashmap[name] = value;
			}
		}
		
		return hashmap;
	}

	/// 组合字符串,与split()相反
	/// \param strings 字符串数组
	/// \param tag 组合分隔符
	void String::join( const vector<string> &strings, const string &tag )
	{
		if ( strings.size() > 0 )
		{
			erase();
			*this = strings[0];
			
			for ( unsigned int i=1; i<strings.size(); ++i )
				*this += ( tag + strings[i] );
		}
	}
	
	// for vector<String>
	void String::join( const vector<String> &strings, const string &tag )
	{
		if ( strings.size() > 0 )
		{
			erase();
			*this = strings[0];
			
			for ( unsigned int i=1; i<strings.size(); ++i )
				*this += ( tag + strings[i] );
		}
	}

	/// 格式化赋值
	/// 各参数定义与标准sprintf()函数完全相同
	/// \retval true 执行成功
	/// \retval false 失败
	bool String::sprintf( const char *format, ... )
	{
		va_list ap;
		int strlen = 256;

		// alloc memery
		char *buf = new char[strlen];
		memset( buf, 0, strlen );
		
		va_start( ap, format );
		
		// try
		int size = vsnprintf( buf, strlen, format, ap );
		if ( size >= strlen )
		{
			// retry
			delete[] buf;
			buf = new char[size+1];
			memset( buf, 0, size+1 );
			
			/*
			这儿之前是一个bug，在64-bit的机器上会出错，2010-08-03修复
			*/
			va_end( ap );
			va_start( ap, format );
			vsnprintf( buf, size+1, format, ap );
		}
		
		va_end( ap );
		
		// free
		*this = buf;
		delete[] buf;
		return true;
	}

	/// 替换
	/// 该函数重载了std::string::replace()
	/// \param oldstr 被替换掉的字符串
	/// \param newstr 用来替换旧字符串的新字符串
	/// \retval 1 替换成功
	/// \retval 0 失败
	int String::replace( const string &oldstr, const string &newstr )
	{
		string::size_type pos = 0;
		if ( oldstr!="" && (pos=find(oldstr)) != string::npos )
		{
			string::replace( pos, oldstr.length(), newstr );
			return 1;
		}
		return 0;
	}
	
	/// 全文替换
	/// \param oldstr 被替换掉的字符串
	/// \param newstr 用来替换旧字符串的新字符串
	/// \return 执行替换的次数
	int String::replace_all( const string &oldstr, const string &newstr )
	{
		if ( oldstr == "" )
			return 0;
		
		int i = 0;
		string::size_type pos = 0;
		unsigned int curpos = 0;
		while ( (pos=find(oldstr,curpos)) != string::npos )
		{
			string::replace( pos, oldstr.length(), newstr );
			curpos = pos + newstr.length();
			++i;
		}
		return i;
	}
	
	/// 转换为大写字母
	void String::upper()
	{
		for( unsigned int i=0; i<length(); i++ )
			(*this)[i] = toupper( (*this)[i] );
	}
	
	/// 转换为小写字母
	void String::lower()
	{
		for( unsigned int i=0; i<length(); i++ )
			(*this)[i] = tolower( (*this)[i] );
	}

	/// 字符串是否完全由数字组成
	/// \retval true 是
	/// \retval false 否
	bool String::isnum() const
	{
		if ( length() == 0 )
			return false;
			
		for ( unsigned int i=0; i<length(); ++i )
		{
			if ( !isdigit((*this)[i]) )
				return false;
		}
		return true;
	}

	/// 读取文件到字符串
	/// \param filename 要读取的文件完整路径名称
	/// \retval true 读取成功
	/// \retval false 失败
	bool String::load_file( const string &filename )
	{
		FILE *fp = fopen( filename.c_str(), "rb" );
		if ( fp == NULL ) return false;
		
		// read file size
		fseek( fp, 0, SEEK_END );
		int bufsize = ftell( fp );
		rewind( fp );
		
		char *buf = new char[bufsize+1];
		memset( buf, 0, bufsize+1 );
		fread( buf, 1, bufsize, fp );
		fclose( fp );
		*this = buf;
		delete[] buf;
		return true;
	}
	
	/// 保存字符串到文件
	/// \param filename 要写入的文件路径名称
	/// \param mode 写入方式,默认为ios::trunc|ios::out
	/// \param permission 文件属性参数，默认为0666
	/// \retval true 写入成功
	/// \retval false 失败
	/*
	bool String::save_file( const string &filename, 
							const ios::openmode mode,
							const mode_t permission ) const
	{
		ofstream outfile( filename.c_str(), mode );
		if ( outfile )
		{
			outfile << *this;
			outfile.close();
			
			// chmod
			mode_t mask = umask( 0 );
			chmod( filename.c_str(), permission );
			umask( mask );
			
			return true;
		}
		return false;
	}
	*/
	/// 过滤字符
	/// \param filter 要过滤掉的字符列表（支持全角字符）,默认为webdevlib::cgi_senchs
	/// \param newstr 替换字符串,默认为空字符串
	void String::filter( const string &filter, const string &newstr )
	{
		string res;
		
		// split filter
		unsigned int i;
		String fstr = filter;
		vector<string> flist = fstr.split_char();
		set<string> fset;
		for( i=0; i<flist.size(); ++i )
			fset.insert( flist[i] );
		
		// split self
		vector<string> words = split_char();
		for( i=0; i<words.size(); ++i )
		{
			// filter
			if ( fset.find(words[i]) != fset.end() )
				res += newstr;
			else
				res += words[i];
		}
		
		*this = res;
	}
	
	/// HTML代码转义
	/// \param mode 转换模式,
	/// String::HTML_TO_ESC HTML代码转换为转义符,
	/// String::HTML_TO_TAG 转义符转换为HTML代码
	/// \return 转换结果字符串
	string String::escape_html( const eschtml_mode mode )
	{
		String s = *this;
		
		if ( mode == HTML_TO_ESC )
		{
			s.replace_all( "&", "&amp;" );
			s.replace_all( "\"", "&quot;" );
			s.replace_all( "<", "&lt;" );
			s.replace_all( ">", "&gt;" );
		}
		else if ( mode == HTML_TO_TAG )
		{
			s.replace_all( "&amp;", "&" );
			s.replace_all( "&quot;", "\"" );
			s.replace_all( "&lt;", "<" );
			s.replace_all( "&gt;", ">" );
		}
		
		return s;
	}
	
	/// 将字符串拆分为字符序列，支持全角字符
	/// \return 返回字符序列 vector<string>
	vector<string> String::split_char() const
	{
		unsigned int len = length();
		vector<string> chs;
		string word;
		char ch[3];
		
		for ( unsigned int i=0; i<len; ++i ) 
		{
			if ( i<(len-1) && isgbk(at(i),at(i+1)) ) 
			{
				// double byte char
				ch[0] = at(i);
				ch[1] = at(++i);
				ch[2] = '\0';
				word = ch;
			}
			else
			{
				// single byte char
				word = at(i);
			}
				
			chs.push_back( word );
		}
		
		return chs;
	}
	
	
	/***********************************************************
	*						New Functions 					   *
	***********************************************************/
	/// HTML代码转义
	/// \return 转换结果字符串
	string String::html_escape()
	{
		return escape_html(HTML_TO_ESC);
	}

	/// HTML代码转义
	/// \return 转换结果字符串
	string String::html_unescape()
	{
		return escape_html(HTML_TO_TAG);
	}
	
	/// XML代码转义
	/// \param mode 转换模式,
	/// String::XML_TO_ESC XML代码转换为转义符,
	/// String::XML_TO_TAG 转义符转换为XML代码
	/// \return 转换结果字符串
	string String::escape_xml( const escxml_mode mode )
	{
		String s = *this;
		
		if ( mode == XML_TO_ESC )
		{
			s.replace_all( "&", "&amp;" );
			s.replace_all( "'", "&apos;" );
			s.replace_all( "\"", "&quot;" );
			s.replace_all( "<", "&lt;" );
			s.replace_all( ">", "&gt;" );
		}
		else if ( mode == XML_TO_TAG )
		{
			s.replace_all( "&lt;", "<" );
			s.replace_all( "&gt;", ">" );
			s.replace_all( "&apos;", "'" );
			s.replace_all( "&quot;", "\"" );
			s.replace_all( "&amp;", "&" );
		}
		
		return s;
	}
	
	/// XML代码转义
	/// \return 转换结果字符串
	string String::xml_escape()
	{
		return escape_xml(XML_TO_ESC);
	}

	/// XML代码转义
	/// \return 转换结果字符串
	string String::xml_unescape()
	{
		return escape_xml(XML_TO_TAG);
	}
	
	/// 忽略大小写查找
	string::size_type String::find_case( const char *str, string::size_type pos ) const
	{
		unsigned int str_len = strlen(str);
		
		if ( str_len <= 0 || str_len > length() )
			return string::npos;
		
		for ( string::size_type i = pos; i <= length()-str_len ; i++ )
		{
			for ( unsigned int j = 0; j < str_len; j++ )
			{
				if ( tolower(at(i+j)) != tolower( *(str+j) ) )
					break;
				else if ( j == str_len - 1 )
					return i;		// found
			}
		}
		
		return string::npos;	
	}
	
	string::size_type String::rfind_case( const char *str ) const
	{
		int i;
		unsigned int j;
		unsigned int str_len = strlen(str);
		
		if ( str_len <= 0 || str_len > length() )
			return string::npos;
		
		for ( i = length()- str_len; i >= 0; i-- )
		{
			for ( j = 0; j < str_len; j++ )
			{
				if ( tolower(at(i+j)) != tolower( *(str+j) ) )
					break;
				else if ( j == str_len - 1 )
					return i;		// found
			}
		}	
		
		return string::npos;	
	}
	
}
