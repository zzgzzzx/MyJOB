//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NDTYPE_H
#define VPN_NDTYPE_H

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <stdarg.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <sys/msg.h>
#include <linux/if_ether.h>
#include <netdb.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>


/*******************************************************************************
*                               Type Definitions
******************************************************************************/
typedef unsigned char   ndUChar;
typedef unsigned short  ndUShort;
typedef unsigned long   ndULong;

typedef unsigned char   ndUInt8;
typedef unsigned short  ndUInt16;
typedef unsigned int    ndUInt32;

typedef signed char     ndInt8;
typedef signed char     ndChar;
typedef signed short    ndInt16;
typedef signed long     ndInt32;
typedef double			 ndDouble;

typedef unsigned char   ND_BOOLEAN;
typedef bool			 ndBool;

typedef signed long     ndStatus;
typedef unsigned char	 ndByte;


//调试日志结果输出
typedef ndChar DEBUG_EXPORT_TYPE;
#define TYPE_DEBUG_EXPORT_SCREEN	1	//1:屏幕输出
#define TYPE_DEBUG_EXPORT_FILE	2	//2:文件输出 
#define TYPE_DEBUG_EXPORT_REMOTE	4	//4:远程监控输出 

/*---------------------------------------------------------------------------------------
//日志文件名
-----------------------------------------------------------------------------------------*/
#define LOG_FILE_NAME	"/var/log/supervpn.log"

/**
 ******************************************************************************
 * @brief   Definition of ND_FALSE
 ******************************************************************************
 */
#if defined( ND_FALSE )
#if  (!(ND_FALSE==0))
      #error ND_FALSE MACRO defined wrongly outside ND Components
   #endif
#else
#define ND_FALSE   0
#endif

/**
 ******************************************************************************
 * @brief   Definition of TRUE
 * #define ND_DLLEXPORT extern "C"
 ******************************************************************************
 */
#if defined( ND_TRUE )
#if  (!(ND_TRUE==1))
      #error ND_TRUE MACRO defined wrongly outside DP Components
   #endif
#else
#define ND_TRUE   1
#endif

#ifdef WIN32
#define ND_DLLEXPORT __declspec(dllexport)
#else
#define ND_DLLEXPORT
#endif

/*---------------------------------------------------------------------------------------
//VPN版本号
-----------------------------------------------------------------------------------------*/
#define SUPER_VPN_CLIENT_VER "1.0.0"

/*---------------------------------------------------------------------------------------
//系统支持的最大消息队列数
-----------------------------------------------------------------------------------------*/
#define MAX_FIFO_MSG_NUM 5000

/*---------------------------------------------------------------------------------------
//定时器ID设置
-----------------------------------------------------------------------------------------*/
#define TIMER_ID_NODE_HELLO_CHECK	0x0001			//hello检测检测

/*---------------------------------------------------------------------------------------
//定时器时间设置(秒数)
-----------------------------------------------------------------------------------------*/
#define TIMER_VALUE_NODE_HELLO_CHECK	60			//节点Hello检测


/*---------------------------------------------------------------------------------------
//curl连接服务器超时时间（秒）
-----------------------------------------------------------------------------------------*/
#define VALUE_CURLOPT_CONNECTTIMEOUT 10

/*---------------------------------------------------------------------------------------
//curl接收数据超时时间（秒）
-----------------------------------------------------------------------------------------*/
#define VALUE_CURLOPT_LOW_SPEED_TIME 30

/*---------------------------------------------------------------------------------------
//设备的key
-----------------------------------------------------------------------------------------*/
#define BUILTIN_KEY  "ZEASN2016"

/*---------------------------------------------------------------------------------------
//中心服务器地址
-----------------------------------------------------------------------------------------*/
#define VPN_CENTER_USER_URL  "http://45.33.58.27:8080/anywhere/node/unode"
#define VPN_CENTER_SRV_URL  "http://45.33.58.27:8080/anywhere/node/pnode"


/*---------------------------------------------------------------------------------------
//数据包Action标签名称定义
-----------------------------------------------------------------------------------------*/
#define SUPER_ACTION_SERVER_NODE_HELLO "server-node-hello"
#define SUPER_ACTION_USER_NODE_HELLO "user-node-hello"

#define SUPER_ACTION_SERVER_NODE_SET "server-node-set"
#define SUPER_ACTION_USER_NODE_SET "user-node-set"

#define VALUE_CURLOPT_DOWNLOAD_RETRY_TIMES 5
#define VALUE_CURLOPT_POST_RETRY_TIMES 3

/*---------------------------------------------------------------------------------------
//节点交易类型定义
-----------------------------------------------------------------------------------------*/
typedef enum
{
	//节点init
	ND_ACTION_NODE_INIT,

	//节点hello
	ND_ACTION_NODE_HELLO
}ndActionEn;

/**
 ******************************************************************************
//返回码定义
 ******************************************************************************
 */
typedef enum
{
    //成功0
    ND_SUCCESS = 0,

    //参数无效1
    ND_ERROR_INVALID_PARAM,

    //平台错误2
    ND_ERROR_PLATFORM_OS,

    //认证失败3
    ND_ERROR_AUTHENTICATION_FAILED,

    //系统不支持4
    ND_ERROR_RESUME_NOT_SUPPORTED,

    //网络无效5
    ND_ERROR_NETWORK_NOT_AVAILABLE,

    //创建socket失败6
    ND_ERROR_SOCKET_CREATION_FAILED,

    //数据发送失败7
    ND_ERROR_SEND_FAILED,

    //数据接收失败8
    ND_ERROR_RECEIVE_FAILED,

    //服务器返回错误9
    ND_ERROR_SERVER,

    //系统不支持10
    ND_ERROR_UNSUPPORTED,

    //下载中断11
    ND_ERROR_DOWNLOAD_ABORTED,

    //crc校验失败12
	ND_ERROR_CRC_CHECK_FAILED,

	//长度溢出13
	ND_ERROR_SIZE_OVERFLOW,

	//编码错误14
	ND_ERROR_MSG_ENCODE,

	//解码错误15
	ND_ERROR_MSG_DECODE,

	//普通错误16
	ND_ERROR_GENERAL,

	//无效的请求17
	ND_ERROR_INVALID_REQUEST,

	//应答无效18
	ND_ERROR_INVALID_RESPONSE,

	//加载证书失败19
	ND_ERROR_LOAD_CA_CERTIFICATE,

	//普通网络错误20
	ND_ERROR_NETWORK_GENERAL,

	//网络不可达21
	ND_ERROR_SERVER_NOT_REACHABLE,

	//连接超进22
	ND_ERROR_CONNECT_TIMEDOUT,

	//线程已在运行中23
	ND_ERROR_THREAD_ALREADY_RUNNING,

	//数据上传中断24
	DP_ERROR_DATA_UPLOAD_ABORTED,

	//无权访问25
	ND_ERROR_FORBIDDEN_ERROR,

	//解密失败26
	ND_ERROR_DECRYPTION_FAILED,

	//调用openssl库失败27
	ND_ERROR_OPENSSL_LIB_CALL,
}ndErrorEn;


/**
******************************************************************************
* @brief   Callback function for DP Client
* @note    This is the syntax of the callback function for DP Client
*			asynchronous APIs
* @param	iCommand: Command ID of the callback.
* @param	iStatus:  Status of the call.
* @param	pResponse POinter to the response structure
******************************************************************************
*/
typedef void (*ndCallbackFunction) (void *param);


#define TRACE printf


#endif //VPN_DPTYPE_H

