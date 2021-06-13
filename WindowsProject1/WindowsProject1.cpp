// WindowsProject1.cpp : Definiuje punkt wejścia dla aplikacji.
#include "framework.h"
#include "WindowsProject1.h"
#pragma comment (lib,"Gdiplus.lib")

using namespace std;
using namespace Gdiplus;

#define MAX_LOADSTRING 100

class Object
{
public:
    Rect rectangle; //dodaje klocka
    bool is_hold;//czy jest trzymany ten obiekt
    bool is_falling;// czy spada
    bool is_collison;//czy jest wykryta kolizja z innym obiektem z listy obiektów
    bool is_attached;// czy styka się z podłożem
    int x, y;//koordynaty punktu zacczepienia obniektu
    Object(bool a, bool b, bool c, bool d, int e, int f, Rect dupa);
    ~Object();
    void set_object(bool a, bool b, bool c, bool d, int e, int f, Rect dupa);
    void fall(int width, int height);
    void change_possition(int dx, int dy, int width, int height);
    void change_possition2(int dx, int dy, int width, int height);
private:

};

Object::Object(bool a, bool b, bool c, bool d, int e, int f, Rect dupa)
{
    set_object(a, b, c, d, e, f, dupa);
}

Object::~Object()
{

}

void Object::set_object(bool a, bool b, bool c, bool d, int e, int f, Rect dupa)
{
    is_hold = a;
    is_falling = b;
    is_collison = c;
    is_attached = d;
    x = e;
    y = f;
    rectangle = dupa;
}
void Object::fall(int width, int height)
{
    x += 1;
    y += 1;
    Rect dupa(x, y, width, height);
    rectangle = dupa;
}

void Object::change_possition(int dx, int dy, int width, int height)
{
    x = x + dx;
    y = y - dy;
    Rect dupa(x, y, width, height);
    rectangle = dupa;
}
void Object::change_possition2(int dx, int dy, int width, int height)
{
    x = dx;
    y = dy;
    Rect dupa(x, y, width, height);
    rectangle = dupa;
}
//do przeniesienia deklaracje nazw funkcji (do pliku naglówkowego .h)
void repaintWindow(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea, float& arm_position, float& hand_position);
void make_collision(list<Object>& object);
void make_hold_oobject(int hand_position_x, int hand_position_y);
void end_collision(list<Object>& object);
VOID OnPaint(HDC hdc, float& arm_position, float& hand_position);
VOID PAINT_RECTS();
void which_is_hold(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea, float& arm_position, float& hand_position, int x, int y, int ddx, int ddy, int dx, int dy);
list<Object>::iterator get_itterator_of_object(list<Object>& object, int x, int y);
list<Object>::iterator get_itterator_of_object_in_area(list<Object>& object, int x, int y);
list<Object>::iterator get_itterator_of_falling_object(list<Object>& object);
bool is_in_area_of_object(list<Object>& object, int x, int y);
bool is_fall(list<Object>& object);
bool is_collision(list<Object>& object);
bool is_attached(list<Object>& object);
//int algorytm_losujacy(int beg, int end);

//koniec
HWND arm_down, arm_up, hand_down, hand_up, hold, drop; //arm to ramie dolne a up to ramie gorne

