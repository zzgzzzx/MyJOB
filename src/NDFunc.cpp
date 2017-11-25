/*********************************************************
模块名       : 
文件名       :Func.cpp
相关文件     :Func.hpp
文件实现功能 :全局函数实现
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "NDFunc.hpp"
#include "NDGlobal.hpp"

extern CSuperVPNApp TSuperVPNApp;

/*********************************************************
函数说明：插入单回定时器
入参说明：
出参说明：
返回值  ：timeout(秒数)
*********************************************************/
bool AfxInsertSingleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	return TSuperVPNApp.InsertSingleTimer(callid,timeout,CallBack);
}

/*********************************************************
函数说明：插入循环定时器
入参说明：
出参说明：
返回值  ：timeout(秒数)
*********************************************************/
bool AfxInsertCircleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	return TSuperVPNApp.InsertCircleTimer(callid,timeout,CallBack);
}

/*********************************************************
函数说明：删除循环定时器
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool AfxDeleteCircleTimer(unsigned long callid)
{
	return TSuperVPNApp.DeleteCircleTimer(callid);
}

/*********************************************************
函数说明：删除单个定时器
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool AfxDeleteSingleTimer(unsigned long callid)
{
	return TSuperVPNApp.DeleteSingleTimer(callid);
}

/*********************************************************
函数说明：写调试日志
入参说明：
出参说明：
返回值  ：
*********************************************************/
#define LOG_MIN_SIZE (500*1024)
#define LOG_MAX_SIZE (1*1024*1024)
	
int Shringk(char *pFileName)
{
	long Count;
	char *pBuff;
	FILE *pFile;
	struct stat StatBuff;

	if (stat(pFileName, &StatBuff) == -1) return -1;

	if (StatBuff.st_size > LOG_MAX_SIZE) 
	{
		if ((pBuff = (char *)malloc(LOG_MIN_SIZE)) == NULL)
			return -1;

		if ((pFile = fopen(pFileName,"r")) == NULL)
		{free(pBuff); return -1;}

		if (fseek(pFile, -LOG_MIN_SIZE, SEEK_END) != 0 || fgets(pBuff, LOG_MIN_SIZE, pFile) == NULL)
		{fclose(pFile);free(pBuff);return -1;}

		Count = fread(pBuff, sizeof(char), LOG_MIN_SIZE, pFile);
		if (fclose(pFile) != 0 || (pFile = fopen(pFileName,"w")) == NULL)
		{free(pBuff); return -1;}

		fwrite(pBuff, sizeof(char), Count, pFile);
		fclose(pFile);
		free(pBuff);
	}
	return 0;
}

/*********************************************************
函数说明：写调试日志
入参说明：
出参说明：
返回值  ：
*********************************************************/
void AfxTraceLocalLog(char *sLogContext)
{
	FILE *pFile;

	if ((pFile = fopen(LOG_FILE_NAME,"a+")) == NULL) return;

	fputs(sLogContext, pFile);
	fputs("\n", pFile);
	fclose(pFile);
	Shringk(LOG_FILE_NAME);
}

/*********************************************************
函数说明：写调试日志
入参说明：
出参说明：
返回值  ：
*********************************************************/
void AfxWriteDebugLog(char *Format,...)
{
	if (OPEN_DEBUG_FLAG == ND_FALSE) return;

	va_list Args;
	time_t NowTime;
	char StrBuff[1024];

	time(&NowTime);
	strftime(StrBuff, sizeof(StrBuff)-1,"[%b%d %H:%M:%S]", localtime(&NowTime));
	va_start(Args, Format);
	vsnprintf(StrBuff+strlen(StrBuff), sizeof(StrBuff)-strlen(StrBuff)-1, Format, Args);
	va_end(Args);

	//屏幕输出
	if (DEBUG_TYPE & TYPE_DEBUG_EXPORT_SCREEN)
	{
		cout<<StrBuff<<endl;
	}

	//文件输出
	if (DEBUG_TYPE & TYPE_DEBUG_EXPORT_FILE)
	{
		AfxTraceLocalLog(StrBuff);
	}
}

