
// Vigenere_encrypt.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CVigenereencryptApp:
// Сведения о реализации этого класса: Vigenere_encrypt.cpp
//

class CVigenereencryptApp : public CWinApp
{
public:
	CVigenereencryptApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CVigenereencryptApp theApp;
