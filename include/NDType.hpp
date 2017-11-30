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


//������־������
typedef ndChar DEBUG_EXPORT_TYPE;
#define TYPE_DEBUG_EXPORT_SCREEN	1	//1:��Ļ���
#define TYPE_DEBUG_EXPORT_FILE	2	//2:�ļ���� 
#define TYPE_DEBUG_EXPORT_REMOTE	4	//4:Զ�̼����� 

/*---------------------------------------------------------------------------------------
//��־�ļ���
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
//VPN�汾��
-----------------------------------------------------------------------------------------*/
#define SUPER_VPN_CLIENT_VER "1.0.0"

/*---------------------------------------------------------------------------------------
//ϵͳ֧�ֵ������Ϣ������
-----------------------------------------------------------------------------------------*/
#define MAX_FIFO_MSG_NUM 5000

/*---------------------------------------------------------------------------------------
//��ʱ��ID����
-----------------------------------------------------------------------------------------*/
#define TIMER_ID_NODE_HELLO_CHECK	0x0001			//hello�����

/*---------------------------------------------------------------------------------------
//��ʱ��ʱ������(����)
-----------------------------------------------------------------------------------------*/
#define TIMER_VALUE_NODE_HELLO_CHECK	60			//�ڵ�Hello���


/*---------------------------------------------------------------------------------------
//curl���ӷ�������ʱʱ�䣨�룩
-----------------------------------------------------------------------------------------*/
#define VALUE_CURLOPT_CONNECTTIMEOUT 10

/*---------------------------------------------------------------------------------------
//curl�������ݳ�ʱʱ�䣨�룩
-----------------------------------------------------------------------------------------*/
#define VALUE_CURLOPT_LOW_SPEED_TIME 30

/*---------------------------------------------------------------------------------------
//�豸��key
-----------------------------------------------------------------------------------------*/
#define BUILTIN_KEY  "ZEASN2016"

/*---------------------------------------------------------------------------------------
//���ķ�������ַ
-----------------------------------------------------------------------------------------*/
#define VPN_CENTER_USER_URL  "http://45.33.58.27:8080/anywhere/node/unode"
#define VPN_CENTER_SRV_URL  "http://45.33.58.27:8080/anywhere/node/pnode"


/*---------------------------------------------------------------------------------------
//���ݰ�Action��ǩ���ƶ���
-----------------------------------------------------------------------------------------*/
#define SUPER_ACTION_SERVER_NODE_HELLO "server-node-hello"
#define SUPER_ACTION_USER_NODE_HELLO "user-node-hello"

#define SUPER_ACTION_SERVER_NODE_SET "server-node-set"
#define SUPER_ACTION_USER_NODE_SET "user-node-set"

#define VALUE_CURLOPT_DOWNLOAD_RETRY_TIMES 5
#define VALUE_CURLOPT_POST_RETRY_TIMES 3

/*---------------------------------------------------------------------------------------
//�ڵ㽻�����Ͷ���
-----------------------------------------------------------------------------------------*/
typedef enum
{
	//�ڵ�init
	ND_ACTION_NODE_INIT,

	//�ڵ�hello
	ND_ACTION_NODE_HELLO
}ndActionEn;

/**
 ******************************************************************************
//�����붨��
 ******************************************************************************
 */
typedef enum
{
    //�ɹ�0
    ND_SUCCESS = 0,

    //������Ч1
    ND_ERROR_INVALID_PARAM,

    //ƽ̨����2
    ND_ERROR_PLATFORM_OS,

    //��֤ʧ��3
    ND_ERROR_AUTHENTICATION_FAILED,

    //ϵͳ��֧��4
    ND_ERROR_RESUME_NOT_SUPPORTED,

    //������Ч5
    ND_ERROR_NETWORK_NOT_AVAILABLE,

    //����socketʧ��6
    ND_ERROR_SOCKET_CREATION_FAILED,

    //���ݷ���ʧ��7
    ND_ERROR_SEND_FAILED,

    //���ݽ���ʧ��8
    ND_ERROR_RECEIVE_FAILED,

    //���������ش���9
    ND_ERROR_SERVER,

    //ϵͳ��֧��10
    ND_ERROR_UNSUPPORTED,

    //�����ж�11
    ND_ERROR_DOWNLOAD_ABORTED,

    //crcУ��ʧ��12
	ND_ERROR_CRC_CHECK_FAILED,

	//�������13
	ND_ERROR_SIZE_OVERFLOW,

	//�������14
	ND_ERROR_MSG_ENCODE,

	//�������15
	ND_ERROR_MSG_DECODE,

	//��ͨ����16
	ND_ERROR_GENERAL,

	//��Ч������17
	ND_ERROR_INVALID_REQUEST,

	//Ӧ����Ч18
	ND_ERROR_INVALID_RESPONSE,

	//����֤��ʧ��19
	ND_ERROR_LOAD_CA_CERTIFICATE,

	//��ͨ�������20
	ND_ERROR_NETWORK_GENERAL,

	//���粻�ɴ�21
	ND_ERROR_SERVER_NOT_REACHABLE,

	//���ӳ���22
	ND_ERROR_CONNECT_TIMEDOUT,

	//�߳�����������23
	ND_ERROR_THREAD_ALREADY_RUNNING,

	//�����ϴ��ж�24
	DP_ERROR_DATA_UPLOAD_ABORTED,

	//��Ȩ����25
	ND_ERROR_FORBIDDEN_ERROR,

	//����ʧ��26
	ND_ERROR_DECRYPTION_FAILED,

	//����openssl��ʧ��27
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