/*********************************************************
函数说明：获取本机所有网卡的名称，包括未启用
入参说明：
出参说明：
返回值  ：返回网卡的数目
*********************************************************/
#define isspace(c) ((((c) == ' ') || (((unsigned int)((c) - 9)) <= (13 - 9))))

static char *get_name(char *name, char *p)
{
	while (isspace(*p))
		p++;
	while (*p) {
		if (isspace(*p))
			break;
		if (*p == ':') {	/* could be an alias */
			char *dot = p, *dotname = name;

			*name++ = *p++;
			while (isdigit(*p))
				*name++ = *p++;
			if (*p != ':') {	/* it wasn't, backup */
				p = dot;
				name = dotname;
			}
			if (*p == '\0')
				return NULL;
			p++;
			break;
		}
		*name++ = *p++;
	}
	*name++ = '\0';

	return p;
}

int AfxGetAllIfName(vector<string> &vIFName)
{
	FILE *fh;
	char buf[512];

	if ((fh = fopen(PATH_PROCNET_DEV, "r")) == NULL) return 0;

	fgets(buf, sizeof buf, fh);	/* eat line */
	fgets(buf, sizeof buf, fh);	/* eat line */

	while (fgets(buf, sizeof buf, fh)) 
	{
		char name[128];
		get_name(name, buf);

		if (strncmp(name, "eth", 3)) continue;
		if (strchr(name, '.') != NULL) continue;

		vIFName.push_back(name);
	}

	fclose(fh);

	return vIFName.size();
}

/*********************************************************
函数说明：执行系统命令
入参说明：
出参说明：
返回值  ：返回false表示失败
*********************************************************/
bool AfxExecCmd(const char *cmd)
{
	AfxWriteDebugLog("SuperVPN run at [AfxExecCmd] Exec Cmd=[%s]",cmd);
	int status = system(cmd);
	if (status == -1)
	{
		return false;
	}

	if (WIFEXITED(status))
	{
		if (!WEXITSTATUS(status))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	} 
}

/*********************************************************
函数说明：十六进制的串转成整数
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndUInt32 AfxHexStrToInt(const char src[])
{
	unsigned int lDes;

	if ((NULL == src)||(strlen(src)<8)) return 0;

	sscanf(src,"%08X",&lDes);

	return lDes;
}

/*********************************************************
函数说明：转换MAC格式00-11-22-33-44-55转成6字符
入参说明：
出参说明：
返回值  ：
*********************************************************/
void AfxFormatMac(const char src[],ndUChar des[])
{
	int lMac[6];

	if ((NULL == src)||(strlen(src)<17)) return;

	sscanf(src,"%02X-%02X-%02X-%02X-%02X-%02X",\
					&lMac[0],&lMac[1],&lMac[2],\
					&lMac[3],&lMac[4],&lMac[5]);

	for(int m=0; m<6; m++) des[m] = lMac[m];
}

/*********************************************************
函数说明：转换MAC格式00:11:22:33:44:55转成6字符
入参说明：
出参说明：
返回值  ：
*********************************************************/
void AfxFormatMacT(const char src[],ndUChar des[])
{
	int lMac[6];

	if ((NULL == src)||(strlen(src)<17)) return;

	sscanf(src,"%02X:%02X:%02X:%02X:%02X:%02X",\
		&lMac[0],&lMac[1],&lMac[2],\
		&lMac[3],&lMac[4],&lMac[5]);

	for(int m=0; m<6; m++) des[m] = lMac[m];
}

