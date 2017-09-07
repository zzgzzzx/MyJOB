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
//系统文件路径名称
-----------------------------------------------------------------------------------------*/
#define PATH_PROCNET_DEV		"/proc/net/dev"
#define PATH_PROCNET_ARP		"/proc/net/arp"
#define PATH_PROCNET_ROUTE	"/proc/net/route"

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

//系统支持的最大消息队列数
#define MAX_FIFO_MSG_NUM 5000

/**
 ******************************************************************************
 * @brief  CRC文件读取默认长度
 ******************************************************************************
 */
#define MAX_READ_FILE_LEN (8*1024)

/**
 ******************************************************************************
 * @brief  SecurityKey Length
 ******************************************************************************
 */
#define MAX_SECURITY_KEY_LEN  512

/**
 ******************************************************************************
 * @brief  curl连接服务器超时时间（秒）
 ******************************************************************************
 */
#define VALUE_CURLOPT_CONNECTTIMEOUT 10

/**
 ******************************************************************************
 * @brief  curl接收数据超时时间（秒）
 ******************************************************************************
 */
#define VALUE_CURLOPT_LOW_SPEED_TIME 30

/**
 ******************************************************************************
 * @brief  设备的key
 ******************************************************************************
 */
#define BUILTIN_KEY  "ZEASN2016"

/**
 ******************************************************************************
 * @brief  router服务器地址
 ******************************************************************************
 */
//#define ROUTE_URL  "https://deviceportal.nettvservices.com/dp/route"
//#define ROUTE_URL_CERT_CHECK  "http://deviceportal.nettvservices.com/route/dp/handle"

#define ROUTE_URL  "https://deviceportal.zeasn.tv/dp/route"
#define ROUTE_URL_CERT_CHECK  "https://deviceportal.zeasn.tv/route/dp/handle"

/**
 ******************************************************************************
 * @brief  HTTP HEAD 标识
 ******************************************************************************
 */
#define TAG_HEAD_AUTHENTICATE "WWW-Authenticate"

//本地设备证书路径
#define LOCAL_FILE_DEV_CERT_NAME "zeasn.device.crt"

//本地设备私钥路径
#define LOCAL_FILE_DEV_PRI_KEY_NAME "private.key"

//本地设备私钥路径
#define SERVER_CERT_FILE_NAME "opcacrt6.pem"

//EVN GET返回码定义
#define ENV_GET_ERR -1
#define ENV_GET_NO_ATTACH 0
#define ENV_GET_ATTACH 1

//文件下载路径名称定义
#define NODE_DOWNLOAD_FILE_PATH "DownFilePath"

//证书路径名称定义
#define NODE_FILE_PATH "NodeFilePath"

//完整性校验的hash_method类别定义
#define TYPE_INTEGRITY_CHECK_CRC32 "CRC32"
#define TYPE_INTEGRITY_CHECK_MD5 "MD5"

/**
 ******************************************************************************
 * @brief   DP Client specific error codes
 ******************************************************************************
 */
