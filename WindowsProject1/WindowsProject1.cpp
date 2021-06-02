// WindowsProject1.cpp : Definiuje punkt wejścia dla aplikacji.
//

#include "framework.h"
#include "WindowsProject1.h"
#pragma comment (lib,"Gdiplus.lib")

using namespace std;
using namespace Gdiplus;


#define MAX_LOADSTRING 100

//do przeniesienia deklaracje nazw funkcji (do pliku naglówkowego .h)
void repaintWindow(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea, float arm_position, float hand_position);
VOID OnPaint(HDC hdc, float arm_position, float hand_position);
//koniec
HWND arm_down, arm_up, hand_down, hand_up; //arm to ramie dolne a up to ramie gorne

// Zmienne globalne:
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego
int timer = 0;

// Przekaż dalej deklaracje funkcji dołączone w tym module kodu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: W tym miejscu umieść kod.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    // Inicjuj ciągi globalne
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Wykonaj inicjowanie aplikacji:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Główna pętla komunikatów:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    GdiplusShutdown(gdiplusToken);

    return (int) msg.wParam;
}



//
//  FUNKCJA: MyRegisterClass()
//
//  PRZEZNACZENIE: Rejestruje klasę okna.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKCJA: InitInstance(HINSTANCE, int)
//
//   PRZEZNACZENIE: Zapisuje dojście wystąpienia i tworzy okno główne
//
//   KOMENTARZE:
//
//        W tej funkcji dojście wystąpienia jest zapisywane w zmiennej globalnej i
//        jest tworzone i wyświetlane okno główne programu.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNKCJA: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PRZEZNACZENIE: Przetwarza komunikaty dla okna głównego.
//
//  WM_COMMAND  - przetwarzaj menu aplikacji
//  WM_PAINT    - Maluj okno główne
//  WM_DESTROY  - opublikuj komunikat o wyjściu i wróć
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    const double pi = 3.1415926535897932384626433832795;
    float arm_position = pi;
    float hand_position = pi;
    switch (message)
    {
    case WM_CREATE:
        arm_down = CreateWindow(TEXT("button"), TEXT("Ramie dolne w prawo"),
            WS_VISIBLE | WS_CHILD,
            20, 50, 240, 25,
            hWnd, (HMENU)IDARM_DOWN, NULL, NULL);

        arm_up = CreateWindow(TEXT("button"), TEXT("Ramie dolne w lewo"),
            WS_VISIBLE | WS_CHILD,
            280, 50, 240, 25,
            hWnd, (HMENU)IDARM_UP, NULL, NULL);

        hand_down = CreateWindow(TEXT("button"), TEXT("Ramie gorne w prawo"),
            WS_VISIBLE | WS_CHILD,
            540, 50, 240, 25,
            hWnd, (HMENU)IDHAND_DOWN, NULL, NULL);

        hand_up = CreateWindow(TEXT("button"), TEXT("Ramie gorne w lewo"),
            WS_VISIBLE | WS_CHILD,
            800, 50, 240, 25,
            hWnd, (HMENU)IDHAND_UP, NULL, NULL);

        break;
    case WM_TIMER:
        repaintWindow(hWnd, hdc, ps , NULL,0,0);
        timer++;
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizuj zaznaczenia menu:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDARM_DOWN:
                MessageBox(NULL, TEXT("button_one_clicked"), TEXT("mleko"), MB_OK | MB_ICONINFORMATION);
                arm_position = arm_position - pi/8;
                repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
                break;
            case IDARM_UP:
                MessageBox(NULL, TEXT("button_two_clicked"), TEXT("kakao"), MB_OK | MB_ICONINFORMATION);
                arm_position = arm_position + pi / 8;
                repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
                break;
            case IDHAND_DOWN:
                MessageBox(NULL, TEXT("button_three_clicked"), TEXT("kawa"), MB_OK | MB_ICONINFORMATION);
                hand_position = hand_position - pi/8;
                repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
                break;
            case IDHAND_UP:
                MessageBox(NULL, TEXT("button_four_clicked"), TEXT("czelolada"), MB_OK | MB_ICONINFORMATION);
                hand_position = hand_position + pi/8;
                repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            // TODO: Tutaj dodaj kod rysujący używający elementu hdc...
            repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Procedura obsługi komunikatów dla okna informacji o programie.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// funkcje do przeniesiena do innego cpp

void repaintWindow(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea, float arm_position, float hand_position)
{
    if (drawArea == NULL)
        InvalidateRect(hWnd, NULL, TRUE); // repaint all
    else
        InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
    hdc = BeginPaint(hWnd, &ps);
    OnPaint(hdc, arm_position, hand_position);
    EndPaint(hWnd, &ps);
}

VOID OnPaint(HDC hdc, float arm_position,float hand_position)
{
    int hook_x = 300;
    int hook_y = 400;
    int arm_length = 100;
    int hand_length = 100;
    int arm_position_x, arm_position_y, hand_position_x, hand_position_y;
    arm_position_x = arm_length * cos(arm_position) + hook_x;
    arm_position_y = arm_length * sin(arm_position) + hook_y;
    hand_position_x = hand_length * cos(hand_position) + arm_position_x;
    hand_position_y = hand_length * sin(hand_position) + arm_position_y;
    Graphics graphics(hdc);
    // Create a Pen object.
    Pen blackPen(Color(255, 0, 0, 0), 3);
    SolidBrush greenBrush(Color(255, 0, 200, 50));

    // Create an array of Rect objects.
    Rect rect1(350, 350, 10, 100);
    Rect rect2(400, 300, 10, 150);
    Rect rect3(450, 250, 10, 200);
    Rect rect4(500, 400, 10, 50);
    Rect rect5(550, 200, 10, 250);

    Rect rects[] = { rect1, rect2, rect3, rect4, rect5 };
    Rect* pRects = rects;
    // Draw the rectangles.
    graphics.DrawRectangles(&blackPen, pRects, 5);
    graphics.FillRectangles(&greenBrush, rects, 5);

     graphics.DrawLine(&blackPen, hook_x, hook_y, arm_position_x, arm_position_y); //wyswqietlanie reki
     graphics.DrawLine(&blackPen, arm_position_x, arm_position_y, hand_position_x, hand_position_y);//wyswietlanie dloni
}

