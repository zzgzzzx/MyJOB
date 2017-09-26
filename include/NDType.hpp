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
//ϵͳ�ļ�·������
-----------------------------------------------------------------------------------------*/
#define PATH_PROCNET_DEV		"/proc/net/dev"
#define PATH_PROCNET_ARP		"/proc/net/arp"
#define PATH_PROCNET_ROUTE	"/proc/net/route"

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
 * @brief  curl���ӷ�������ʱʱ�䣨�룩
-----------------------------------------------------------------------------------------*/
#define VALUE_CURLOPT_CONNECTTIMEOUT 10

/*---------------------------------------------------------------------------------------
 * @brief  curl�������ݳ�ʱʱ�䣨�룩
-----------------------------------------------------------------------------------------*/
#define VALUE_CURLOPT_LOW_SPEED_TIME 30

/*---------------------------------------------------------------------------------------
 * @brief  �豸��key
-----------------------------------------------------------------------------------------*/
#define BUILTIN_KEY  "ZEASN2016"

/*---------------------------------------------------------------------------------------
 * @brief  ���ķ�������ַ
-----------------------------------------------------------------------------------------*/
#define VPN_CENTER_URL  "https://deviceportal.zeasn.tv/dp/route"


/*---------------------------------------------------------------------------------------
 * @brief   �ڵ㽻�����Ͷ���
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
 * @brief   �����붨��
 ******************************************************************************
 */
typedef enum
{
    //�ɹ�
    ND_SUCCESS = 0,

    //������Ч
    ND_ERROR_INVALID_PARAM,

    //ƽ̨����
    ND_ERROR_PLATFORM_OS,

    //��֤ʧ��
    ND_ERROR_AUTHENTICATION_FAILED,

    //ϵͳ��֧��
    ND_ERROR_RESUME_NOT_SUPPORTED,

    //������Ч
    ND_ERROR_NETWORK_NOT_AVAILABLE,

    //����socketʧ��
    ND_ERROR_SOCKET_CREATION_FAILED,

    //���ݷ���ʧ��
    ND_ERROR_SEND_FAILED,

    //���ݽ���ʧ��
    ND_ERROR_RECEIVE_FAILED,

    //���������ش���
    ND_ERROR_SERVER,

    //ϵͳ��֧��
    ND_ERROR_UNSUPPORTED,

    //�����ж�
    ND_ERROR_DOWNLOAD_ABORTED,

    //crcУ��ʧ��
	ND_ERROR_CRC_CHECK_FAILED,

	//�������
	ND_ERROR_SIZE_OVERFLOW,

	//�������
	ND_ERROR_MSG_ENCODE,

	//�������
	ND_ERROR_MSG_DECODE,

	//��ͨ����
	ND_ERROR_GENERAL,

	//��Ч������
	ND_ERROR_INVALID_REQUEST,

	//Ӧ����Ч
	ND_ERROR_INVALID_RESPONSE,

	//����֤��ʧ��
	ND_ERROR_LOAD_CA_CERTIFICATE,

	//��ͨ�������
	ND_ERROR_NETWORK_GENERAL,

	//���粻�ɴ�
	ND_ERROR_SERVER_NOT_REACHABLE,

	//���ӳ���
	ND_ERROR_CONNECT_TIMEDOUT,

	//�߳�����������
	ND_ERROR_THREAD_ALREADY_RUNNING,

	//�����ϴ��ж�
	DP_ERROR_DATA_UPLOAD_ABORTED,

	//��Ȩ����
	ND_ERROR_FORBIDDEN_ERROR,

	//����ʧ��
	ND_ERROR_DECRYPTION_FAILED,

	//����openssl��ʧ��
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

