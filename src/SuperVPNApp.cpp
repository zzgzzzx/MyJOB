/*********************************************************
ģ����       : 
�ļ���       :BaseApp.cpp
����ļ�     :BaseApp.hpp
�ļ�ʵ�ֹ��� :Ӧ�ù�����
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "BaseApp.hpp"
#include "NDFunc.hpp"
#include "NodeUser.hpp"
#include "NodeSrv.hpp"

CNodeBase *CSuperVPNApp::mPNode;

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CSuperVPNApp::InitApplication(void)
{
	//ϵͳ���ݳ�ʼ��
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] SYSTEM START BEGIN INIT SYSTEM...");
	if (!InitSystem()) 
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] Init System Error......");
		cout<<"Init System Error......";
		return false;
	}
	return true;
}

/*********************************************************
����˵����ϵͳ���ݳ�ʼ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CSuperVPNApp::InitSystem(void)
{
	//�汾�������
    CHttpFileDown FileDown;
    SDownloadFileReqSt downloadFileReqSt;
    downloadFileReqSt.sLocalFile = "";
    downloadFileReqSt.sUrl = "";
    downloadFileReqSt.fFile = NULL;
    downloadFileReqSt.iOffset = 0;
    downloadFileReqSt.iCRC = 0;
    downloadFileReqSt.iSize = 0;
    if (!FileDown.SetDownFileReq(downloadFileReqSt))
    {
        return ND_ERROR_INVALID_REQUEST;
    }
    ndStatus ret = FileDown.ExecAction();
    if(ret != ND_SUCCESS)
    {
        TRACE("SuperVPN run at [CCertificateSrv::GetSNInformAndCheck] Download Private Key Err=[%s] Code=[%d]\n", mKeyUrl.c_str(), ret);
        return ret;
    }
	//�ڵ��ʼ����ʼ
	mPNode->NodeInit();

	//���Ӷ�ʱHello
	AfxInsertCircleTimer(TIMER_ID_NODE_HELLO_CHECK, TIMER_VALUE_NODE_HELLO_CHECK, NodeHelloFunc);

	//ѭ���ȴ���֤���˳�Ӧ��
	while(true)
		sleep(60);

	return true;
}

/*********************************************************
����˵�������캯��
���˵����
����˵����
����ֵ  ��
*********************************************************/
CSuperVPNApp::CSuperVPNApp()
{
	#ifdef GENERAL_NODE_USER_APP
		mPNode = new CNodeUser();
	#else
		mPNode = new CNodeSrv();
	#endif
}

/*********************************************************
����˵������������
���˵����
����˵����
����ֵ  ��
*********************************************************/
CSuperVPNApp::~CSuperVPNApp()
{
	delete mPNode;
}

/*********************************************************
����˵����Hello�Ķ�ʱ������
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CSuperVPNApp::NodeHelloFunc(ndULong param)
{
	mPNode->NodeHello();
}

