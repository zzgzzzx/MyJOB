/*********************************************************
模块名       : 
文件名       :MainServer.cpp
相关文件     :
文件实现功能 :主服务程序
作者         :Lewis
版本         :1.0 
**********************************************************/
#include "BaseApp.hpp"

CSuperVPNApp TSuperVPNApp;

int main(int argc,char *argv[])
{
	
	TSuperVPNApp.Run(argc, argv);

	return true;
}
