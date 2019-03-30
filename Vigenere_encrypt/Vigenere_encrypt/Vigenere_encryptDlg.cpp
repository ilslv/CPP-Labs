
// Vigenere_encryptDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "Vigenere_encrypt.h"
#include "Vigenere_encryptDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CVigenereencryptDlg



CVigenereencryptDlg::CVigenereencryptDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIGENERE_ENCRYPT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVigenereencryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TOENCR, input_file_name);
	DDX_Control(pDX, IDC_EDIT_TOOUT, output_file_name);
	DDX_Control(pDX, IDC_EDIT_KEY, keyword_edit);
	DDX_Control(pDX, IDC_RADIO_ENG, ENG_RADIO);
	DDX_Control(pDX, IDC_RADIO_RUS, RUS_RADIO);
}

BEGIN_MESSAGE_MAP(CVigenereencryptDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ENCR, &CVigenereencryptDlg::OnBnClickedButtonEncr)
	ON_EN_CHANGE(IDC_EDIT_KEY, &CVigenereencryptDlg::OnEnChangeEditKey)
END_MESSAGE_MAP()


// Обработчики сообщений CVigenereencryptDlg

BOOL CVigenereencryptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	CWnd* pEncrypt = GetDlgItem(IDC_STATIC_KEY);
	pEncrypt->SetFont(&my_font);
	CWnd* pKey = GetDlgItem(IDC_STATIC_TOENCR);
	pKey->SetFont(&my_font);
	CWnd* pOut = GetDlgItem(IDC_STATIC_TOOUT);
	pOut->SetFont(&my_font);

	

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CVigenereencryptDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CVigenereencryptDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CVigenereencryptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CVigenereencryptDlg::set_font()
{
	LOGFONT lf;
	lf.lfHeight = 40;
	lf.lfWidth = FW_BOLD;
	my_font.CreateFontIndirect(&lf);
}

void CVigenereencryptDlg::OnBnClickedButtonEncr()
{
	CString in_file_name, out_file_name, keyword;
	input_file_name.GetWindowTextW(in_file_name);
	output_file_name.GetWindowTextW(out_file_name);
	keyword_edit.GetWindowTextW(keyword);

	std::ifstream inp(in_file_name);

	char to_encrypt[128];
	inp.getline(to_encrypt, 128);
	inp.close();
	char* encrypted = new char[strlen(to_encrypt + 1)];
	char** vig_square;
	int letters_count = 0;
	std::string language;

	if (ENG_RADIO.GetCheck())
	{
		letters_count = 26;
		language = "Eng";
	}
	else
	{
		letters_count = 33;
		language = "Rus";
	}

	vig_square = new char*[letters_count + 2];
	for (int i = 0; i < letters_count + 2; i++)
		vig_square[i] = new char[letters_count + 2];

	create_square(vig_square, language.c_str(), letters_count);
	encrypt(encrypted, to_encrypt, strlen(to_encrypt), keyword, vig_square, language.c_str());

	std::ofstream out(out_file_name);
	out << encrypted;
	out.close();
}


void CVigenereencryptDlg::OnEnChangeEditKey()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
