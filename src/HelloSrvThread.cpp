/*********************************************************
ģ����       : 
�ļ���       :HelloSrv.cpp
����ļ�     :HelloSrv.hpp
�ļ�ʵ�ֹ��� :Hello���������ʵ���ļ�
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "HelloSrvThread.hpp"

/*********************************************************
����˵�������캯��
���˵����
����˵����
����ֵ  ��
*********************************************************/
CHelloSrvThread::CHelloSrv()
{
}

/*********************************************************
����˵������������
���˵����
����˵����
����ֵ  ��
*********************************************************/
CHelloSrvThread::~CHelloSrv()
{
}

/*********************************************************
����˵�����������ݰ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
int CHelloSrvThread::SendTo(ndUInt32 IP, ndInt16 iPort, char *pBuf, int iBufLen)
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Send Pkt To Hello Server SrvIP=[%s] port=[%d]!",AfxHostIPToStr(IP),iPort);
	return mSrvSkt.SendTo(IP,iPort,pBuf,iBufLen);
}

/*********************************************************
����˵�����̴߳�����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CHelloSrvThread::ProcessVirtual(void)
{
	if (!StartService())
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Hello Rcv Server Start Error!!!\n");
		return;
	}

	while(true)
	{
		RcvAndDealMsg();
	}
}

/*********************************************************
����˵������������socket
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CHelloSrvThread::StartService()
{
	if (mSrvSkt.Open(HELLO_SRV_LOCAL_PORT) == -1) return false;

	return true;
}

/*********************************************************
����˵�������ղ���������
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CHelloSrvThread::RcvAndDealMsg()
{
	char sBuff[PACKET_BUFF_LEN];
	int iRcvLen;
	ndInt32 ip;
	ndInt16 port;
	SNetParam sNP;	

	iRcvLen = mSrvSkt.RecvFrom((ndUChar*)sBuff, PACKET_BUFF_LEN, ip, port);

	if (iRcvLen <= 0) return;
	
	sNP.lSrcIP = ip;
	sNP.uSrcPort = port;

	CPacket *pkt = new CHelloPkt();
	//���ý��հ�������
	pkt->SetRcvPktBuf(sBuff, iRcvLen);
	//���ý��հ����������
	pkt->SetRcvNetParam(sNP);
	//�����ݰ�����ϵͳ����(Ҳ�����ڴ�ֱ�Ӵ���)
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Recv Hello Pkt IP=[%s]",AfxHostIPToStr(ip));
	AfxInsertPktToSysQue(pkt);	
}

