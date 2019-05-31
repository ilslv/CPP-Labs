#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <winuser.h>
#include <atlstr.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "kr_template.h"
#include "kr.h"

bool is_date_in_range(tm from, tm to, tm val)
{
	val.tm_hour = val.tm_min = 0;
	return mktime(&from) <= mktime(&val) && mktime(&val) <= mktime(&to);
}

int CALLBACK wWinMain(const HINSTANCE hInstance, HINSTANCE, PWSTR lpCmdLine, const int nShowCmd)
{
	static kr_array<kr_template> array;

	static HWND SearchControl, OutputControl;
	MSG msg{};
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
			OutputControl = CreateWindowExA(NULL, "edit", nullptr,
				WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_CLIENTEDGE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
				0, 40, 483, 100, hWnd, nullptr, nullptr, nullptr);
			SearchControl = CreateWindowExA(NULL, "edit", nullptr,
				WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_CLIENTEDGE,
				230, 10, 200, 20, hWnd, nullptr, nullptr, nullptr);
			CreateWindowExA(NULL, "button", "Choose file",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				10, 10, 100, 20, hWnd, reinterpret_cast<HMENU>(1), nullptr, nullptr);
			CreateWindowExA(NULL, "button", "Find",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				120, 10, 100, 20, hWnd, reinterpret_cast<HMENU>(2), nullptr, nullptr);
		}
		return 0;

		case WM_COMMAND:
		{
			if (LOWORD(wParam) == 1)
			{
				OPENFILENAMEA ofn;
				char szFile[260];
				// Initialize OPENFILENAME
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = szFile;
				// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
				// use the contents of szFile to initialize itself.
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = nullptr;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = nullptr;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				if(GetOpenFileNameA(&ofn) == TRUE)
				{
					std::ifstream input_file(ofn.lpstrFile);
					while(!input_file.eof())
					{
						input_file >> array;
					}
					std::stringstream output;
					output << array;
					SetWindowTextA(OutputControl, output.str().c_str());
				}
			}

			if (LOWORD(wParam) == 2)
			{
				auto field = new char[200];
				GetWindowTextA(SearchControl, field, 200);
				std::string name(field);
				auto it = array.find_by_name(name);
				std::stringstream output;
				if(it != array.end())
				{
					output << *it;
					SetWindowTextA(OutputControl, output.str().c_str());
				}
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
	wc.lpszClassName = L"KR";
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
	{
		return EXIT_FAILURE;
	}
	const auto hwnd = CreateWindow(wc.lpszClassName,
		L"KR",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
		500, 179, nullptr, nullptr, wc.hInstance, nullptr);
	if (hwnd == INVALID_HANDLE_VALUE)
	{
		return EXIT_FAILURE;
	}

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}
