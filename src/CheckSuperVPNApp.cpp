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

/*********************************************************
����˵����application��ʼ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CCheckSuperVPNApp::InitApplication(int argc,char *argv[])
{
	AfxWriteDebugLog("CheckSuperVPN run at [CCheckSuperVPNApp::InitApplication] ...");

	//ϵͳ���ݳ�ʼ��
	AfxWriteDebugLog("CheckSuperVPN run at [CCheckSuperVPNApp::InitApplication] SYSTEM START BEGIN INIT SYSTEM...");
	if (!InitSystem()) 
	{
		AfxWriteDebugLog("CheckSuperVPN run at [CCheckSuperVPNApp::InitApplication] Init System END......");
		return false;
	}
	
	return true;
}


/*********************************************************
����˵�������SuperVPN������
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CCheckSuperVPNApp::CheckVPNAndRun()
{
	char cmd[512]={0};
	ndString result;

	sprintf(cmd, "ps | grep SuperVPN | grep -v grep | grep -v CheckSuperVPN | wc -l");
	AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::CheckVPNAndRun] Begin get SuperVPN process num");	
	if (AfxRunCmdGetResult(cmd, result))
	{
		AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::CheckVPNAndRun] Process Num=[%d]", atoi(result.c_str()));	
		if(atoi(result.c_str()) <= 0)
		{
			AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::CheckVPNAndRun] Begin Run %s", VPN_EXE_FILE_NAME);	
			sprintf(cmd, "%s&\n", VPN_EXE_FILE_NAME);
			AfxExecCmd(cmd);
		}
	}

	/*
	AfxWriteDebugLog("CheckSuperVPN run at [CCheckSuperVPNApp::CheckVPNAndRun] Begin make /dev/shm/checkandstartvpn.sh");
	FILE *pFile;
	char cmd[512]={0};
	
	if ((pFile = fopen(CHECK_START_SH_FILE_NAME, "w+")) == NULL) return;

	sprintf(cmd, "ps -fe|grep %s |grep -v grep\n", VPN_EXE_FILE_NAME);
	fputs(cmd, pFile);
	sprintf(cmd, "if [ $? -ne 0 ]\n");
	fputs(cmd, pFile);
	sprintf(cmd, "then\n");
	fputs(cmd, pFile);
	sprintf(cmd, "%s&\n", VPN_EXE_FILE_NAME);
	fputs(cmd, pFile);
	sprintf(cmd, "else\n");
	fputs(cmd, pFile);
	sprintf(cmd, "echo SuperVPN Already Runing.....\n");	
	fputs(cmd, pFile);
	sprintf(cmd, "fi\n");
	fputs(cmd, pFile);
	sprintf(cmd, "rm %s\n", CHECK_START_SH_FILE_NAME);
	fputs(cmd, pFile);	
	fclose(pFile);

	AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::CheckVPNAndRun] Exec chmod 755 /dev/shm/checkandstartvpn.sh");	
	sprintf(cmd, "chmod 755 %s", CHECK_START_SH_FILE_NAME);
	AfxExecCmd(cmd);
	sprintf(cmd, "%s", CHECK_START_SH_FILE_NAME);
	AfxExecCmd(cmd);	
	*/
}



/*********************************************************
����˵����ֹͣSuperVPNִ�в�����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CCheckSuperVPNApp::StopSuperVPNAndUpgrade()
{
	char cmd[512]= {0};
	sprintf(cmd, "killall %s", "SuperVPN");
	AfxExecCmd(cmd);
	sprintf(cmd, "mv %s %s\n", VPN_UPGRADE_FILE_NAME, VPN_EXE_FILE_NAME);
	AfxExecCmd(cmd);
}


/*********************************************************
����˵����ϵͳ���ݳ�ʼ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CCheckSuperVPNApp::InitSystem(void)
{
	//����SuperVPN�������
	AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::InitSystem] begin exec supervpn -u");	
	AfxExecCmd("SuperVPN -u");

	//�ж��Ƿ����°�������ļ����ڣ�����У�����SuperVPN�������ļ����滻
	AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::InitSystem] begin check if exists upgrade file");	
	if(AfxFileExist(VPN_UPGRADE_FILE_NAME))
	{
		AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::InitSystem] stop all supervpn process");
		StopSuperVPNAndUpgrade();
	}

	//SuperVPN�����Ƿ����еļ�⣬���û�������ͽ�������
	AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::InitSystem] check and run supervpn process");
	CheckVPNAndRun();

	return true;
}

/*********************************************************
����˵�������캯��
���˵����
����˵����
����ֵ  ��
*********************************************************/
CCheckSuperVPNApp::CCheckSuperVPNApp()
{
}

/*********************************************************
����˵������������
���˵����
����˵����
����ֵ  ��
*********************************************************/
CCheckSuperVPNApp::~CCheckSuperVPNApp()
{
}

