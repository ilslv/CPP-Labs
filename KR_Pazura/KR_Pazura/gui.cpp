#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <winuser.h>
#include <atlstr.h>
#include <iomanip>
#include "Med.h"
#include <fstream>

void dtoa(char* s, const double x)
{
	char ret[10], tmp[3];
	auto a = static_cast<int>(x);
	_itoa_s(a, ret, 10);
	a = int(double(x - a) * 100);
	_itoa_s(a, tmp, 10);
	strcat_s(ret, ".");
	strcat_s(ret, tmp);
	strcpy_s(s, 10, ret);
}

int CALLBACK wWinMain(const HINSTANCE hInstance, HINSTANCE, PWSTR lpCmdLine, const int nShowCmd)
{
	static MB1 m1;
	static MB2 m2;
	static MB3 m3;
	static ABS_Med *a1, *a2;

	static HWND SearchControl, OutputControl1, OutputControl2, OutputControl3;
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
			OutputControl1 = CreateWindowExA(NULL, "edit", nullptr,
				WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_CLIENTEDGE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
				0, 40, 300, 100, hWnd, nullptr, nullptr, nullptr);
			OutputControl2 = CreateWindowExA(NULL, "edit", nullptr,
				WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_CLIENTEDGE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
				300, 40, 300, 100, hWnd, nullptr, nullptr, nullptr);
			OutputControl3 = CreateWindowExA(NULL, "edit", nullptr,
				WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_CLIENTEDGE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
				0, 140, 600, 100, hWnd, nullptr, nullptr, nullptr);
			SearchControl = CreateWindowExA(NULL, "edit", nullptr,
				WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_CLIENTEDGE,
				120, 10, 200, 20, hWnd, nullptr, nullptr, nullptr);
			CreateWindowExA(NULL, "button", "Find",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				10, 10, 100, 20, hWnd, reinterpret_cast<HMENU>(1), nullptr, nullptr);

			std::ifstream in1("in1.txt");
			std::ifstream in2("in2.txt");
			std::ofstream out("out.txt");
			a1 = &m1;
			a2 = &m2;
			a1->GetMeds(in1);
			a1->SortUp();
			a1->PutMeds(out);
			a2->GetMeds(in2);
			a2->SortUp();
			a2->PutMeds(out);

			char output[1000];
			output[0] = '\0';
			for (auto i = 0; i < m1.GetNumberOfElements(); ++i)
			{
				char count[10];
				_itoa_s(*static_cast<int*>((*m1[i])[3]), count, 10);
				strcat_s(output, static_cast<char*>((*m1[i])[0]));
				strcat_s(output, "\t");
				strcat_s(output, static_cast<char*>((*m1[i])[1]));
				strcat_s(output, "\t");
				strcat_s(output, static_cast<char*>((*m1[i])[2]));
				strcat_s(output, "\t");
				strcat_s(output, count);
				strcat_s(output, "\r\n");
			}
			SetWindowTextA(OutputControl1, output);

			output[0] = '\0';
			for (auto i = 0; i < m2.GetNumberOfElements(); ++i)
			{
				char d[10];
				dtoa(d, *static_cast<double*>((*m2[i])[1]));
				strcat_s(output, static_cast<char*>((*m2[i])[0]));
				strcat_s(output, "\t");
				strcat_s(output, d);
				strcat_s(output, "\r\n");
			}
			SetWindowTextA(OutputControl2, output);

			auto i = 0, j = 0;
			char s[1];
			while(i < m1.GetNumberOfElements() && j < m2.GetNumberOfElements())
			{
				switch(strcmp(static_cast<char*>((*m1[i])[0]), static_cast<char*>((*m2[j])[0])))
				{
				case -1:
					i++;
					break;
				case 1:
					j++;
					break;
				case 0:
					Med tmp;
					strcpy_s(tmp.Name, static_cast<char*>((*m1[i])[0]));
					strcpy_s(tmp.myDate, static_cast<char*>((*m1[i])[1]));
					strcpy_s(tmp.Section, static_cast<char*>((*m1[i])[2]));
					tmp.Count = *static_cast<int*>((*m1[i])[3]);
					tmp.pr = *static_cast<double*>((*m2[j])[1]);
					m3.push_back(tmp);
					i++;
					j++;
					break;
				}
			}
			output[0] = '\0';
			for (i = 0; i < m3.GetNumberOfElements(); ++i)
			{
				char d[10];
				dtoa(d, *static_cast<double*>((*m3[i])[3]));
				char count[10];
				_itoa_s(*static_cast<int*>((*m3[i])[4]), count, 10);
				strcat_s(output, static_cast<char*>((*m3[i])[0]));
				strcat_s(output, "\t");
				strcat_s(output, static_cast<char*>((*m3[i])[1]));
				strcat_s(output, "\t");
				strcat_s(output, static_cast<char*>((*m3[i])[2]));
				strcat_s(output, "\t");
				strcat_s(output, count);
				strcat_s(output, "\t");
				strcat_s(output, d);
				strcat_s(output, "\r\n");
			}
			SetWindowTextA(OutputControl3, output);
		}
		return 0;

		case WM_COMMAND:
		{
			if (LOWORD(wParam) == 1)
			{
				char key[100], result[100];
				GetWindowTextA(SearchControl, key, 100);
				auto val = m3.find(key);
				if(val != nullptr)
				{
					char d[10], i[10];
					dtoa(d, val->pr);
					_itoa_s(val->Count, i, 10, 10);
					strcpy_s(result, val->Name);
					strcat_s(result, "\t");
					strcat_s(result, val->myDate);
					strcat_s(result, "\t");
					strcat_s(result, val->Section);
					strcat_s(result, "\t");
					strcat_s(result, d);
					strcat_s(result, "\t");
					strcat_s(result, i);
					strcat_s(result, "\r\n");
				}
				else
				{
					strcpy_s(result, "Not found :(");
				}
				MessageBoxA(nullptr, result, "Search result", MB_OK | MB_ICONINFORMATION);
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
	wc.lpszClassName = L"Medicine";
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
	{
		return EXIT_FAILURE;
	}
	const auto hwnd = CreateWindow(wc.lpszClassName,
		L"Medicine",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
		619, 279, nullptr, nullptr, wc.hInstance, nullptr);
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
