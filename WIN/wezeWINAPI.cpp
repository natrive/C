#include<windows.h>
#define BUFFER(x,y) * (pBuffer+y * cxBuffer +x)
#define ROZMIAR 20

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Typer");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Graficzne Weze"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;

}

short wiersz = 0, kolumna = 0, iteracje = 0;
bool obrot = false;
char pom = 'A';
const char* znak = &pom;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static DWORD dwCharSet = DEFAULT_CHARSET;
	static int cxChar, cyChar, cxClient, cyClient, cxBuffer, cyBuffer, xCaret, yCaret;
	static TCHAR * pBuffer = NULL;
	HDC hdc;
	int x, y, i;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
	
	switch (message)
	{
		case WM_INPUTLANGCHANGE:
			dwCharSet = wParam;

		case WM_CREATE:
			hdc = GetDC(hwnd);
			SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));
			GetTextMetrics(hdc, &tm);
			cxChar = tm.tmAveCharWidth;
			cyChar = tm.tmHeight;
			DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
			ReleaseDC(hwnd, hdc);

		case WM_SIZE:
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			cxBuffer = max(1, cxClient / cxChar);
			cyBuffer = max(1, cyClient / cyChar);
			if (pBuffer != NULL) free(pBuffer);
			pBuffer = (TCHAR *)malloc(cxBuffer * cyBuffer * sizeof(TCHAR));
			for (y = 0; y < cyBuffer; y++)
				for (x = 0; x < cxBuffer; x++)
				{
					BUFFER(x, y) = ' ';
				}
			xCaret = 0;
			yCaret = 0;
			if (hwnd = GetFocus()) SetCaretPos(xCaret * cxChar, yCaret * cyChar);
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;

		case WM_SETFOCUS:
			CreateCaret(hwnd, NULL, cxChar, cyChar);
			SetCaretPos(xCaret * cxChar, yCaret * cyChar);
			ShowCaret(hwnd);
			return 0;

		case WM_KILLFOCUS:
			HideCaret(hwnd);
			DestroyCaret();
			return 0;

		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_HOME: // klawisz HOME - przesuwanie kursora

					BUFFER(xCaret, yCaret) = (TCHAR)wParam;
					HideCaret(hwnd);
					hdc = GetDC(hwnd);
					SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));

					// przypisz wartości dla rysowania gornej krawedzi
					// lewy górny róg
					if (kolumna == iteracje && wiersz == iteracje && obrot == true)
					{
						iteracje++;
						obrot = false;
						kolumna++;
						wiersz++;
						pom = znak[0] + 1;
					}
					// góra
					else if (kolumna < ROZMIAR - iteracje && wiersz == iteracje)
					{
						kolumna++;
					}
					// górna krawędź

					// przypisz wartości dla rysowania prawej krawedzi
					// prawy górny róg
					if (kolumna == ROZMIAR - iteracje && wiersz == iteracje)
					{
						kolumna--;
						pom = znak[0] + 1;
						wiersz++;
					}
					// prawo
					else if (wiersz > iteracje && wiersz < ROZMIAR + 1 - iteracje && kolumna == ROZMIAR - 1 - iteracje)
					{
						pom = znak[0] + 1;
						wiersz++;
					}
					// prawa krawędź

					//przypisz wartości dla rysowania dolnej krawędzi
					// prawy dolny róg
					if (wiersz == ROZMIAR - iteracje && kolumna == ROZMIAR - 1 - iteracje)
					{
						pom = znak[0] - 1;
						kolumna--;
						wiersz--;
					}
					// dół
					else if (wiersz == ROZMIAR - 1 - iteracje && kolumna > -1 + iteracje && kolumna < ROZMIAR - 1 - iteracje)
					{
						kolumna--;
					}
					// dolna krawedz

					// przypisz wartosci dla rysowania lewej krawędzi
					// lewy dolny róg
					if (kolumna == -1 + iteracje && wiersz == ROZMIAR - 1 - iteracje)
					{
						pom = znak[0] - 1;
						wiersz--;
						kolumna++;
						obrot = true;
					}
					// lewo
					else if (kolumna == iteracje && wiersz > iteracje && wiersz < ROZMIAR - 1 - iteracje)
					{
						pom = znak[0] - 1;
						wiersz--;
					}
					// lewa krawędź

					//zmiana pozycji kursora
					xCaret = kolumna;
					yCaret = wiersz;

					//drukowanie litery
					TextOut(hdc, xCaret * cxChar, yCaret * cyChar, znak, 1);
					DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
					ReleaseDC(hwnd, hdc);
					ShowCaret(hwnd);
					break;

			}
			SetCaretPos(xCaret*cxChar, yCaret*cyChar);
			return 0;

		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));
			TextOut(hdc, 0, 0, znak, 1);
			DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
			EndPaint(hwnd, &ps);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);

}