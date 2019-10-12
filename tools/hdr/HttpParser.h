#ifndef _HTTPPARSER_H_
#define _HTTPPARSER_H_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"
#include "InputPacket.h"
#include "IPDRuleMgr.h"
#include "SuperMatch.h"
#include "HashTableMgr.h"
#include "Timer.h"
#include "picohttpparser.h"
#include "StrProc.h"

namespace NS_HTTPPARSER
{
	//host
	static const std::string HOST = "Host";
	//cookie
	static const std::string COOKIE = "Cookie";
	//Content-Type
	static const std::string CONTENT_TYPE = "Content-Type";

	//请求方法枚举
	typedef enum HTTPMETHOD
	{
		EM_METHOD_START = -1,              //枚举保护
		EM_DELETE = 0,                     //DELETE请求方法 
		EM_GET,	                           //GET请求方法
		EM_HEAD,                           //HEAD请求方法
		EM_POST,                           //POST请求方法
		EM_PUT,                            //PUT请求方法
		EM_CONNECT,                        //CONNECT请求方法
		EM_OPTIONS,                        //OPTIONS请求方法
		EM_TRACE,                          //TRACE请求方法
		EM_METHOD_END                      //枚举保护
	}EM_HTTPMETHOD;

	static const std::string HttpMethod [] = 
	{
		"DELETE",
		"GET",
		"HEAD",
		"POST",
		"PUT",
		"CONNECT",
		"OPTIONS",
		"TRACEE"
	};
}

class HttpParser
{
	public:
		/**
		 * @brief 构造函数
		 */
		HttpParser();

		/**
		 * @brief 析构函数
		 */
		~HttpParser();

		/**
		 * @brief 重载 内存分配
		 *
		 * @prame size 内存大小
		 *
		 * @return 内存地址
		 */
		static void * operator new(size_t size)
		{
			void *p = (void*)_MEM_NEW_(size);
			return p;
		}

		/**
		 * @brief 重载 内存释放
		 *
		 * @prame p 释放地址
		 */
		static void operator delete(void *p)
		{
			_MEM_DEL_(p);
		}
		
		/**
		 * @brief 解析器初始化接口
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t init();

		/**
		 * @brief 解析器入口
		 *
		 * @prame pInputPkt 数据主体
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t start(InputPacket *pInputPkt);

#ifdef _GTEST_
	public:
#else
	private:
#endif
		/**
		 * @brief supermatch 初始化
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t superMatchInit();

		/**
		 * @brief 解析uri
		 *
		 * @prame uri; pInputPkt 包体 
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t parserUri(std::string uri, InputPacket *pInputPkt);
	
		/**
		 * @brief 过滤url请求参数
		 *
		 * @prame url
		 *
		 * @return 过滤结果
		 */
		std::string filterUrl(std::string url);

		/**
		 * @brief 解析query
		 *
		 * @prame pInputPkt
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t parserQuery(InputPacket *pInputPkt);

		/**
		 * @brief 解析cookie
		 *
		 * @prame cookie pInputPkt
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t parserCookie(std::string cookie, InputPacket *pInputPkt);

		/**
		 * @brief 解析http body
		 *
		 * @prame http_body http请求体
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t parserBody(std::string http_body, InputPacket *pInputPkt);

		/**
		 * @brief 比较结点信息
		 *
		 * @prame pNode 哈希结点; pInputPkt 数据包结点
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败 
		 */
		int32_t compareMessage(DList<HashNode> *pNode, InputPacket *pInputPkt);

		/**
		 * @brief 统计学习方法
		 *
		 * @prame pNode 哈希结点; pInputPkt 数据包结点
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败 
		 */
		int32_t statisticalMethod(DList<HashNode> *&pNode, InputPacket *pInputPkt);

		/**
		 * @brief 查询模型状态
		 *
		 * @prame pNode 哈希结点; pInputPkt 数据包结点
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败 
		 */
		int32_t checkModelStatus();

		/**
		 * @brief 创建新结点
		 *
		 * @prame pNode 哈希链; pInputPkt 数据包结点
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败 
		 */
		int32_t createNewNode(DList<HashNode> *&pList, InputPacket *pInputPkt,
						std::string cookie, std::string http_body);

		/**
		 * @brief 创建args表
		 *
		 * @prame argaTable args表名
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t createArgsTable(std::string argsTable);

	private:
		/**
		 * @brief 通用状态机 
		 */
		SuperMatch super;

		/**
		 * @brief post状态机
		 */
		SuperMatch postSuper;

		/**
		 * @brief cookie状态机
		 */
		SuperMatch cookieSuper;

};

#endif
