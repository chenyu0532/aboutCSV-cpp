/// \file LibCode.h
/// 编码,加解密函数头文件
/// BASE64编码,字符串加密解密,URI编码函数
   
#ifndef _WEBDEVLIB_CODE_H_
#define _WEBDEVLIB_CODE_H_ 

#include <string>


// WEB Application Develop Library namaspace
namespace devlib
{
  /// Base64编码
	std::string base64_encode(const std::string& src);
	std::string base64_encode(const char *src, int src_len);
  
  /// Base64解码
	std::string base64_decode(const std::string &src);
	std::string base64_decode(const char *src, int src_len);
  
  /// 无双字符串加密
	std::string vogso_encrypt(const std::string &source);
  
  /// 无双字符串解密
	std::string vogso_decrypt(const std::string &source);

  /// URI编码
	std::string uri_encode(const std::string &source);
  
  /// URI编码 (对全部字符进行编码)
	std::string uri_encode_new(const std::string &source);
  
  /// URI解码
	std::string uri_decode(const std::string &source);
}

#endif //_WEBDEVLIB_CODE_H_