// Zmienne globalne:
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego
int timer = 0;
const double pi = 3.1415926535897932384626433832795;
float arm_position = 0;
float hand_position = 0;
bool holding = false;
int ddx = 0;
int ddy = 0;
// Zmienne określające rysowanie
const int free_space = 50;
const int length = 25;
const int hook_x = 300;
const int hook_y = 400;
const int arm_length = 150;
const int hand_length = 150;
// inicjalizowanie listy obiektow
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
    PAINT_RECTS();

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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; 

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    switch (message)
    {
    case WM_CREATE:
        hold = CreateWindow(TEXT("button"), TEXT("Zlap lapa"),
            WS_VISIBLE | WS_CHILD,
            280, 100, 240, 25,
            hWnd, (HMENU)IDHOLD, NULL, NULL);

        drop = CreateWindow(TEXT("button"), TEXT("upusc lapa"),
            WS_VISIBLE | WS_CHILD,
            540, 100, 240, 25,
            hWnd, (HMENU)IDDROP, NULL, NULL);

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
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDHOLD:
            MessageBox(NULL, TEXT("button_four_clicked"), TEXT("czelolada"), MB_OK | MB_ICONINFORMATION);
            holding = true;
            int arm_position_x, arm_position_y, hand_position_x, hand_position_y;
            arm_position_x = arm_length * cos(arm_position) + hook_x;
            arm_position_y = arm_length * sin(arm_position) + hook_y;
            hand_position_x = hand_length * cos(hand_position + arm_position) + arm_position_x;
            hand_position_y = hand_length * sin(hand_position + arm_position) + arm_position_y;
            if (is_in_area_of_object(object, hand_position_x, hand_position_y) == true) {
                list<Object>::iterator wsk_object;
                wsk_object = get_itterator_of_object_in_area(object, hand_position_x, hand_position_y);
                ddx = hand_position_x - wsk_object->x;
                ddy = hand_position_y - wsk_object->y;
            }
            break;
        case IDDROP:
            MessageBox(NULL, TEXT("button_two_clicked"), TEXT("kakao"), MB_OK | MB_ICONINFORMATION);
            holding = false;
            if (is_fall(object) == true)
            {
                list<Object>::iterator wsk_object;
                wsk_object = get_itterator_of_falling_object(object);
                while (is_fall(object) == true && is_attached(object) == false)
                {
                    make_collision(object);
                    wsk_object->y += 1;
                    wsk_object->change_possition(0, -1, length, length);
                    if (wsk_object->is_collison == true)
                    {
                        wsk_object->y -= 2;
                        wsk_object->change_possition(0, 2, length, length);
                        wsk_object->is_attached = true;
                        end_collision(object);
                        break;
                    }
                    if ((wsk_object->y + length) == hook_y)
                    {
                        wsk_object->y -= 1;
                        wsk_object->change_possition(0, 1, length, length);
                        wsk_object->is_attached = true;
                        break;
                    }
                    repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
                    Sleep(16.66666666666 / 16);
                }
            }
            break;
        case IDARM_DOWN:
            for (int i = 1; i <= 32; i++) {
                int arm_position_x, arm_position_y, hand_position_x, hand_position_y;
                arm_position_x = arm_length * cos(arm_position) + hook_x;
                arm_position_y = arm_length * sin(arm_position) + hook_y;
                hand_position_x = hand_length * cos(hand_position + arm_position) + arm_position_x;
                hand_position_y = hand_length * sin(hand_position + arm_position) + arm_position_y;

                if ((arm_position > 0 && arm_position < pi) || arm_position<-pi && arm_position>-2 * pi || hand_position_y > hook_y) {
                    MessageBox(NULL, TEXT("Nie mozna wykonac dalszego ruchu!"), TEXT("za daleko"), MB_OK | MB_ICONINFORMATION);
                    arm_position = arm_position - pi / 512;
                    break;
                }
                arm_position = arm_position + pi / 512;
                //repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
                if (holding == 1 && is_in_area_of_object(object, hand_position_x, hand_position_y) == true)
                {
                    int dx, dy;
                    dx = hand_length * cos(hand_position + arm_position) + arm_length * cos(arm_position) + hook_x - hand_position_x;
                    dy = hand_length * sin(hand_position + arm_position) + arm_length * sin(arm_position) + hook_y - hand_position_y;
                    make_hold_oobject(hand_position_x, hand_position_y);
                    make_collision(object);
                    which_is_hold(hWnd, hdc, ps, NULL, arm_position, hand_position, hand_position_x, hand_position_y, ddx, ddy, dx, dy);
                }
                repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
                Sleep(16.66666666666);
            }
            break;
        case IDARM_UP:
            for (int i = 1; i <= 32; i++) {
                int arm_position_x, arm_position_y, hand_position_x, hand_position_y;
                arm_position_x = arm_length * cos(arm_position) + hook_x;
                arm_position_y = arm_length * sin(arm_position) + hook_y;
                hand_position_x = hand_length * cos(hand_position + arm_position) + arm_position_x;
                hand_position_y = hand_length * sin(hand_position + arm_position) + arm_position_y;
                if ((arm_position > 0 && arm_position < pi) || arm_position<-pi && arm_position>-2 * pi || hand_position_y > hook_y) {
                    MessageBox(NULL, TEXT("Nie mozna wykonac dalszego ruchu!"), TEXT("za daleko"), MB_OK | MB_ICONINFORMATION);
                    arm_position = arm_position + pi / 512;
                    break;
                }
                arm_position = arm_position - pi / 512;
                if (holding == 1 && is_in_area_of_object(object, hand_position_x, hand_position_y) == true)
                {
                    int dx, dy;
                    dx = hand_length * cos(hand_position + arm_position) + arm_length * cos(arm_position) + hook_x - hand_position_x;
                    dy = hand_length * sin(hand_position + arm_position) + arm_length * sin(arm_position) + hook_y - hand_position_y;
                    make_hold_oobject(hand_position_x, hand_position_y);
                    make_collision(object);
                    which_is_hold(hWnd, hdc, ps, NULL, arm_position, hand_position, hand_position_x, hand_position_y, ddx, ddy, dx, dy);
                }
                repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
                Sleep(16.66666666666);
            }
            break;
        case IDHAND_DOWN:
            
            for (int i = 1; i <= 32; i++) {
                int arm_position_x, arm_position_y, hand_position_x, hand_position_y;
                arm_position_x = arm_length * cos(arm_position) + hook_x;
                arm_position_y = arm_length * sin(arm_position) + hook_y;
                hand_position_x = hand_length * cos(hand_position + arm_position) + arm_position_x;
                hand_position_y = hand_length * sin(hand_position + arm_position) + arm_position_y;

                if (hand_position_y > hook_y) {
                    MessageBox(NULL, TEXT("Nie mozna wykonac dalszego ruchu!"), TEXT("za daleko"), MB_OK | MB_ICONINFORMATION);
                    hand_position = hand_position - pi / 512;
                    break;
                }
                hand_position = hand_position + pi / 512;
                if (holding == 1 && is_in_area_of_object(object, hand_position_x, hand_position_y) == true)
                {
                    int dx, dy;
                    dx = hand_length * cos(hand_position + arm_position) + arm_length * cos(arm_position) + hook_x - hand_position_x;
                    dy = hand_length * sin(hand_position + arm_position) + arm_length * sin(arm_position) + hook_y - hand_position_y;
                    make_hold_oobject(hand_position_x, hand_position_y);
                    make_collision(object);
                    which_is_hold(hWnd, hdc, ps, NULL, arm_position, hand_position, hand_position_x, hand_position_y, ddx, ddy, dx, dy);
                }
                repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
                Sleep(16.6666666666);
            }
            break;
        case IDHAND_UP:
            for (int i = 1; i <= 32; i++) {
                int arm_position_x, arm_position_y, hand_position_x, hand_position_y;
                arm_position_x = arm_length * cos(arm_position) + hook_x;
                arm_position_y = arm_length * sin(arm_position) + hook_y;
                hand_position_x = hand_length * cos(hand_position + arm_position) + arm_position_x;
                hand_position_y = hand_length * sin(hand_position + arm_position) + arm_position_y;

                if (hand_position_y > hook_y) {
                    MessageBox(NULL, TEXT("Nie mozna wykonac dalszego ruchu!"), TEXT("za daleko"), MB_OK | MB_ICONINFORMATION);
                    hand_position = hand_position + pi / 512;
                    break;
                }
                hand_position = hand_position - pi / 512;
                if (holding == 1 && is_in_area_of_object(object, hand_position_x, hand_position_y) == true)
                {
                    int dx, dy;
                    dx = hand_length * cos(hand_position + arm_position) + arm_length * cos(arm_position) + hook_x - hand_position_x;
                    dy = hand_length * sin(hand_position + arm_position) + arm_length * sin(arm_position) + hook_y - hand_position_y;
                    make_hold_oobject(hand_position_x, hand_position_y);
                    make_collision(object);
                    which_is_hold(hWnd, hdc, ps, NULL, arm_position, hand_position, hand_position_x, hand_position_y, ddx, ddy, dx, dy);
                }
                repaintWindow(hWnd, hdc, ps, NULL, arm_position, hand_position);
                Sleep(16.66666666666);
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


void repaintWindow(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea, float& arm_position, float& hand_position)
{
    if (drawArea == NULL)
        InvalidateRect(hWnd, NULL, TRUE); 
    else
        InvalidateRect(hWnd, drawArea, TRUE); 
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

    Pen blackPen(Color(255, 0, 0, 0), 3);
    Pen bluePen(Color(255, 21, 235, 220), 3);
    Pen redPen(Color(255, 245, 99, 66), 3);
    SolidBrush greenBrush(Color(255, 0, 200, 50));
    list<Object>::iterator j = object.begin();
    Rect rects[6];
    for (int i = 0; i < 6; i++)
    {
        rects[i] = get_itterator_of_object(object, j->x, j->y)->rectangle;
        j++;
    }
    Rect* pRects = rects;
    graphics.DrawRectangles(&blackPen, pRects, 6);
    graphics.FillRectangles(&greenBrush, rects, 6);
    // rysowanie reki
    graphics.DrawLine(&blackPen, 0, hook_y, 1920, hook_y);
    graphics.DrawLine(&bluePen, hook_x, hook_y, arm_position_x, arm_position_y); //wyswqietlanie reki
    graphics.DrawLine(&redPen, arm_position_x, arm_position_y, hand_position_x, hand_position_y);//wyswietlanie dloni
}

VOID PAINT_RECTS()
{
    Rect rect1((hook_x + (2 * free_space) - 1), hook_y - length, length, length);
    Rect rect2((hook_x + (3 * free_space) - 1), hook_y - length, length, length);
    Rect rect3((hook_x + (4 * free_space) - 1), hook_y - length, length, length);
    Rect rect4((hook_x - (2 * free_space) - 1), hook_y - length, length, length);
    Rect rect5((hook_x - (3 * free_space) - 1), hook_y - length, length, length);
    Rect rect6((hook_x - (4 * free_space) - 1), hook_y - length, length, length);
    //robienie listy
    Object obj1(0, 0, 0, 1, (hook_x + (2 * free_space) - 1), hook_y - length, rect1);
    object.push_back(obj1);
    obj1.set_object(0, 0, 0, 1, (hook_x + (3 * free_space) - 1), hook_y - length, rect2);
    object.push_back(obj1);
    obj1.set_object(0, 0, 0, 1, (hook_x + (4 * free_space) - 1), hook_y - length, rect3);
    object.push_back(obj1);
    obj1.set_object(0, 0, 0, 1, (hook_x - (2 * free_space) - 1), hook_y - length, rect4);
    object.push_back(obj1);
    obj1.set_object(0, 0, 0, 1, (hook_x - (3 * free_space) - 1), hook_y - length, rect5);
    object.push_back(obj1);
    obj1.set_object(0, 0, 0, 1, (hook_x - (4 * free_space) - 1), hook_y - length, rect6);
    object.push_back(obj1);
    //wypelnianie tablicy 
}

void which_is_hold(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea, float& arm_position, float& hand_position, int x, int y, int ddx, int ddy, int dx, int dy)
{
    int coordinate_x = x - ddx;
    int coordinate_y = y - ddy;
    list<Object>::iterator wsk_object;
    wsk_object = get_itterator_of_object_in_area(object, x, y);
    //wsk_object->change_possition(coordinate_x, coordinate_y, length, length);
    wsk_object->change_possition2(coordinate_x, coordinate_y, length, length);
    wsk_object->is_attached = false;
    wsk_object->is_falling = true;
    if (is_collision(object) == true)
    {
        wsk_object->change_possition(-2 * dx, -2 * dy, length, length);
        holding = 0;
        end_collision(object);
    }
}

void falling(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea, float& arm_position, float& hand_position, int x, int y, int dx, int dy)
{
    list<Object>::iterator wsk_object;
    wsk_object = get_itterator_of_object_in_area(object, x, y);
    wsk_object->change_possition(dx, dy, length, length);
    wsk_object->is_attached = false;
    wsk_object->is_falling = true;
    if (is_collision(object) == true)
    {
        wsk_object->change_possition(-2 * dx, -2 * dy, length, length);
        holding = 0;
        end_collision(object);
    }
}

list<Object>::iterator get_itterator_of_object(list<Object>& object, int x, int y)
{
    for (list<Object>::iterator i = object.begin(); i != object.end(); ++i)
    {
        if (i->x == x && i->y == y)
        {
            return i;
        }
    }
}

list<Object>::iterator get_itterator_of_object_in_area(list<Object>& object, int x, int y)
{
    for (list<Object>::iterator i = object.begin(); i != object.end(); ++i)
    {
        if (x >= (i->x) && x <= (i->x + length) && y >= (i->y) && y <= (i->y + length))
        {
            return i;
        }
    }
}

list<Object>::iterator get_itterator_of_falling_object(list<Object>& object)
{
    for (list<Object>::iterator i = object.begin(); i != object.end(); ++i)
    {
        if (i->is_falling == true)
        {
            return i;
        }
    }
}


bool is_in_area_of_object(list<Object>& object, int x, int y)
{
    for (list<Object>::iterator i = object.begin(); i != object.end(); ++i)
    {
        if (x >= (i->x) && x <= (i->x + length) && y >= (i->y) && y <= (i->y + length))
        {
            return true;
        }
    }
}

void make_collision(list<Object>& object)
{
    for (list<Object>::iterator j = object.begin(); j != object.end(); ++j)
    {
        for (list<Object>::iterator i = object.begin(); i != object.end(); ++i)
        {
            if (i == j) continue;
            else if (j->x + length < i->x || i->x + length < j->x || j->y + length < i->y || i->y + length < j->y)
            {

                j->is_collison = false;
                i->is_collison = false;
            }
            else
            {
                j->is_collison = true;
                i->is_collison = true;
                return;
            }
        }
    }
}

bool is_fall(list<Object>& object)
{
    for (list<Object>::iterator i = object.begin(); i != object.end(); ++i)
    {
        if (i->is_falling == true)
        {
            return true;
        }
    }
}

bool is_collision(list<Object>& object)
{
    for (list<Object>::iterator i = object.begin(); i != object.end(); ++i)
    {
        if (i->is_collison == true)
        {
            return true;
        }
    }
}

bool is_attached(list<Object>& object)
{
    for (list<Object>::iterator i = object.begin(); i != object.end(); ++i)
    {
        if (i->is_attached == false)
        {
            return false;
        }
    }
}

void make_hold_oobject(int hand_position_x, int hand_position_y)
{
    list<Object>::iterator wsk_object;
    if (is_in_area_of_object(object, hand_position_x, hand_position_y) == true)
    {
        if (holding == 1)
        {
            wsk_object = get_itterator_of_object_in_area(object, hand_position_x, hand_position_y);
            wsk_object->is_hold = true;
        }
        else
        {
            wsk_object = get_itterator_of_object_in_area(object, hand_position_x, hand_position_y);
            wsk_object->is_hold = true;
        }
    }
}

void end_collision(list<Object>& object)
{
    for (list<Object>::iterator i = object.begin(); i != object.end(); ++i)
    {
        if (i->is_collison == true)
        {
            i->is_collison = false;
        }
    }
}
 