/*********************************************************
函数说明：转换MAC6字符格式转成00-11-22-33-44-55
入参说明：
出参说明：
返回值  ：
*********************************************************/
char *AfxMacToStr(const ndUChar src[])
{
	static char sDes[32]={0};

	if (NULL == src) return sDes;
	sprintf(sDes, "%02X-%02X-%02X-%02X-%02X-%02X",src[0],src[1],src[2],src[3],src[4],src[5]);
	
	return sDes;
}

/*********************************************************
函数说明：读取网卡的MAC地址
入参说明：
出参说明：
返回值  ：
*********************************************************/
void AfxGetEthMac(const char *ethname,ndUChar mac[])
{
	ndUChar sMac[6];
	int sd = 0;
	struct  ifreq  if_data;

	sd = socket(AF_INET, SOCK_DGRAM, 0);
	strcpy( if_data.ifr_name, ethname );
	ioctl( sd, SIOCGIFHWADDR, &if_data);
	memcpy(mac, &if_data.ifr_hwaddr.sa_data, 6);
	//memcpy(sMac,&if_data.ifr_hwaddr.sa_data,6);
	//sprintf((char *)mac,"%02X-%02X-%02X-%02X-%02X-%02X",sMac[0],sMac[1],sMac[2],sMac[3],sMac[4],sMac[5]);
	close( sd );
}

/*********************************************************
函数说明：GBK编码转成utf8
入参说明：src:源串,des:返回串,maxlen:返回串最大值
出参说明：
返回值  ：转换后长度
*********************************************************/
int AfxGBK2UTF8(const char *src, char *des, int maxlen)
{
	iconv_t cd = iconv_open("UTF-8", "GBK");
	if ((iconv_t) -1 == cd)	return 0;

	size_t ilen = strlen(src);
	size_t olen = maxlen;
	char *in = (char *)src;
	char *out = des;
	int iLen = maxlen;

	if (iconv(cd, &in, &ilen, &out, &olen) == -1) return 0;

	iLen -= olen;

	des[iLen] = 0;

	iconv_close(cd);

	return iLen;
} 

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
void AfxChangeToUTF8(string &context)
{
	if (context.size() <= 0) return;

	char *pos = (char *)malloc(context.size()*2);

	int iLen = AfxGBK2UTF8(context.c_str(), pos, context.size()*2-1);

	if (iLen <= 0)
	{
		free(pos);
		return;
	}

	context.clear();
	context.append(pos, iLen);

	free(pos);
}

/*********************************************************
函数说明：数组转成16进制字符串
入参说明：src：源数组
		  srclen:数组字符个数
出参说明：
返回值  ：
*********************************************************/
string AfxByteArrayToHexStr(const ndByte src[],const int &srclen)
{
	string sDes;
	char sTemp[8];

	for (int i=0; i<srclen; i++)
	{
		sprintf(sTemp,"%02X",src[i]);
		sDes.append(sTemp);
	}
	return sDes;
}

/*********************************************************
函数说明：字符串理函数
入参说明：src：源字符串
		  tok：分隔符
		  num：至少返回多少个(不足的时候自动补空串)
		  trim：是否过滤空串
		  null_subst：空串替换串
出参说明：
返回值  ：
*********************************************************/
StrVector AfxStrTokenize(const string& src,const string& tok,int num,bool trim, string null_subst)
{
	StrVector v;
	v.clear();
	if( src.empty() || tok.empty() ) return v;

	unsigned int pre_index = 0, index = 0, len = 0;   
	while( (index = src.find_first_of(tok, pre_index)) != string::npos )   
	{   
		if( (len = index-pre_index) != 0 )   
			v.push_back(src.substr(pre_index, len));   
		else if(trim == false)   
			v.push_back(null_subst);   
		pre_index = index+1;   
	}   
	string endstr = src.substr(pre_index);   
	if( trim == false ) 
		v.push_back( endstr.empty()?null_subst:endstr );   
	else 
		if( !endstr.empty() ) v.push_back(endstr);   

	for (int m=v.size(); m<num; m++) v.push_back(null_subst);

	return v; 
}
