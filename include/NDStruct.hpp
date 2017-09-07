//
// Created by lewis on 2017/3/18.
// FZ-Zeasn
//

#ifndef VPN_DPCOMMSTR_H
#define VPN_DPCOMMSTR_H

#include "NDType.hpp"
#include <string>
#include <string.h>
#include <stdlib.h>
#include <set>
#include <list>
#include <map>
#include <vector>

using namespace std;

typedef string ndString;

/*----------------------------------------------------------------
					��־����
----------------------------------------------------------------*/
/* ��־���� */
typedef struct
{
	ndChar	IfOpenDebug;	//�Ƿ���������־
	ndChar	DebugType;		//������־�����ʽ(1:��Ļ��� 2:�ļ���� 4:������)
	ndInt16 CacheTime;		//���ش洢ʱ�Ļ���ʱ��(���ӣ�0��ʾ������);	
	ndInt16 LogDayNum;		//������־�Ĵ洢������(��);
}SLogConf, *pSLogConf;

/**
 ******************************************************************************
 * @brief   Date Time request structure
 * @note    This is the request structure for #dpClientGetDateTime.
 ******************************************************************************
 */
typedef struct
{
    ND_BOOLEAN			bApplyDST;

    ndString			cOffset;

    ndString			cTimeZone;
}dpDateTimeReqSt;


/**
 ******************************************************************************
 * @brief   Service parameters
 * @note    This structure hold information about various service parameters
 *			in name-value format. The example of this can be Name="Country",
 *			cValue="NL". The name and value pairs are agreed between the
 *			server and device teams. The application must allocate enough
 *			memory to hold the data. Else, it may result in memory overflow
 *			or crash.
 ******************************************************************************
 */
typedef struct
{
    ndString sName;

    ndString sValue;
}dpResourceproperSt;

/**
 ******************************************************************************
 * @brief   Service Portal structure
 * @note    This structure provides information about the service portal.
 ******************************************************************************
 */
typedef struct
{
    ndString sName;

    ndString sValue;

    list<dpResourceproperSt> lpropertys;
}dpResourceSt;

/**
 ******************************************************************************
 * @brief   Service Portal response structure
 * @note    This is the response structure for #dpClientGetServicePortals
 ******************************************************************************
 */
typedef struct
{
    ndUInt32 uiResourceReturned;

    list<dpResourceSt> lResource;
}dpResourceRspSt;

//�ַ�����̬���鶨��
typedef vector<string> StrVector;
typedef vector<string>::iterator StrVtrItr;


#endif //VPN_DPCOMMSTR_H

