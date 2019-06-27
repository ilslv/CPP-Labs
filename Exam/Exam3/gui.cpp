#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <winuser.h>
#include <atlstr.h>
#include <iomanip>
#include <fstream>
#include "Med.h"
#include "is.h"

bool cmpDates_(char* s1, char* s2)
{
	int year1, year2, month1, month2;

	char year_s1[5], month_s1[3], tmp1[8];
	strcpy_s(tmp1, 8, s1);
	auto i1 = strchr(tmp1, '.');
	strcpy_s(year_s1, 5, i1 + 1);
	tmp1[i1 - tmp1] = '\0';
	strcpy_s(month_s1, 3, tmp1);
	month1 = strtol(month_s1, nullptr, 10);
	year1 = strtol(year_s1, nullptr, 10);

	char year_s2[5], month_s2[3], tmp2[8];
	strcpy_s(tmp2, 8, s2);
	auto i2 = strchr(tmp2, '.');
	strcpy_s(year_s2, 5, i2 + 1);
	tmp2[i2 - tmp2] = '\0';
	strcpy_s(month_s2, 3, tmp2);
	month2 = strtol(month_s2, nullptr, 10);
	year2 = strtol(year_s2, nullptr, 10);



	if (year1 == year2)
	{
		return month1 > month2;
	}
	return year1 > year2;
}

