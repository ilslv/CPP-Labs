#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <winuser.h>
#include <atlstr.h>
#include <string>
#include <bitset>

// 0-9, . 10, + 11, - 12, * 13, / 14, = 15, C 16, CE 17, ± 18, ⌫ 19, ~ 20, and 21, or 22, xor 23, >> 24, << 25 

bool isInt(const double x)
{
	return ceil(x) == x;
}

bool isInt(const CString& text)
{
	return ceil(_tstof(text)) == _tstof(text);
}

class CalcData
{
	double field_;
	int operation_id_;
public:
	CalcData()
	{
		field_ = 0;
		operation_id_ = 0;
	}

	void cleanAll()
	{
		field_ = operation_id_ = 0;
	}
	CString operation(const int operation_id, const CString& text)
	{
		if (operation_id_ == 0)
		{
			if (operation_id != 5)
			{
				operation_id_ = operation_id;
				field_ = _tstof(text);
			}
		}
		else if (operation_id_ == 1)
		{
			operation_id_ = operation_id;
			field_ += _tstof(text);
		}
		else if (operation_id_ == 2)
		{
			operation_id_ = operation_id;
			field_ -= _tstof(text);
		}
		else if (operation_id_ == 3)
		{
			operation_id_ = operation_id;
			field_ *= _tstof(text);
		}
		else if (operation_id_ == 4)
		{
			if (_tstof(text) == 0)
			{
				this->cleanAll();
				throw std::invalid_argument("Division by 0!");
			}
			operation_id_ = operation_id;
			field_ /= _tstof(text);

		}
		else if (isInt(field_) && isInt(text))
		{
			if (operation_id_ == 11)
			{
				field_ = static_cast<int>(field_) & _tstoi(text);
				operation_id_ = operation_id;
			}
			else if (operation_id_ == 12)
			{
				field_ = static_cast<int>(field_) | _tstoi(text);
				operation_id_ = operation_id;
			}
			else if (operation_id_ == 13)
			{
				field_ = static_cast<int>(field_) ^ _tstoi(text);
				operation_id_ = operation_id;
			}
		}
		else
		{
			cleanAll();
			throw std::invalid_argument("Both numbers must be int!");
		}

		if (operation_id == 5)
		{
			if (operation_id_ == 0)
			{
				return text;
			}
			else
			{
				operation_id_ = 0;
				std::string str = std::to_string(field_);
				str.erase(str.find_last_not_of('0') + 1, std::string::npos);
				if (str[str.length() - 1] == '.')
				{
					str[str.length() - 1] = '\0';
				}
				return str.c_str();
			}

		}
		else
		{
			operation_id_ = operation_id;
			return L"";
		}
	}
};

HWND label_dec, label_bin;
CalcData calc;

