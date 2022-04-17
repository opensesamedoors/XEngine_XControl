﻿#ifdef _MSC_BUILD
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <json/json.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#ifdef _CENTOS
#include <json/json.h>
#else
#include <jsoncpp/json/json.h>
#endif
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <list>
#include <thread>
#include <memory>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Error.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpServer_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpServer_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/EmailClient_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/EmailClient_Error.h>
#include <XEngine_Include/XEngine_DownLoad/DownLoad_Define.h>
#include <XEngine_Include/XEngine_DownLoad/DownLoad_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include "../XControl_Protocol.h"
#include "../XControl_Configure/Config_Define.h"
#include "../XControl_Configure/Config_Error.h"
#include "../XControl_Infomation/XInfo_Define.h"
#include "../XControl_Infomation/XInfo_Error.h"
#include "../XControl_Protocol/Protocol_Define.h"
#include "../XControl_Protocol/Protocol_Error.h"

#define XENGINE_CONTRALAPP_NETTYPE_XPRC 1
//邮件配置
typedef struct
{
	RFCCOMPONENTS_EMAILSMTP st_EMailSmtp;
	CHAR tszAddrList[2048];
	BOOL bEnable;
	BOOL bCreateEmail;
}XENGIEN_EMAILCONFIG;

extern BOOL bIsRun;
extern BOOL bExist;
extern XLOG xhLog;
extern SOCKET hTCPSocket;
extern SOCKET hUDPSocket;
extern int m_nTaskSerial;
extern XHANDLE xhRPCPacket;
extern XNETHANDLE xhRPCPool;
extern XNETHANDLE xhRPCSocket;
extern XNETHANDLE xhRPCHeart;

extern shared_ptr<std::thread> pSTDThread_Http;
extern shared_ptr<std::thread> pSTDThread_TCP;
extern shared_ptr<std::thread> pSTDThread_UDP;
extern shared_ptr<std::thread> pSTDThread_App;
extern XENGINE_SERVERCONFIG st_ServiceConfig;
extern XENGINE_CONFIGAPP st_APPConfig;
extern XENGIEN_EMAILCONFIG st_EMailConfig;

#include "XControl_Config.h"
#include "XControl_Handle.h"
#include "XControl_Task.h"
#include "XControl_Network.h"
#include "XControl_Process.h"
#include "XControl_RPCTask.h"

#include "XControl_RPCTask/RPCTask_MethodCal.h"
#include "XControl_RPCTask/RPCTask_MethodMessage.h"

#ifdef _MSC_BUILD
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_OPenSsl.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Core/XEngine_ManagePool.lib")
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_XLog.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_EmailClient.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_HttpServer.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIHelp.lib")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi.lib")
#pragma comment(lib,"XEngine_DownLoad/XEngine_DownLoad.lib")
#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib,"../x64/Debug/jsoncpp")
#pragma comment(lib,"../x64/Debug/XControl_Configure.lib")
#pragma comment(lib,"../x64/Debug/XControl_Infomation.lib")
#pragma comment(lib,"../x64/Debug/XControl_Protocol.lib")
#else
#pragma comment(lib,"../x64/Release/jsoncpp")
#pragma comment(lib,"../x64/Release/XControl_Configure.lib")
#pragma comment(lib,"../x64/Release/XControl_Infomation.lib")
#pragma comment(lib,"../x64/Release/XControl_Protocol.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib,"../Debug/jsoncpp")
#pragma comment(lib,"../Debug/XControl_Configure.lib")
#pragma comment(lib,"../Debug/XControl_Infomation.lib")
#pragma comment(lib,"../Debug/XControl_Protocol.lib")
#else
#pragma comment(lib,"../Release/jsoncpp")
#pragma comment(lib,"../Release/XControl_Configure.lib")
#pragma comment(lib,"../Release/XControl_Infomation.lib")
#pragma comment(lib,"../Release/XControl_Protocol.lib")
#endif
#endif
#endif