int CALLBACK wWinMain(const HINSTANCE hInstance, HINSTANCE, PWSTR lpCmdLine, const int nShowCmd)
{
	static MB1 m1;
	static MB2 m2;
	static MB3 m3;
	static ABS_Med *a1, *a2;

	static HWND SearchControl, OutputControl1, OutputControl2, OutputControl3, OutputControl4;
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
			std::ofstream outbin1("F1.bin", std::ios::binary);
			MB1 tmp1;
			Items col, dox, alp, mul;
			strcpy_s(col.Name, "Coldrex");
			strcpy_s(col.MyDate, "10.2018");
			strcpy_s(col.Section, "Medicine");
			col.Count = 10;

			strcpy_s(dox.Name, "Doxiciclin");
			strcpy_s(dox.MyDate, "11.2017");
			strcpy_s(dox.Section, "Medicine");
			dox.Count = 20;

			strcpy_s(alp.Name, "Alphabet");
			strcpy_s(alp.MyDate, "12.2008");
			strcpy_s(alp.Section, "Vitamin");
			alp.Count = 20;

			strcpy_s(mul.Name, "Multivitamin");
			strcpy_s(mul.MyDate, "05.2028");
			strcpy_s(mul.Section, "Vitamin");
			mul.Count = 40;

			tmp1.push_back(col);
			tmp1.push_back(dox);
			tmp1.push_back(alp);
			tmp1.push_back(mul);
			tmp1.WriteMeds(outbin1);
				
			outbin1.close();

			/*std::ofstream outbin2("F2.bin", std::ios::binary);
			MB2 tmp2;
			Price col, alp, mul;

			strcpy_s(col.Name, "Coldrex");
			strcpy_s(col.MyDate, "10.2018");
			col.pr = 100.20;


			strcpy_s(alp.Name, "Alphabet");
			strcpy_s(alp.MyDate, "10.2019");
			alp.pr = 20.30;

			strcpy_s(mul.Name, "Multivitamin");
			strcpy_s(mul.MyDate, "05.2018");
			mul.pr = 200.00;

			tmp2.push_back(col);
			tmp2.push_back(alp);
			tmp2.push_back(mul);
			tmp2.WriteMeds(outbin2);*/

			OutputControl1 = CreateWindowExA(NULL, "edit", nullptr,
				WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_CLIENTEDGE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
				0, 40, 300, 100, hWnd, nullptr, nullptr, nullptr);
			OutputControl2 = CreateWindowExA(NULL, "edit", nullptr,
				WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_CLIENTEDGE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
				300, 40, 300, 100, hWnd, nullptr, nullptr, nullptr);
			OutputControl3 = CreateWindowExA(NULL, "edit", nullptr,
				WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_CLIENTEDGE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
				0, 140, 600, 100, hWnd, nullptr, nullptr, nullptr);
			OutputControl4 = CreateWindowExA(NULL, "edit", nullptr,
				WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_CLIENTEDGE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
				0, 240, 600, 100, hWnd, nullptr, nullptr, nullptr);
			SearchControl = CreateWindowExA(NULL, "edit", nullptr,
				WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_CLIENTEDGE,
				260, 10, 200, 20, hWnd, nullptr, nullptr, nullptr);
			CreateWindowExA(NULL, "button", "Find",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				140, 10, 100, 20, hWnd, reinterpret_cast<HMENU>(1), nullptr, nullptr);

			std::fstream in1("F1.bin", std::ios::in);
			std::ofstream out("out.txt");
			a1 = &m1;
			a1->GetMeds(in1);
			a1->SortUp();
			a1->PutMeds(out);
			std::fstream in2("F2.bin", std::ios::in);
			a2 = &m2;
			a2->GetMeds(in2);
			a2->SortUp();
			a2->PutMeds(out);


			auto i = 0, j = 0;
			while (i < m1.GetSize() && j < m2.GetSize())
			{
				switch (strcmp(m1[i].Name, m2[j].Name))
				{
				case 1:
					j++;
					break;
				case -1:
					i++;
					break;
				case 0:
					Med tmp;
					strcpy_s(tmp.Name, m1[i].Name);
					strcpy_s(tmp.Section, m1[i].Section);
					tmp.pr = m2[j].pr;
					tmp.Count = m1[i].Count;
					if (cmpDates_(m1[i].MyDate, m2[j].MyDate))
					{
						strcpy_s(tmp.MyDate, m1[i].MyDate);
					}
					else
					{
						strcpy_s(tmp.MyDate, m2[j].MyDate);
					}
					m3.push_back(tmp);
					i++;
					j++;
					break;
				}
			}

			a1 = &m3;
			a1->PutMeds(out);
			a2 = &m3;
			a2->SortUp();
			a2->PutMeds(out);
			std::fstream outbin("res.bin", std::ios::out);
			a2->WriteMeds(outbin);


			char output[1000] = "";

			for (i = 0; i < m1.GetSize(); ++i)
			{
				char count[10];
				_itoa_s(m1[i].Count, count, 10, 10);
				is::strcat_m(output, 1000, m1[i].Name, "\t", m1[i].MyDate, "\t", m1[i].Section, "\t", count, "\r\n");
			}
			SetWindowTextA(OutputControl1, output);

			output[0] = '\0';

			for (i = 0; i < m2.GetSize(); ++i)
			{
				char pr[10];
				is::dtoa(m2[i].pr, 2, pr, 10);
				is::strcat_m(output, 1000, m2[i].Name, "\t", m2[i].MyDate, "\t", pr, "\r\n");
			}
			SetWindowTextA(OutputControl2, output);

			output[0] = '\0';

			for (i = 0; i < m3.GetSize(); ++i)
			{
				char count[10];
				char pr[10];
				is::dtoa(m3[i].pr, 2, pr, 10);
				_itoa_s(m3[i].Count, count, 10, 10);
				is::strcat_m(output, 1000, m3[i].Name, "\t", m3[i].MyDate, "\t", m3[i].Section, "\t", pr, "\t", count, "\r\n");
			}
			SetWindowTextA(OutputControl3, output);

		}
		return 0;

		case WM_COMMAND:
		{
			if (LOWORD(wParam) == 1)
			{
				char date[10], searchOut[1000] = "";
				GetWindowTextA(SearchControl, date, 10);

				for (auto i = 0; i < m3.GetSize(); ++i)
				{
					if(!cmpDates_(date, m3[i].MyDate))
					{
						char count[10];
						char pr[10];
						is::dtoa(m3[i].pr, 2, pr, 10);
						_itoa_s(m3[i].Count, count, 10, 10);
						is::strcat_m(searchOut, 1000, m3[i].Name, "\t", m3[i].MyDate, "\t", m3[i].Section, "\t", pr, "\t", count, "\r\n");
					}
				}

				SetWindowTextA(OutputControl4, searchOut);
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
		619, 379, nullptr, nullptr, wc.hInstance, nullptr);
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
