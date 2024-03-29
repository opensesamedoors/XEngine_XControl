﻿#include "XControl_Hdr.h"

BOOL XControl_Handle_PostListFile(CHAR** ppszFileList, int nListCount, LPCSTR lpszPostUrl)
{
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;

	for (int i = 0; i < nListCount; i++)
	{
		Json::Value st_JsonObject;
		st_JsonObject["FileName"] = ppszFileList[i];
		st_JsonObject["FileDir"] = ppszFileList[i];
		st_JsonArray.append(st_JsonObject);
	}
	st_JsonRoot["Count"] = nListCount;
	st_JsonRoot["List"] = st_JsonArray;

	if (!APIClient_Http_Request("POST", lpszPostUrl, st_JsonRoot.toStyledString().c_str()))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, "HTTP任务:发送文件列表失败,地址:%s,错误码:%lX", lpszPostUrl, APIClient_GetLastError());
		return FALSE;
	}
	return TRUE;
}