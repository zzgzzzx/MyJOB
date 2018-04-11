/*********************************************************
模块名       : 
文件名       :BaseApp.cpp
相关文件     :BaseApp.hpp
文件实现功能 :应用工程类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "BaseApp.hpp"
#include "NDFunc.hpp"

/*********************************************************
函数说明：application初始化
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CCheckSuperVPNApp::InitApplication(int argc,char *argv[])
{
	AfxWriteDebugLog("CheckSuperVPN run at [CCheckSuperVPNApp::InitApplication] ...");

	//系统数据初始化
	AfxWriteDebugLog("CheckSuperVPN run at [CCheckSuperVPNApp::InitApplication] SYSTEM START BEGIN INIT SYSTEM...");
	if (!InitSystem()) 
	{
		AfxWriteDebugLog("CheckSuperVPN run at [CCheckSuperVPNApp::InitApplication] Init System END......");
		return false;
	}
	
	return true;
}


/*********************************************************
函数说明：检测SuperVPN并启动
入参说明：
出参说明：
返回值  ：
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
函数说明：停止SuperVPN执行并升级
入参说明：
出参说明：
返回值  ：
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
函数说明：系统数据初始化
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CCheckSuperVPNApp::InitSystem(void)
{
	//调用SuperVPN检测升级
	AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::InitSystem] begin exec supervpn -u");	
	AfxExecCmd("SuperVPN -u");

	//判断是否有新版的升级文件存在，如果有，结束SuperVPN，进行文件的替换
	AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::InitSystem] begin check if exists upgrade file");	
	if(AfxFileExist(VPN_UPGRADE_FILE_NAME))
	{
		AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::InitSystem] stop all supervpn process");
		StopSuperVPNAndUpgrade();
	}

	//SuperVPN进程是否运行的检测，如果没启动，就进行启动
	AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::InitSystem] check and run supervpn process");
	CheckVPNAndRun();

	return true;
}

/*********************************************************
函数说明：构造函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CCheckSuperVPNApp::CCheckSuperVPNApp()
{
}

/*********************************************************
函数说明：析构函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CCheckSuperVPNApp::~CCheckSuperVPNApp()
{
}