typedef enum
{
    /**
     * Success 0
     */
    ND_SUCCESS = 0,

    /**
     * One of the input parameters(s) is(are) not valid 1
     */
    ND_ERROR_INVALID_PARAM,

    /**
     * Platform or OS error: e.g. out of memory or semaphore, task, queue error 2
     */
    ND_ERROR_PLATFORM_OS,

    /**
     * The device is not authenticated. 3
     */
     ND_ERROR_AUTHENTICATION_FAILED,

    /**
     * Resume feature is not supported in the device. 4
     */
     ND_ERROR_RESUME_NOT_SUPPORTED,

    /**
     * Network is not available. This happens if the device is not connected
    to the network or if the server is down. 5
     */
     ND_ERROR_NETWORK_NOT_AVAILABLE,

    /**
     * The socket cannot be created 6
     */
     ND_ERROR_SOCKET_CREATION_FAILED,

    /**
     * The client could not send the request to the server.
    This happens if the network is disconnected in between. 7
     */
     ND_ERROR_SEND_FAILED,

    /**
     * The client could not receive the request to the server.
    This happens if server network is disconnected, timeout or
    server not responding.  8
     */
    ND_ERROR_RECEIVE_FAILED,

    /**
     *  The response from the server is invalid. 9
     */
    ND_ERROR_SERVER,

    /**
     * The feature is unsupported 10
     */
    ND_ERROR_UNSUPPORTED,

    /**
     * Download is aborted by the user. 11
     */
    ND_ERROR_DOWNLOAD_ABORTED,

    /**
     * CRC check failed for downloadind file.  12
     */
	ND_ERROR_CRC_CHECK_FAILED,

	/**
	 * Indicates that the download is not in progress. 13
	 */
	ND_ERROR_DOWNLOAD_NOT_IN_PROGRESS,

	/**
	 * SIZE of a destination BUFFER is not enough for creating the ndString chunk 14
	 */
	ND_ERROR_SIZE_OVERFLOW,

	/**
	 * Indicates the failure of request encode 15
	 */
	ND_ERROR_MSG_ENCODE,

	/**
	 * Indicates the failure of response decode 16
	 */
	ND_ERROR_MSG_DECODE,

	/**
	* General error. 17
	*/
	ND_ERROR_GENERAL,

	/**
	* Server returned a failure for Requested operation 18
	*/
	ND_ERROR_OPERATION_FAILED,

	/**
	* Server returned error for incorrect details in request
	Or Application placed invalid request to client 19
	*/
	ND_ERROR_INVALID_REQUEST,

	/**
	* Server returned error 20
	*/
	ND_ERROR_INVALID_RESPONSE,

	/**
	* load CA certificate err 21
	*/
	ND_ERROR_LOAD_CA_CERTIFICATE,


	/**
	* Need more data from application. 22
	*/
	ND_ERROR_NEED_MORE_DATA_FOR_UPLOAD,

	/**
	* Indicates data has been partially uploaded to the server. 23
	*/
	ND_ERROR_DATA_PARTIALLY_UPLOADED,

	/**
	* Device is not provisioned 24
	*/
	ND_ERROR_NOT_PROVISIONED,

	/**
	* General Network Error. 25
	*/
	ND_ERROR_NETWORK_GENERAL,

	/**
	* Server is not Reachable.26
	*/
	ND_ERROR_SERVER_NOT_REACHABLE,

	/**
	* Connection Timed out. 27
	*/
	ND_ERROR_CONNECT_TIMEDOUT,

	/**
	* Thread Already running. 28
	*/
	ND_ERROR_THREAD_ALREADY_RUNNING,

	/**
	* Data upload is aborted by the user. 29
	*/
	DP_ERROR_DATA_UPLOAD_ABORTED,

	/**
	* Indicates that the upload is not in progress. 30
	*/
	ND_ERROR_UPLOAD_NOT_IN_PROGRESS,

	/**
	* Indicates this device is Blocked or HTTP_403. 31
	*/
	ND_ERROR_FORBIDDEN_ERROR,

	/**
	* Indicates that Key provided for decryption is invalid. 32
	*/
	ND_ERROR_DECRYPTION_FAILED,

	/**
	* Call Open SSL lib return err. 33
	*/
	ND_ERROR_OPENSSL_LIB_CALL,

	/**
	* filedown complete
	*/
	ND_ERROR_DOWN_COMPLETE = 1001
}ndErrorEn;

/**
 ******************************************************************************
 * @brief   Node commands indicated by callback function.
 ******************************************************************************
 */
typedef enum
{
	/**
	* Action Init
	*/
	ND_ACTION_NODE_INIT,

	/**
	* Action Hello
	*/
	ND_ACTION_NODE_HELLO,

	/**
	* Action Setting
	*/
	ND_ACTION_NODE_SETTING,
}ndActionEn;


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

