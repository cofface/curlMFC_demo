
// curlMFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CcurlMFCApp: 
// �йش����ʵ�֣������ curlMFC.cpp
//

class CcurlMFCApp : public CWinApp
{
public:
	CcurlMFCApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CcurlMFCApp theApp;