void addButtons(const HWND h_wnd)
{
	for (int i = 0; i < 9; i++)
	{
		CString t;
		t.Format(_T("%d"), i + 1);
		CreateWindow(
			L"Button", t,
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			i % 3 * 50, (i / 3 + 1) * 50, 50, 50, h_wnd, reinterpret_cast<HMENU>(i + 1), nullptr, nullptr
		);
	}
	CreateWindow(
		L"Button", L"0",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		50, 200, 50, 50, h_wnd, reinterpret_cast<HMENU>(0), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L"±",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 200, 50, 50, h_wnd, reinterpret_cast<HMENU>(18), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L".",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 200, 50, 50, h_wnd, reinterpret_cast<HMENU>(10), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L"+",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		150, 50, 50, 50, h_wnd, reinterpret_cast<HMENU>(11), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L"-",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		150, 100, 50, 50, h_wnd, reinterpret_cast<HMENU>(12), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L"*",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		150, 150, 50, 50, h_wnd, reinterpret_cast<HMENU>(13), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L"/",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		150, 200, 50, 50, h_wnd, reinterpret_cast<HMENU>(14), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L"⌫",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		150, 0, 50, 50, h_wnd, reinterpret_cast<HMENU>(19), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L"C",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 250, 50, 50, h_wnd, reinterpret_cast<HMENU>(16), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L"=",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		50, 250, 100, 50, h_wnd, reinterpret_cast<HMENU>(15), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L"CE",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		150, 250, 50, 50, h_wnd, reinterpret_cast<HMENU>(17), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L"~",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 0, 50, 50, h_wnd, reinterpret_cast<HMENU>(20), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L">>",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 50, 50, 50, h_wnd, reinterpret_cast<HMENU>(24), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L"<<",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 100, 50, 50, h_wnd, reinterpret_cast<HMENU>(25), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L"AND",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 150, 50, 50, h_wnd, reinterpret_cast<HMENU>(21), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L"OR",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 200, 50, 50, h_wnd, reinterpret_cast<HMENU>(22), nullptr, nullptr
	);
	CreateWindow(
		L"Button", L"XOR",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 250, 50, 50, h_wnd, reinterpret_cast<HMENU>(23), nullptr, nullptr
	);


	label_dec = CreateWindow(
		L"static", L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_CLIENTEDGE | ES_RIGHT,
		0, 0, 150, 50, h_wnd, nullptr, nullptr, nullptr
	);
	label_bin = CreateWindow(
		L"static", L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_CLIENTEDGE | ES_RIGHT,
		0, 300, 250, 20, h_wnd, nullptr, nullptr, nullptr
	);
	auto hFontDec = CreateFont(30, 0, 0, 0,
		FW_DONTCARE, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"MS Shell Dlg"
	);
	auto hFontBin = CreateFont(15, 0, 0, 0,
		FW_DONTCARE, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"MS Shell Dlg"
	);
	SendMessage(label_dec, WM_SETFONT, WPARAM(hFontDec), TRUE);
	SendMessage(label_bin, WM_SETFONT, WPARAM(hFontBin), TRUE);
}

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	MSG msg{};
	HWND hwnd{};
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT
	{
		switch (uMsg)
		{
		case WM_CREATE:
		{
			addButtons(hWnd);
		}
		return 0;

		case WM_COMMAND:
		{
			const auto str = new TCHAR[100];
			GetWindowText(label_dec, str, 100);
			CString text(str);

			if (LOWORD(wParam) < 10)
			{
				text += std::to_string(LOWORD(wParam)).c_str();
				SetWindowText(label_dec, text);
			}
			else if (LOWORD(wParam) == 10)
			{
				if (text.Find('.') == -1)
				{
					text += '.';
				}
				SetWindowText(label_dec, text);
			}
			else if (LOWORD(wParam) == 16)
			{
				text.Empty();
				SetWindowText(label_dec, L"");
			}
			else if (LOWORD(wParam) == 17)
			{
				text.Empty();
				calc.cleanAll();
				SetWindowText(label_dec, L"");
			}
			else if (LOWORD(wParam) == 18)
			{
				if (text[0] == '-')
				{
					text.Delete(0);
				}
				else
				{
					text.Insert(0, '-');
				}
				SetWindowText(label_dec, text);
			}
			else if (LOWORD(wParam) == 19)
			{
				text.Delete(text.GetLength() - 1);
				SetWindowText(label_dec, text);
			}
			else if (LOWORD(wParam) == 20)
			{
				if (isInt(text))
				{
					int x = ~_tstoi(text);
					text.Format(L"%d", x);
					SetWindowText(label_dec, text);
				}
				else
				{
					MessageBox(hWnd, L"Not an integer!", L"Error!", MB_ICONERROR);
				}
			}
			else if (LOWORD(wParam) == 24)
			{
				if (isInt(text))
				{
					int x = _tstoi(text) >> 1;
					text.Format(L"%d", x);
					SetWindowText(label_dec, text);
				}
				else
				{
					MessageBox(hWnd, L"Not an integer!", L"Error!", MB_ICONERROR);
				}
			}
			else if (LOWORD(wParam) == 25)
			{
				if (isInt(text))
				{
					int x = _tstoi(text) << 1;
					text.Format(L"%d", x);
					SetWindowText(label_dec, text);
				}
				else
				{
					MessageBox(hWnd, L"Not an integer!", L"Error!", MB_ICONERROR);
				}
			}
			else
			{
				try
				{
					text = calc.operation(LOWORD(wParam) - 10, text);
					SetWindowText(label_dec, text);
				}
				catch (const std::invalid_argument& e)
				{
					SetWindowText(label_dec, L"");
					wchar_t wtext[100];
					mbstowcs_s(nullptr, wtext, e.what(), 100);
					MessageBox(hWnd, wtext, L"Error!", MB_ICONERROR);
				}
			}


			if (_tstof(text) < pow(2, 32))
			{
				if (isInt(text))
				{
					const CString bin = std::bitset<32>(_tstoi(text)).to_string().c_str();
					SetWindowText(label_bin, bin);
				}
				else
				{
					SetWindowText(label_bin, L"Not an int");
				}
			}
			else
			{
				SetWindowText(label_bin, L"Too big");
			}

		}
		return 0;

		case WM_DESTROY:
		{
			PostQuitMessage(EXIT_SUCCESS);
		}
		return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	};
	wc.lpszClassName = L"Calc";
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
	{
		return EXIT_FAILURE;
	}
	hwnd = CreateWindow(wc.lpszClassName,
		L"Calc",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
		266, 359, nullptr, nullptr, wc.hInstance, nullptr);
	if (hwnd == INVALID_HANDLE_VALUE)
	{
		return EXIT_FAILURE;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}
