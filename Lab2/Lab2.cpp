// Lab2.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab2.h"
#include "Phrases.h"
#include "Drawer.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр


// Отправить объявления функций, включенных в этот модуль кода:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#define ROW_COUNT 5
#define COLUMN_COUNT 4

RECT clientSize;
Phrases* phrasesProvider = new Phrases();
auto phrases = phrasesProvider->phrases;
Drawer* tableDrawer = new Drawer(ROW_COUNT, COLUMN_COUNT, phrases);


int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPWSTR    lpCmdLine,
                      int       nCmdShow)
{
	WNDCLASSEXW wcex; //создаём экземпляр, для обращения к членам класса
	HWND hWnd;
	MSG msg;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Lab2Class";
	wcex.hIconSm = wcex.hIcon;

	if (!RegisterClassEx(&wcex)) {
		// в случае отсутствия регистрации класса:
		MessageBox(NULL, L"Не получилось зарегистрировать класс!", L"Ошибка", MB_OK);
		return NULL; // возвращаем, следовательно, выходим из WinMain
	}

//    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB2));
	hWnd = CreateWindow(L"Lab2Class", L"Lab2!",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		MessageBox(NULL, L"Не получилось создать окно!", L"Ошибка", MB_OK);
		return NULL;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); // интерпретируем сообщения
		DispatchMessage(&msg); // передаём сообщения обратно ОС
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lp;
			lp = (LPMINMAXINFO)lParam;
			lp->ptMinTrackSize.x = 400;
			lp->ptMinTrackSize.y = 400;
		}
		break;
    case WM_PAINT:
        {
			PAINTSTRUCT ps;
			HDC hDc = BeginPaint(hWnd, &ps);
			COLORREF color = RGB(0, 0, 0);
			const auto pen = CreatePen(PS_SOLID, 1, color);
			SelectObject(hDc, pen);

			tableDrawer->SetClientSize(clientSize);
			tableDrawer->RefreshTable(hDc);

			DeleteObject(hDc);
			EndPaint(hWnd, &ps);
        }
        break;
	case WM_SIZE:
		{
			GetClientRect(hWnd, &clientSize);
		}
		break;
    case WM_DESTROY:
	{	free(phrasesProvider);
		free(tableDrawer);
		PostQuitMessage(0);
	}
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
