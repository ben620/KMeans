#include "StdAfx.h"
#include "basetype.h"
#include "KMeans.h"

using namespace std;
using namespace algo;
using namespace base_type;


vector<Point2T<float> > g_vPoint(16);

template <class ForwardIterator>
void Render(HDC hDC, ForwardIterator begin, ForwardIterator end, int K)
{
	typedef typename ForwardIterator::value_type value_type;

	vector<HBRUSH> vBrush(K);
	int n = 0;
	generate_n(vBrush.begin(), K, [&n]()
		{
			return CreateSolidBrush(RGB(256 - n * 64, (n++) * 64, 128));
		}
	);
	
	for_each(begin, end, [&hDC, &vBrush](value_type & pt)
		{
			RECT rc;
			rc.left = LONG(pt.x);
			rc.right = LONG(pt.x + 8);
			rc.bottom = LONG(pt.y + 8);
			rc.top    = LONG(pt.y);
			FillRect(hDC, &rc, vBrush[pt.ID]);
		}
	);

	for_each(vBrush.begin(), vBrush.end(), [&](HBRUSH &brush)
		{
			DeleteObject(brush);
		}
	);
}

void OnSize(HWND hWnd, WPARAM wParam, int cx, int cy)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	InvalidateRect(hWnd, &rc, TRUE);
}

LRESULT CALLBACK WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rc;
	switch (msg)
	{
	case WM_CREATE:
#if 0
		srand(GetTickCount());
		generate(g_vPoint.begin(), g_vPoint.end(), [&]()
			{
				return Point2T<float>(float(rand() % 600), float(rand() % 600));
			}
		);
#else
		for (size_t ii = 0; ii < g_vPoint.size(); ++ii)
		{
			if (ii < 8)
			{
				g_vPoint[ii].x = rand() % 100;
				g_vPoint[ii].y = rand() % 100;
			}
			else
			{
				g_vPoint[ii].x = rand() % 100 + 300;
				g_vPoint[ii].y = rand() % 100 + 300;
			}
		}
#endif
		return 0;
	case WM_LBUTTONUP:
		KMeans(g_vPoint.begin(), g_vPoint.end(), 2);

		GetClientRect(hWnd, &rc);
		InvalidateRect(hWnd, &rc, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Render(hdc, g_vPoint.begin(), g_vPoint.end(), 2);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_SIZE:
		HANDLE_WM_SIZE(hWnd, wParam, lParam, OnSize);
		return 0;
	case WM_ERASEBKGND:
		GetClientRect(hWnd, &rc);
		hdc = GetDC(hWnd);
		FillRect(hdc, &rc, WHITE_BRUSH);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);

	}
	return LRESULT(0);
}

TCHAR szClassName[] = TEXT("KMeans");


int WINAPI WinMain(HINSTANCE hPrev, HINSTANCE hInst, LPSTR lpCmd, int nCmd)
{
	WNDCLASS wc;
	wc.cbClsExtra     = 0;
	wc.cbWndExtra     = 0;
	wc.hbrBackground  = WHITE_BRUSH;
	wc.hCursor        = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wc.hIcon          = ::LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.lpszClassName  = szClassName;
	wc.lpszMenuName   = NULL;
	wc.style          = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc    = WindowProcess;

	RegisterClass(&wc);

	HWND hWnd = CreateWindow(szClassName, TEXT("KMeans"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, 0);
	
	ShowWindow(hWnd, nCmd);
	UpdateWindow(hWnd);

	MSG msg;

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.lParam;
}