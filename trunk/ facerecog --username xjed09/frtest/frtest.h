
// frtest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CfrtestApp:
// �йش����ʵ�֣������ frtest.cpp
//

class CfrtestApp : public CWinAppEx
{
public:
	CfrtestApp();

// ��д
	public:
	 BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CfrtestApp theApp;