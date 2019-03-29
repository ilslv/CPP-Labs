#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <winuser.h>
#include <atlstr.h>
#include <string>
#include <fstream>
#include "Agency.h"
#include <sstream>
#include <iomanip>

bool is_date_in_range(tm from, tm to, tm val)
{
	val.tm_hour = val.tm_min = 0;
	return mktime(&from) <= mktime(&val) && mktime(&val) <= mktime(&to);
}

int CALLBACK wWinMain(const HINSTANCE hInstance, HINSTANCE, PWSTR lpCmdLine, const int nShowCmd)
{
	static Agency agency;
	std::ifstream file("concerts.csv");
	while (!file.eof())
	{
		file >> agency;
	}
	agency.sort_by_date();

	static HWND SearchControl, OutputControl, FromDateControl, ToDateControl;
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
			FromDateControl = CreateWindowExA(NULL, "SysDateTimePick32", nullptr,
				WS_BORDER | WS_CHILD | WS_VISIBLE,
				0, 0, 150, 20, hWnd, nullptr, nullptr, nullptr);
			ToDateControl = CreateWindowExA(NULL, "SysDateTimePick32", nullptr,
				WS_BORDER | WS_CHILD | WS_VISIBLE,
				150, 0, 150, 20, hWnd, nullptr, nullptr, nullptr);
			SearchControl = CreateWindowExA(NULL, "edit", nullptr,
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				0, 20, 200, 20, hWnd, nullptr, nullptr, nullptr);
			OutputControl = CreateWindowExA(NULL, "edit", nullptr,
				WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_CLIENTEDGE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
				0, 40, 300, 100, hWnd, nullptr, nullptr, nullptr);
			CreateWindowExA(NULL, "button", "Search",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				200, 20, 100, 20, hWnd, reinterpret_cast<HMENU>(1), nullptr, nullptr);
		}
		return 0;

		case WM_COMMAND:
		{
			if (LOWORD(wParam) == 1)
			{
				tm date_from, date_to;
				const auto date_from_s = new char[100];
				GetWindowTextA(FromDateControl, date_from_s, 100);
				std::stringstream ss1(date_from_s);
				ss1 >> std::get_time(&(date_from), "%d.%m.%Y");
				const auto date_to_s = new char[100];
				GetWindowTextA(ToDateControl, date_to_s, 100);
				std::stringstream ss2(date_to_s);
				ss2 >> std::get_time(&(date_to), "%d.%m.%Y");
				date_from.tm_hour = date_from.tm_min = date_to.tm_hour = date_to.tm_min = 0;

				const auto text_length = GetWindowTextLengthA(SearchControl) + 1;
				const auto search_text = new char[text_length];
				GetWindowTextA(SearchControl, search_text, text_length);

				std::string output_text;
				char date[20];

				for (const auto& e : agency)
				{
					if ((text_length == 0 || e.name.find(search_text) != std::string::npos) && is_date_in_range(date_from, date_to, e.date))
					{
						strftime(date, 20, "%b %d %Y %H:%M", &(e.date));
						output_text += e.name + "\r\t" + date + "\r\n";
					}
				}
				SetWindowTextA(OutputControl, output_text.c_str());
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
	wc.lpszClassName = L"Agency";
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
	{
		return EXIT_FAILURE;
	}
	const auto hwnd = CreateWindow(wc.lpszClassName,
		L"Agency",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
		315, 179, nullptr, nullptr, wc.hInstance, nullptr);
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
