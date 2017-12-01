//
// Created by lewis on 2017/3/18.
// FZ-ZEASN
//

#ifndef _UPDATECHECK_H_
#define _UPDATECHECK_H_

#include <curl.h>
#include "NDType.hpp"
#include "NDStruct.hpp"


class CUpdateCheck{
private:
	//生成请求的数据包
	void MakeRequestPkg();
	
    void DownloadFile();

	void UpgradeSelf();

public:
    CUpdatCheck(void);
    ~CUpdatCheck(void);

    //开始检测升级
    void BeginCheck();
};
#endif
