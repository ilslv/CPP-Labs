
// Vigenere_encryptDlg.h: файл заголовка
//

#pragma once
#include "encrypt.h"
#include <fstream>

// Диалоговое окно CVigenereencryptDlg
class CVigenereencryptDlg : public CDialogEx
{
	CFont my_font;
// Создание
public:
	void set_font();
	CVigenereencryptDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIGENERE_ENCRYPT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStaticToencr();
	afx_msg void OnBnClickedButtonEncr();
	CEdit input_file_name;
	CEdit output_file_name;
	CEdit keyword_edit;
	CButton ENG_RADIO;
	CButton RUS_RADIO;
	afx_msg void OnEnChangeEditKey();
};
