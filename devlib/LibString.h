/// \file LibString.h
/// String类头文件
/// 继承自std::string的字符串类
/// <a href=std_string.html>基类std::string使用说明文档</a>

#ifndef _WEBDEVLIB_STRING_H_
#define _WEBDEVLIB_STRING_H_ 

#include <sys/stat.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>



// WEB Application Develop Library namaspace
namespace devlib
{
	int custom_snprintf(char* _dest, size_t _count, const char* format, const long i);
	/// long int转换为string
	std::string itos(const long i, const std::ios::fmtflags base = std::ios::dec);
	
	/// string转换为int
	int stoi(const std::string s, const std::ios::fmtflags base = std::ios::dec);
	
	/// double转换为string
	std::string ftos( const double f, const int ndigit = 2 );
	
	/// string转换为double
	double stof( const std::string &s );

	// 空白字符列表
	const char blank_chs[] = " \t\n\r\v\f";

	// CGI敏感字符列表
	const char cgi_senchs[] = "&;`'\\\"|*?~<>^()[]{}$\n\r\t\0#./%";
	
	/// 判断一个双字节字符是否是GBK编码汉字
	bool isgbk( const unsigned char c1, const unsigned char c2 );

	/// 清除两侧空白字符
	void trim( std::string &str );
	
	/// 继承自std::string的字符串类
	/// <a href=std_string.html>基类std::string使用说明文档</a>
	class String : public std::string
	{
		public:
		
		/// 默认构造函数
		String(){}
		
		/// 参数为char*的构造函数
		String( const char *s )
		{
			if( s ) assign( s );
			else erase();
		}
		
		/// 参数为string的构造函数
		String( const std::string &s )
		{
			assign( s );
		}
		
		/// 参数为char的构造函数
		String( const char c )
		{
			assign( 1, c );
		}
		
		/// 析构函数
		virtual ~String(){}
		
		/// 函数String::split()分割结果返回方式
		enum split_resmode
		{
			/// 忽略分割结果中的空子串
			SPLIT_ERASE_BLANK,
			/// 保留分割结果中的空子串
			SPLIT_KEEP_BLANK
		};
		
		/// 函数String::split()切分标记方式
		enum split_splmode
		{
			/// 将切分标记字符串作为完整切分依据
			SPLIT_SINGLE,
			/// 将切分标记字符串作为切分依据序列
			SPLIT_MULTI
		};
		
		/// 函数String::escape_html()转换方式
		enum eschtml_mode
		{
			/// HTML代码转换为转义符
			HTML_TO_ESC,		
			/// 转义符转换为HTML代码
			HTML_TO_TAG			
		};

		/// 返回字符数量，支持全角字符
		std::string::size_type w_length() const;
		
		/// 截取子字符串，支持全角字符
		String w_substr( const std::string::size_type pos = 0, 
						 const std::string::size_type n = npos ) const;

		/// 清除左侧空白字符
		void trim_left( const std::string &blank = blank_chs );
		
		/// 清除右侧空白字符
		void trim_right( const std::string &blank = blank_chs );
		
		/// 清除两侧空白字符
		void trim( const std::string &blank = blank_chs );
		
		/// 从左边截取指定长度子串
		String left( const std::string::size_type n ) const;

		/// 从中间截取指定长度子串
		String mid( const std::string::size_type pos, 
					const std::string::size_type n = npos ) const;

		/// 从右边截取指定长度子串
		String right( const std::string::size_type n ) const;
		
		/// 调整字符串长度
		void resize( const std::string::size_type n );

		/// 统计指定子串出现的次数
		int count( const std::string &str ) const;
		
		/// 根据分割符分割字符串
		std::vector<String> split(const std::string &tag,
							  const int limit = 0, 
							  const split_resmode ret = SPLIT_ERASE_BLANK,
							  const split_splmode spl = SPLIT_SINGLE ) const;
		
		/// 转换字符串为HASH结构(map<string,string>)
		std::map<std::string, std::string> tohash(const std::string &itemtag = "&",
								   const std::string &exptag = "=" ) const;

		/// 组合字符串
		void join(const std::vector<std::string> &strings, const std::string &tag);
		void join(const std::vector<String> &strings, const std::string &tag);

		/// 格式化赋值
		bool sprintf( const char *format, ... );
		
		/// 替换
		int replace( const std::string &oldstr, const std::string &newstr );

		/// 替换
		inline int replace( const std::string &oldstr, const long &i )
		{
			return replace( oldstr, itos(i) );
		}
		
		/// 全文替换
		int replace_all( const std::string &oldstr, const std::string &newstr );

		/// 全文替换
		inline int replace_all( const std::string &oldstr, const long &i )
		{
			return replace_all( oldstr, itos(i) );
		}
		
		/// 转换为大写字母
		void upper();
		
		/// 转换为小写字母
		void lower();
		
		/// 字符串是否完全由数字组成
		bool isnum() const;
		
		/// 读取文件到字符串
		bool load_file( const std::string &filename );
		
		/// 保存字符串到文件
		/*
		bool save_file( const string &filename, 
						const ios::openmode mode = ios::trunc|ios::out,
						const mode_t permission = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH ) const;
		*/
		/// 过滤字符
		void filter( const std::string &filter = cgi_senchs, 
					 const std::string &newstr = "" );

		/// HTML代码转义
		std::string escape_html( const eschtml_mode mode );
		
		/// 将字符串拆分为字符，支持全角字符
		std::vector<std::string> split_char() const;
		
		
		/***********************************************************
		*						New Functions 					   *
		***********************************************************/
		/// 函数String::escape_xml()转换方式
		enum escxml_mode
		{
			/// XML代码转换为转义符
			XML_TO_ESC,		
			/// 转义符转换为XML代码
			XML_TO_TAG			
		};
		
		/// HTML代码转义
		std::string html_escape();

		/// HTML代码转义
		std::string html_unescape();
		
		/// XML代码转义
		std::string escape_xml( const escxml_mode mode );

		/// XML代码转义
		std::string xml_escape();

		/// XML代码转义
		std::string xml_unescape();
		
		/// 忽略大小写查找
		std::string::size_type find_case( const char *str, std::string::size_type pos = 0 ) const;
		
		std::string::size_type rfind_case( const char *str ) const;
	};
}

#endif //_WEBDEVLIB_STRING_H_
