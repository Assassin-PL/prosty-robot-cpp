// WindowsProject1.cpp : Definiuje punkt wejścia dla aplikacji.
#include "framework.h"
#include "WindowsProject1.h"
#pragma comment (lib,"Gdiplus.lib")

using namespace std;
using namespace Gdiplus;

#define MAX_LOADSTRING 100
//klasay
class Object
{
public:
    bool is_hold;//czy jest trzymany ten obiekt
    bool is_falling;// czy spada
    bool is_collison;//czy jest wykryta kolizja z innym obiektem z listy obiektów
    bool is_attached;// czy styka się z podłożem
    int x, y;//koordynaty punktu zacczepienia obniektu
    Object(bool a, bool b, bool c, bool d, int e, int f);
    ~Object();

private:

};

Object::Object(bool a, bool b, bool c, bool d, int e, int f)
{
    is_hold = a;
    is_falling = b;
    is_collison = c;
    is_attached = d;
    x = e;
    y = f;
}

Object::~Object()
{

}

//do przeniesienia deklaracje nazw funkcji (do pliku naglówkowego .h)
void repaintWindow(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea, float& arm_position, float& hand_position);
VOID OnPaint(HDC hdc, float& arm_position, float& hand_position);
//koniec
HWND arm_down, arm_up, hand_down, hand_up; //arm to ramie dolne a up to ramie gorne

// Zmienne globalne:
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego
int timer = 0;
const double pi = 3.1415926535897932384626433832795;
float arm_position = 0;
float hand_position = 0;
// Zmienne określające rysowanie
const int free_space = 50;
const int length = 25;
const int hook_x = 300;
const int hook_y = 400;
const int arm_length = 100;
const int hand_length = 100;
list <Object> object;
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
    if (!InitInstance(hInstance, nCmdShow))
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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
        repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
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
            //MessageBox(NULL, TEXT("button_one_clicked"), TEXT("mleko"), MB_OK | MB_ICONINFORMATION);
            for (int i = 1; i <= 64; i++) {
                int arm_position_x, arm_position_y, hand_position_x, hand_position_y;
                arm_position_x = arm_length * cos(arm_position) + hook_x;
                arm_position_y = arm_length * sin(arm_position) + hook_y;
                hand_position_x = hand_length * cos(hand_position + arm_position) + arm_position_x;
                hand_position_y = hand_length * sin(hand_position + arm_position) + arm_position_y;
                arm_position = arm_position + pi / 512;
                if ((arm_position>0 && arm_position<pi)||arm_position<-pi&&arm_position>-2*pi||hand_position_y>=hook_y) {
                    MessageBox(NULL, TEXT("Nie mozna wykonac dalszego ruchu!"), TEXT("za daleko"), MB_OK | MB_ICONINFORMATION);
                    break;
                }
                repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
                Sleep(16.66666666666);
            }
            break;
        case IDARM_UP:
            //MessageBox(NULL, TEXT("button_two_clicked"), TEXT("kakao"), MB_OK | MB_ICONINFORMATION);
            for (int i = 1; i <= 64; i++) {
                int arm_position_x, arm_position_y, hand_position_x, hand_position_y;
                arm_position_x = arm_length * cos(arm_position) + hook_x;
                arm_position_y = arm_length * sin(arm_position) + hook_y;
                hand_position_x = hand_length * cos(hand_position + arm_position) + arm_position_x;
                hand_position_y = hand_length * sin(hand_position + arm_position) + arm_position_y;
                arm_position = arm_position - pi / 512;
                if ((arm_position>0 && arm_position<pi)||arm_position<-pi&&arm_position>-2*pi || hand_position_y >= hook_y) {
                    MessageBox(NULL, TEXT("Nie mozna wykonac dalszego ruchu!"), TEXT("za daleko"), MB_OK | MB_ICONINFORMATION);
                    break;
                }
                repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
                Sleep(16.66666666666);
            }
            break;
        case IDHAND_DOWN:
            //MessageBox(NULL, TEXT("button_three_clicked"), TEXT("kawa"), MB_OK | MB_ICONINFORMATION);
            for (int i = 1; i <= 64; i++) {
                int arm_position_x, arm_position_y, hand_position_x, hand_position_y;
                arm_position_x = arm_length * cos(arm_position) + hook_x;
                arm_position_y = arm_length * sin(arm_position) + hook_y;
                hand_position_x = hand_length * cos(hand_position + arm_position) + arm_position_x;
                hand_position_y = hand_length * sin(hand_position + arm_position) + arm_position_y;
                hand_position = hand_position - pi / 512;
                if ((arm_position > 0 && arm_position < pi) || arm_position<-pi && arm_position>-2 * pi || hand_position_y >= hook_y) {
                    MessageBox(NULL, TEXT("Nie mozna wykonac dalszego ruchu!"), TEXT("za daleko"), MB_OK | MB_ICONINFORMATION);
                    break;
                }
                repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
                Sleep(16.66666666666);
            }
            break;
        case IDHAND_UP:
            //MessageBox(NULL, TEXT("button_four_clicked"), TEXT("czelolada"), MB_OK | MB_ICONINFORMATION);
            for (int i = 1; i <= 64; i++) {
                int arm_position_x, arm_position_y, hand_position_x, hand_position_y;
                arm_position_x = arm_length * cos(arm_position) + hook_x;
                arm_position_y = arm_length * sin(arm_position) + hook_y;
                hand_position_x = hand_length * cos(hand_position + arm_position) + arm_position_x;
                hand_position_y = hand_length * sin(hand_position + arm_position) + arm_position_y;
                hand_position = hand_position + pi / 512;
                if ((arm_position > 0 && arm_position < pi) || arm_position<-pi && arm_position>-2 * pi || hand_position_y >= hook_y) {
                    MessageBox(NULL, TEXT("Nie mozna wykonac dalszego ruchu!"), TEXT("za daleko"), MB_OK | MB_ICONINFORMATION);
                    break;
                }
                repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
                Sleep(16.6666666666);
            }
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

