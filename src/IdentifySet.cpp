/*********************************************************
ģ����       : 
�ļ���       :IdentifySet.cpp
����ļ�     :Set.hpp
�ļ�ʵ�ֹ��� :�û���ʶ������
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "IdentifySet.hpp"
#include "NDFunc.hpp"
#include "NodeUser.hpp"

/*********************************************************
����˵�������캯��
���˵����
����˵����
����ֵ  ��
*********************************************************/
CIdentifySet::CIdentifySet()
{

}

/*********************************************************
����˵������������
���˵����
����˵����
����ֵ  ��
*********************************************************/
CIdentifySet::~CIdentifySet()
{

}

/*********************************************************
����˵�������մ������ݰ�(ֻ�����زŻᴥ��)
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CIdentifySet::DealRcvPkt(CHelloPkt *pkt)
{
	//���յ�����ڵ�����ݰ�����Է���ڵ��IP���в���
	ServiceInform *pSI;
	if (mServiceMap.find(AfxHostIPToStr(pkt->GetRcvNetParam().lSrcIP), pSI))
	{
		pSI->iHelloNum = MAX_VALUE_HELLO_CHECK_TIMES;
		return true;
	}
	return false;
}

/*********************************************************
����˵��:�������������ݿ��ж�ȡMAC����ݵ�ʶ����
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CIdentifySet::ReadMacIdentifyFromGW(list<SBindInform> &ltSI)
{
	//need complete by lewis
	return false;
}

/*********************************************************
����˵��:�������������ݿ��ж�ȡMAC����ݵ�ʶ���벢����ҵ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CIdentifySet::InitIdentifyFromGW()
{
	list<SBindInform> ltSI;

	//=============================================================================
	//1����ȡ�����豸MAC�����ID��Ӧ�Ĺ�ϵ(���������ݿ��ȡ),���������ݵĳ�ʼ��
	//����Mac�����ID
	//=============================================================================
	ReadMacIdentifyFromGW(ltSI);

	//=============================================================================
	//2�����ݻ�ȡ��MAC�����ID���������������ID����ڵĶ�Ӧ��ϵ(�����ķ�������ȡ)
	//=============================================================================
	CNodeUser *pNode= dynamic_cast<CNodeUser *>(AfxGetVPNNode());
	pNode->BindIdentifyService(ltSI);

	return false;
}

/*********************************************************
����˵��:��ARP���ж�ȡMAC��IP��Ӧ��ϵ
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CIdentifySet::ReadARP(list<SBindInform> &ltBSer)
{
	//need complet by lewis
	return false;
}

/*********************************************************
����˵��������Hello�����������Ƿ����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CIdentifySet::SendHelloAndCheck()
{
	ServiceInform *pService;
	SBindInform sBI;
	ndStatus ret;

	CNodeUser *pNode= dynamic_cast<CNodeUser *>(AfxGetVPNNode());	
	
	bool have = mServiceMap.GetBegin(pService);
	while(have)
	{
		//��������ʧ����֪ͨ����������
		if(pService->iHelloNum <= 0)
		{	
			//���з�����������豸ͨ����·�ɽ����ͷ�
			BindInformItr iter = pService->ltBindInform.begin();
			while(iter != pService->ltBindInform.end())
			{
				//֪ͨ���������������Ϣ�����ʧ��
				sBI = *iter;
				ret = pNode->ServiceErrorNotify(sBI);
				if (ret != ND_SUCCESS)
				{
					AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::SendHelloAndCheck] ServiceErrorNotify Err=[%d]", ret);
					iter++; 
					continue;
				}
				//�Ƴ��ɵ�
				pNode->RemoveEdgeAndRoute(*iter);
				//�����µ�
				pNode->BindIdentifyService(sBI);
				BindInformItr iterBak = iter;
				iterBak++;
				pService->ltBindInform.erase(iter);
				iter = iterBak;
			}
			//�ж��Ƿ������豸�����³��ڷ�����ɣ����ȫ����ɣ������Ƴ����ڷ���
			if(pService->ltBindInform.size() <= 0)
			{
				mServiceMap.EraseCurMNext();
				have = mServiceMap.GetCurrent(pService);
				continue;
			}
		}

		//����Hello
		CHelloPkt hellPkt;
		SNetParam sNP;
		sNP.lDesIP = inet_addr(pService->sServiceIP.c_str());
		sNP.uDesPort = HELLO_SRV_LOCAL_PORT;
		hellPkt.SetSndNetParam(sNP);
		hellPkt.SendHelloPkt();

		//Helloʣ�����--
		pService->iHelloNum--;
		
		have = mServiceMap.GetNext(pService);
	}
}

/*********************************************************
����˵�������ӽڵ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CIdentifySet::AddItem(ndString Key, SBindInform *pBI)
{
	mMacMap.insert(pBI->sDeviceMac, pBI);

	ServiceInform *pSI;
	bool bFind = mServiceMap.find(pBI->sServiceIP, pSI);
	if(!bFind)
	{
		pSI = new ServiceInform();
		pSI->iHelloNum = MAX_VALUE_HELLO_CHECK_TIMES;
		pSI->ltBindInform.push_back(*pBI);
		pSI->sServiceIP = pBI->sServiceIP;
		mServiceMap.insert(pBI->sServiceIP, pSI);	
	}
	else
	{
		pSI->ltBindInform.push_back(*pBI);
	}
	
	return insert(Key,pBI);
}

/*********************************************************
����˵����ɾ���ڵ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CIdentifySet::DelItem(ndString Key)
{
	SBindInform *pBI, sBI;
	if (!nlfind(Key, pBI)) return false;

	mMacMap.erase(pBI->sDeviceMac);

	ServiceInform *pSI;
	bool bFind = mServiceMap.find(pBI->sServiceIP, pSI);
	if(bFind)
	{
		BindInformItr iter = pSI->ltBindInform.begin();
		while(iter != pSI->ltBindInform.end())
		{
			sBI = *iter;
			if(sBI.sDeviceFlag == pBI->sDeviceFlag)
			{
				pSI->ltBindInform.erase(iter);
				break;
			}
			iter++;
		}
		if(pSI->ltBindInform.empty())
			mServiceMap.erase(pBI->sServiceIP);
	}	
	delete pBI;
	
	return nlerase(Key);
}

/*********************************************************
����˵�������ҽڵ�(������)
���˵����
����˵����
����ֵ  ��
*********************************************************/
SBindInform *CIdentifySet::FindItemNL(ndString Key)
{
	SBindInform *pBI;

	if (nlfind(Key,pBI)) return pBI;

	return NULL;
}

/*********************************************************
����˵�������ҽڵ�(����)
���˵����
����˵����
����ֵ  ��
*********************************************************/
SBindInform *CIdentifySet::FindItem(ndString Key)
{
	SBindInform *pBI;

	if (find(Key,pBI)) return pBI;

	return NULL;
}

/*********************************************************
����˵��������mac���ҽ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
SBindInform *CIdentifySet::FindItemByMac(ndString mac)
{
	SBindInform *pBI;
	if (mMacMap.find(mac, pBI)) return pBI;

	return NULL;
}


