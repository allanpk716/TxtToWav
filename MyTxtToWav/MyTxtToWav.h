
// MyTxtToWav.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyTxtToWavApp:
// �йش����ʵ�֣������ MyTxtToWav.cpp
//

class CMyTxtToWavApp : public CWinApp
{
public:
	CMyTxtToWavApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyTxtToWavApp theApp;