void repaintWindow(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea, float& arm_position, float& hand_position)
{
    if (drawArea == NULL)
        InvalidateRect(hWnd, NULL, TRUE); // repaint all
    else
        InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
    hdc = BeginPaint(hWnd, &ps);
    OnPaint(hdc, arm_position, hand_position);
    EndPaint(hWnd, &ps);
}

VOID OnPaint(HDC hdc, float& arm_position, float& hand_position)
{
    int arm_position_x, arm_position_y, hand_position_x, hand_position_y;
    arm_position_x = arm_length * cos(arm_position) + hook_x;
    arm_position_y = arm_length * sin(arm_position) + hook_y;
    hand_position_x = hand_length * cos(hand_position + arm_position) + arm_position_x;
    hand_position_y = hand_length * sin(hand_position + arm_position) + arm_position_y;
    Graphics graphics(hdc);
    // Create a Pen object.
    Pen blackPen(Color(255, 0, 0, 0), 3);
    Pen bluePen(Color(255, 21, 235, 220), 3);
    Pen redPen(Color(255, 245, 99, 66), 3);
    SolidBrush greenBrush(Color(255, 0, 200, 50));

    // Create an array of Rect objects.
    Rect rect1((hook_x + (2 * free_space) - 1), hook_y - length, length, length);
    Rect rect2((hook_x + (3 * free_space) - 1), hook_y - length, length, length);
    Rect rect3((hook_x + (4 * free_space) - 1), hook_y - length, length, length);
    Rect rect4((hook_x - (2 * free_space) - 1), hook_y - length, length, length);
    Rect rect5((hook_x - (3 * free_space) - 1), hook_y - length, length, length);
    Rect rect6((hook_x - (4 * free_space) - 1), hook_y - length, length, length);

    Rect rects[] = { rect1, rect2, rect3, rect4, rect5, rect6 };
    Rect* pRects = rects;
    // Draw the rectangles.
    graphics.DrawRectangles(&blackPen, pRects, 6);
    graphics.FillRectangles(&greenBrush, rects, 6);
    // rysowanie reki
    graphics.DrawLine(&blackPen, 0, hook_y, 1920, hook_y );
    graphics.DrawLine(&bluePen, hook_x, hook_y, arm_position_x, arm_position_y); //wyswqietlanie reki
    graphics.DrawLine(&redPen, arm_position_x, arm_position_y, hand_position_x, hand_position_y);//wyswietlanie dloni
    //robienie listy
    int size_of_table = 6;
    for (int i = 0; i < size_of_table; i++)
    {
        Object obj1(0 ,0 ,0 ,1,rects[i].X, rects[i].Y);
        object.push_back(obj1);
    }
}
// klasa obiekt
// 1. musi wykrywac kolizje
// 2. musi spadac (czy jest kontakt z podłożem czy nie)
// 3. musi wykryc czy jest sie chwyconym
// 4.  
