#include "gui-helper.h"
#include "windows.h"
#include "strsafe.h"

const char szClass[] = "winWindowClass";

#define IDC_BUTTON_OK 101
#define IDC_BUTTON_CANCEL 102

static HWND hWndListL;
static HWND hWndButtonOk;
static HWND hWndButtonCancel;
static int selected_index = -1;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	char buffer[50] = "";

	switch(Msg)
	{
		case WM_CREATE:
			hWndListL = CreateWindowExA(
				WS_EX_LEFT,
				"LISTBOX", NULL,
				WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | LBS_NOTIFY,
				0, 0, 100, 100,
				hWnd, NULL,
				GetModuleHandle(NULL), NULL
			);

			hWndButtonOk = CreateWindowExA(
				WS_EX_LEFT,
				"BUTTON", "OK",
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_DISABLED,
				0, 0, 75, 40,
				hWnd, (HMENU)IDC_BUTTON_OK,
				GetModuleHandle(NULL), NULL
			);

			hWndButtonCancel = CreateWindowExA(
				WS_EX_LEFT,
				"BUTTON", "Cancel",
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				0, 0, 75, 40,
				hWnd, (HMENU)IDC_BUTTON_CANCEL,
				GetModuleHandle(NULL), NULL
			);

			break;
		case WM_SIZE:
		{
			HWND hEdit;
			RECT rcClient;

			GetClientRect(hWnd, &rcClient);
			SetWindowPos(hWndListL, NULL, 0, 0, rcClient.right, rcClient.bottom - 50, SWP_NOZORDER);
			SetWindowPos(hWndButtonOk, NULL, rcClient.right - (80 * 2), rcClient.bottom - 50, 75, 40, SWP_NOZORDER);
			SetWindowPos(hWndButtonCancel, NULL, rcClient.right - 80, rcClient.bottom - 50, 75, 40, SWP_NOZORDER);
		}
		case WM_COMMAND:
			switch(HIWORD(wParam))
			{
				case LBN_SELCHANGE:
					EnableWindow(hWndButtonOk, TRUE);
					break;
			}

			switch(LOWORD(wParam))
			{
				case IDC_BUTTON_OK:
				{
					LRESULT res = SendMessage(hWndListL, LB_GETCURSEL, (WPARAM)NULL, (LPARAM)NULL);
					if (res != LB_ERR)
					{
						selected_index = res;
						PostQuitMessage(WM_QUIT);
					}
					break;
				}

				case IDC_BUTTON_CANCEL:
					selected_index = -1;
					PostQuitMessage(WM_QUIT);
					break;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(WM_QUIT);
			break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

int ShowMessageBox(const char* title, const char* message)
{
	return MessageBox(NULL, title, message, MB_OK | MB_ICONEXCLAMATION);
}

int SelectionDialog(const char* title, int count, const char** list, int selection)
{
	MSG Msg;
	HWND hWnd;

	HINSTANCE hInstance = GetModuleHandle(NULL);

	//Settings All Window Class Variables
	WNDCLASSEXA wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClass;

	//Register Window Class
	RegisterClassEx(&wc);

	//Create Window
	hWnd = CreateWindowExA(
		WS_EX_CLIENTEDGE,
		szClass,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		454, 388,
		NULL, NULL,
		hInstance, NULL
	);

	// Add all games to the list
	for (int i = 0; i < count; ++i)
	{
		SendMessage(hWndListL, LB_ADDSTRING, (WPARAM)NULL, (LPARAM)list[i]);
	}

	// Show the window
	ShowWindow(hWnd, SW_SHOWNORMAL);

	// Loop until completion
	while(GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}


	return selected_index;
}