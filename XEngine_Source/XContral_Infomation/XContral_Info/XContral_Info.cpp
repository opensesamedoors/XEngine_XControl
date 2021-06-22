#include "pch.h"
#include "XContral_Info.h"
/********************************************************************
//	Created:	2017/5/26   14:09
//	Filename: 	G:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_NetHelp\NetHelp_BackManage\BackManage_GetInfo\BackManage_GetInfo.cpp
//	File Path:	G:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_NetHelp\NetHelp_BackManage\BackManage_GetInfo
//	File Base:	BackManage_GetInfo
//	File Ext:	cpp
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	信息收集类
//	History:
*********************************************************************/
CXContral_Info::CXContral_Info()
{

}
CXContral_Info::~CXContral_Info()
{

}
//////////////////////////////////////////////////////////////////////////
//                          公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：XContral_Info_HardWare
函数功能：获取硬件信息
 参数.一：ptszHWInfo
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出获取到的数据,这个数据是JSON格式
 参数.二：pInt_Len
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出数据的长度
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXContral_Info::XContral_Info_HardWare(TCHAR *ptszHWInfo,int *pInt_Len)
{
    BackManage_IsErrorOccur = FALSE;

    if ((NULL == ptszHWInfo) || (NULL == pInt_Len))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = ERROR_NETENGINE_NETHELP_BACKMANAGE_GETINFO_HARDWARE_PARAMENT;
        return FALSE;
    }
    int nDiskNumber = 0;
    LPCTSTR lpszDir = _T("/usr");
    TCHAR tszOSName[MAX_PATH];
    DWORD nOSVersion;
    DWORD nOSBuild;
    DWORD nOSPro;
    TCHAR tszOSInfo[1024];
    SYSTEMAPI_DISK_INFOMATION st_DiskInfo;
    SYSTEMAPI_CPU_INFOMATION st_CPUInfo;
    SYSTEMAPI_MEMORY_INFOMATION st_MemoryInfo;
    SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial;

    memset(tszOSName,'\0',sizeof(tszOSName));
    memset(tszOSInfo,'\0',sizeof(tszOSInfo));
    memset(&st_MemoryInfo, '\0', sizeof(SYSTEMAPI_MEMORY_INFOMATION));
    memset(&st_CPUInfo,'\0',sizeof(SYSTEMAPI_CPU_INFOMATION));
    memset(&st_DiskInfo,'\0',sizeof(SYSTEMAPI_DISK_INFOMATION));
    memset(&st_SDKSerial,'\0',sizeof(SYSTEMAPI_SERIAL_INFOMATION));

    if (!SystemApi_HardWare_GetDiskNumber(&nDiskNumber,NULL))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    if (!SystemApi_HardWare_GetDiskInfomation(lpszDir,&st_DiskInfo, XENGINE_SYSTEMSDK_API_SYSTEM_SIZE_MB))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    if (!SystemApi_HardWare_GetCpuInfomation(&st_CPUInfo))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    if (!SystemApi_System_GetMemoryUsage(&st_MemoryInfo, XENGINE_SYSTEMSDK_API_SYSTEM_SIZE_MB))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    if (!SystemApi_HardWare_GetSerial(&st_SDKSerial))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    if (!SystemApi_System_GetSystemVer(tszOSName,&nOSVersion,&nOSBuild,&nOSPro))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }

    Json::Value st_JsonRoot;
    Json::Value st_JsonDisk;
    Json::Value st_JsonCpu;
    Json::Value st_JsonSerial;
    Json::Value st_JsonMemory;
    Json::Value st_JsonNetCard;

    st_JsonDisk["DiskNumber"] = nDiskNumber;
    st_JsonDisk["DiskFree"] = (Json::UInt64)st_DiskInfo.dwDiskFree;
    st_JsonDisk["DiskTotal"] = (Json::UInt64)st_DiskInfo.dwDiskTotal;
    st_JsonDisk["DiskName"] = lpszDir;

    st_JsonCpu["CpuNumber"] = st_CPUInfo.nCpuNumber;
    st_JsonCpu["CpuSpeed"] = st_CPUInfo.nCpuSpeed;
    st_JsonCpu["CpuName"] = st_CPUInfo.tszCpuName;

    st_JsonMemory["MemoryFree"] = (Json::UInt64)st_MemoryInfo.dwMemory_Free;
    st_JsonMemory["MemoryTotal"] = (Json::UInt64)st_MemoryInfo.dwMemory_Total;

    st_JsonSerial["DiskSerial"] = st_SDKSerial.tszDiskSerial;
    st_JsonSerial["CpuSerial"] = st_SDKSerial.tszCpuSerial;
    st_JsonSerial["BaseboardSerial"] = st_SDKSerial.tszBaseBoardSerial;
    st_JsonSerial["BiosSerial"] = st_SDKSerial.tszBiosSerail;

	int nListCount = 0;
	APIHELP_NETCARD** ppSt_NetCard;
	APIHelp_NetWork_GetIPAddr(&ppSt_NetCard, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
        Json::Value st_JsonIPAddr;
        st_JsonIPAddr["tszIFName"] = ppSt_NetCard[i]->tszIFName;
        st_JsonIPAddr["tszIPAddr"] = ppSt_NetCard[i]->tszIPAddr;
        st_JsonIPAddr["tszBroadAddr"] = ppSt_NetCard[i]->tszBroadAddr;
        st_JsonIPAddr["tszMaskAddr"] = ppSt_NetCard[i]->tszMaskAddr;
        st_JsonIPAddr["tszMacAddr"] = ppSt_NetCard[i]->tszMacAddr;
        st_JsonNetCard.append(st_JsonIPAddr);
	}
    BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_NetCard, nListCount);

    st_JsonRoot["Disk"] = st_JsonDisk;
    st_JsonRoot["Cpu"] = st_JsonCpu;
    st_JsonRoot["Memory"] = st_JsonMemory;
    st_JsonRoot["Serial"] = st_JsonSerial;
    st_JsonRoot["NetCard"] = st_JsonNetCard;

    _stprintf_s(tszOSInfo,_T("%s %lu %lu %lu"),tszOSName, nOSVersion,nOSBuild,nOSPro);
    st_JsonRoot["Platfrom"] = tszOSInfo;

    if (*pInt_Len < (int)st_JsonRoot.toStyledString().length())
    {
        *pInt_Len = st_JsonRoot.toStyledString().length();
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = ERROR_NETENGINE_NETHELP_BACKMANAGE_GETINFO_HARDWARE_LEN;
        return FALSE;
    }
    *pInt_Len = st_JsonRoot.toStyledString().length();
    memcpy(ptszHWInfo,st_JsonRoot.toStyledString().c_str(),*pInt_Len);

    return TRUE;
}
/********************************************************************
函数名称：XContral_Info_SoftWare
函数功能：获取软件系统信息
 参数.一：ptszSWInfo
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出系统信息JSON结构
 参数.二：pInt_Len
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出系统信息长度
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXContral_Info::XContral_Info_SoftWare(TCHAR *ptszSWInfo,int *pInt_Len)
{
    BackManage_IsErrorOccur = FALSE;

    if ((NULL == ptszSWInfo) || (NULL == pInt_Len))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = ERROR_NETENGINE_NETHELP_BACKMANAGE_GETINFO_SOFTWARE_PARAMENT;
        return FALSE;
    }
    int nProcessCount;
    DWORD nOSBuild;
    DWORD nOSProcessor;
    DWORD nOSVersion;
    TCHAR tszOSInfo[MAX_PATH];
    TCHAR tszUPTime[MAX_PATH];
    TCHAR tszOSUser[MAX_PATH];
    TCHAR tszServicePacket[MAX_PATH];
    XENGINE_LIBTIMER st_LibTimer;

    memset(tszOSInfo, '\0', MAX_PATH);
    memset(tszUPTime, '\0', MAX_PATH);
    memset(tszOSUser, '\0', MAX_PATH);
    memset(tszServicePacket, '\0', MAX_PATH);
    memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));

#ifdef _WINDOWS
	DWORD dwMaxSize = MAX_PATH;
	if (!GetComputerName(tszOSUser, &dwMaxSize))
	{
		BackManage_IsErrorOccur = TRUE;
		BackManage_dwErrorCode = ERROR_NETENGINE_NETHELP_BACKMANAGE_GETINFO_SOFTWARE_GETNAME;
		return FALSE;
	}
#else
	struct passwd* pSt_Passwd = NULL;
	pSt_Passwd = getpwuid(getuid());
	if (NULL == pSt_Passwd)
	{
		BackManage_IsErrorOccur = TRUE;
		BackManage_dwErrorCode = ERROR_NETENGINE_NETHELP_BACKMANAGE_GETINFO_SOFTWARE_GETNAME;
		return FALSE;
	}
    _tcscpy(tszOSUser, pSt_Passwd->pw_name);
#endif
    if (!SystemApi_System_GetSystemVer(tszOSInfo,&nOSVersion,&nOSBuild,&nOSProcessor))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    if (!SystemApi_System_GetProcessCount(&nProcessCount))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    if (!SystemApi_System_GetUpTime(&st_LibTimer))
    {
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    _stprintf_s(tszUPTime,_T("%04d-%02d-%02d %02d:%02d:%02d"),st_LibTimer.wYear,st_LibTimer.wMonth,st_LibTimer.wDay,st_LibTimer.wHour,st_LibTimer.wMinute,st_LibTimer.wSecond);

    Json::Value st_JsonRoot;
    Json::Value st_JsonSystem;

    st_JsonSystem["OSUser"] = tszOSUser;
    st_JsonSystem["OSUPTime"] = tszUPTime;
    st_JsonSystem["OSVersion"] = tszOSInfo;
    st_JsonSystem["OSProcessCount"] = nProcessCount;
    st_JsonRoot["OSInfo"] = st_JsonSystem;

    if (*pInt_Len < (int)st_JsonRoot.toStyledString().length())
    {
        *pInt_Len = st_JsonRoot.toStyledString().length();
        BackManage_IsErrorOccur = TRUE;
        BackManage_dwErrorCode = ERROR_NETENGINE_NETHELP_BACKMANAGE_GETINFO_SOFTWARE_LEN;
        return FALSE;
    }
    *pInt_Len = st_JsonRoot.toStyledString().length();
    memcpy(ptszSWInfo, st_JsonRoot.toStyledString().c_str(), *pInt_Len);

    return TRUE;
}
