/****************** Copyright (c) 2008 **************
ģ����       : 
�ļ���       :Func.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :ȫ�ֺ�������
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __FUNC_HPP__
#define __FUNC_HPP__

#include <iconv.h> 
#include "BaseApp.hpp"
#include "NDType.hpp"
#include "NDStruct.hpp"

/*---------------------------------------------------------------------------------------
//��ʱ��
-----------------------------------------------------------------------------------------*/
//���뵥�ض�ʱ��	
bool AfxInsertSingleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong));
//����ѭ����ʱ��	
bool AfxInsertCircleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong));

//ɾ��ѭ����ʱ��
bool AfxDeleteCircleTimer(unsigned long callid);
//ɾ��������ʱ��
bool AfxDeleteSingleTimer(unsigned long callid);
//��ȡHello����
CHelloSrv *AfxGetHelloSrv(void);
//��ȡ����ڵ㼯��
CServiceSet *AfxGetServiceSet(void);

/*---------------------------------------------------------------------------------------
//��־
-----------------------------------------------------------------------------------------*/
//д������־
void AfxWriteDebugLog(char *Format,...);
//��ȡ��һ�����õķ�������Ϣ
bool AfxGetNextSrvUrl(SServerInfo &serverInfo);

//�ڵ����ļ���д
void AfxWriteNodeID(const char *nodeid);
char *AfxGetNodeID();
//�����б��ļ���д
ndBool AfxGetServerList(list<SServerInfo> &mServers);
ndBool AfxUpdateServerList(list<SServerInfo> &mServers);

//��ȡmac��ַ
void AfxGetEthMac(const char *ethname, ndString &mac);
	

//������IP��ַת���ַ���
inline char *AfxNetIPToStr(const ndULong &IP)
{
	static char sIP[32];

	sprintf(sIP,"%d.%d.%d.%d",
		((unsigned char *)&IP)[0], 
		((unsigned char *)&IP)[1], 
		((unsigned char *)&IP)[2], 
		((unsigned char *)&IP)[3]);

	return sIP;
}

//������IP��ַת���ַ���
inline char *AfxHostIPToStr(const ndULong &IP)
{
	static char sIP[32];

	sprintf(sIP,"%d.%d.%d.%d",
		((unsigned char *)&IP)[3], 
		((unsigned char *)&IP)[2], 
		((unsigned char *)&IP)[1], 
		((unsigned char *)&IP)[0]);

	return sIP;
}

//��ȡ�Է���IP��ַ
inline ndULong AfxGetSrcIP(int socket)
{
	struct sockaddr_in sa;
	socklen_t sl = sizeof(struct sockaddr_in);

	if (getpeername(socket, (struct sockaddr *)&sa, &sl) < 0) return 0;

	return sa.sin_addr.s_addr;
}

bool AfxFileExist(const string filepath);
ndDouble AfxFileSize(const string filepath);

bool AfxCheckCmdExist(const string cmd);

//ִ��ϵͳ����
bool AfxExecCmd(const char *cmd);
bool AfxExecCmdNotWait(const char *cmd);


/*---------------------------------------------------------------------------------------
//Socket��д����
-----------------------------------------------------------------------------------------*/
//socket������(����ͬ���ķ�ʽ)
int AfxCompleteRead(int iSocket,	 //socket�׽���
					void *pvBuffer,			//���ջ�����
					unsigned long ulSize, 		//����������
					unsigned long ulTimeout);	//��ʱʱ��

//socketд����(����ͬ���ķ�ʽ)
int AfxCompleteWrite(int iSocket,	//socket�׽���
					 const void *pvBuffer,		//���ͻ�����
					 unsigned long ulSize, 		//����������
					 unsigned long ulTimeout);	//��ʱʱ��

////socketд����(�����첽�ķ�ʽ)
//int AfxCompleteWrite(int iSocket,				//socket�׽���
//					 const void *pvBuffer,		//���ͻ�����
//					 unsigned long ulSize 		//����������
//					 );

/*---------------------------------------------------------------------------------------
//�ַ���ת������
-----------------------------------------------------------------------------------------*/
//gbk==>utf-8
int AfxGBK2UTF8(const char *src, char *des, int maxlen);

void AfxChangeToUTF8(string &context);

/*---------------------------------------------------------------------------------------
//�ַ���������
-----------------------------------------------------------------------------------------*/
//��ؿ�
inline void AfxvRTrim(char *pcBuf)
{
	int iLen;
	iLen=strlen(pcBuf);
	while( iLen>0)
	{
		if ((' '==pcBuf[iLen-1])||('\t'==pcBuf[iLen-1])||('\n'==pcBuf[iLen-1])||('\r'==pcBuf[iLen-1]))
		{
			pcBuf[iLen-1]='\0';
			iLen--;
		}
		else
			break;
	}
}

inline void AfxvLTrim(char *pcBuf)
{
	int iLen;
	iLen=strlen(pcBuf);
	while( iLen>0 )
	{
		if ((' '==pcBuf[0])||('\t'==pcBuf[0]))
		{
			strcpy(pcBuf,pcBuf+1);
			iLen--;
		}
		else
			break;
	}
}

//���ҽؿ�
inline void AfxvTrim(char *pcBuf)
{
	AfxvRTrim(pcBuf);
	AfxvLTrim(pcBuf);
}

StrVector AfxStrTokenize(const string& src,		//Դ��
						 const string& tok,		//�ָ���
						 int num=0,				//���ٷ��ص�Ԫ�ظ���
						 bool trim=false, 		//�Ƿ���˿մ�
						 string null_subst="");	//�մ�������ַ���

string AfxByteArrayToHexStr(const ndUChar src[],	//Դ����
							const int &srclen);	//�����ַ�����

#endif
