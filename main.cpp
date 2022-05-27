#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3

#define IDM_EDIT_CHOOSECOLOR 4

#define IDM_LINE_MIDPOINT 5
#define IDM_LINE_DDA 6
#define IDM_LINE_PARAMETRIC 7

#define IDM_Recursive_Fill 8
#define IDM_Non_Recursive_Fill 9

#include <tchar.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include <stack>
using namespace std;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Declare procedure to add menus  */
HMENU CreateMenus();

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

HCURSOR cNormal = LoadCursor(NULL, IDC_ARROW);
HCURSOR cPlus = LoadCursor(NULL, IDC_CROSS);

HCURSOR currentCursor = cNormal;

CHOOSECOLOR colorChosen;
COLORREF acrCustClr[16];
COLORREF rgbCurrent;

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)
{
    HWND hwnd;        /* This is the handle for our window */
    MSG messages;     /* Here messages to the application are saved */
    WNDCLASSEX wincl; /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure; /* This function is called by windows */
    wincl.style = CS_DBLCLKS;            /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL; /* No menu */
    wincl.cbClsExtra = 0;      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx(
               0,                               /* Extended possibilites for variation */
               szClassName,                     /* Classname */
               _T("Computer Graphics Project"), /* Title Text */
               WS_OVERLAPPEDWINDOW,             /* default window */
               CW_USEDEFAULT,                   /* Windows decides the position */
               CW_USEDEFAULT,                   /* where the window ends up on the screen */
               544,                             /* The programs width */
               375,                             /* and height in pixels */
               HWND_DESKTOP,                    /* The window is a child-window to desktop */
               CreateMenus(),                   /* Menu bar */
               hThisInstance,                   /* Program Instance handler */
               NULL                             /* No Window Creation data */
           );

    ZeroMemory(&colorChosen, sizeof(colorChosen));
    colorChosen.lStructSize = sizeof(colorChosen);
    colorChosen.hwndOwner = hwnd;
    colorChosen.lpCustColors = (LPDWORD)acrCustClr;
    colorChosen.rgbResult = rgbCurrent;
    colorChosen.Flags = CC_FULLOPEN | CC_RGBINIT;

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
int Round(double x)
{
    return (int)(x + 0.5);
}

struct point
{
    int x, y;
};
void swap(point p1,point p2)
{
    point temp=p1;
    p1=p2;
    p2=temp;
}
void lineDDA(HDC hdc, point p1, point p2, COLORREF c)
{
    cout << " LineDDA with X1 = " << p1.x << " Y1 = " << p1.y << " X2 = " << p2.x << " Y2 = " << p2.y << endl;
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    if (abs(dy) <= abs(dx)) /// slope < 1
    {
        if (p1.x > p2.x)
        {
            swap(p1,p2);
        }
        int x = p1.x;
        double y = p1.y;
        double m = (double)dy / dx;
        SetPixel(hdc, x, Round(y), c);
        while (x < p2.x)
        {
            x++;
            y += m;
            SetPixel(hdc, x, Round(y), c);
        }
    }
    else /// slope > 1
    {
        if (p1.y > p2.y)
        {
            swap(p1,p2);
        }
        double x = p1.x;
        int y = p1.y;
        double minV = dx / dy;
        SetPixel(hdc, Round(x), y, c);
        while (y < p2.y)
        {
            y++;
            x += minV;
            SetPixel(hdc, Round(x), y, c);
        }
    }
}

void MidPointLine(HDC hdc,point p1, point p2, COLORREF c)
{
    cout << " Midpoint Line with X1 = " << p1.x << " Y1 = " << p1.y << " X2 = " << p2.x << " Y2 = " << p2.y << endl;
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    double slope = dy/dx;
    if (abs(dy)<= abs(dx)) /// slope  <1
    {
        if (p1.x > p2.x)
        {
            swap(p1,p2);
        }
        int x = p1.x, y = p1.y;
        int d = dx - 2 * dy;
        int d1 = 2 * (dx - dy);
        int d2 = -2 * dy;
        SetPixel(hdc, x, y, c);
        while (x < p2.x)
        {
            if (d <= 0)
            {
                x++;
                y++;
                d += d1;

            }
            else
            {
                d += d2;
                x++;
            }
            SetPixel(hdc, x, y, c);
        }
    }
    else /// slope > 1
    {
        int d = 2 * dx - dy;
        int d1 = 2 * dx;
        int d2 = 2 * dx - 2 * dy;
        if (p1.y > p2.y)
        {
            swap(p1,p2);
        }
        int x = p1.x, y = p1.y;
        SetPixel(hdc, x, y, c);

        while (y < p2.y)
        {
            if (d <= 0)
            {
                y++;
                d += d1;
            }
            else
            {
                d += d2;
                x++;
                y++;
            }

            SetPixel(hdc, x, y, c);
        }

    }
}
void paremetricLine(HDC hdc, double x1, double y1, double x2, double y2, COLORREF c)
{
    cout << " ParametricLine with X1 = " << x1 << " Y1 = " << y1 << " X2 = " << x2 << " Y2 = " << y2 << endl;
    double x, y;
    for (double t = 0; t < 1; t += 0.0001)
    {
        x = x1 + t * (x2 - x1);
        y = y1 + t * (y2 - y1);
        SetPixel(hdc, Round(x), Round(y), c);
    }
}

void Recursive_FloodFill(HDC hdc,point p, COLORREF currentColor,  COLORREF filledColor)
{
    COLORREF c= GetPixel(hdc,p.x,p.y);
    if(c !=currentColor)
        return;
    SetPixel(hdc,p.x,p.y,filledColor);
    p.x=p.x+1;
    Recursive_FloodFill(hdc,p,currentColor,filledColor);
    p.x=p.x-2;
    Recursive_FloodFill(hdc,p,currentColor,filledColor);
    p.x=p.x+2; // brg3 el x lel value el 2aslya
    p.y=p.y+1;
    Recursive_FloodFill(hdc,p,currentColor,filledColor);
    p.y=p.y-2;
    Recursive_FloodFill(hdc,p,currentColor,filledColor);
    p.y=p.y+2; // brg3 el y lel value el 2aslya

}

void non_recursiveFloodFill(HDC hdc,point p,COLORREF filledColor)
{
    stack <point> s;
    s.push(p);
    COLORREF c;
    COLORREF currentColor=GetPixel(hdc,p.x,p.y);
    while(!s.empty())
    {
        p= s.top();
        s.pop();
        c=GetPixel(hdc,p.x,p.y);
        if(currentColor!=c)
            continue;
        SetPixel(hdc,p.x,p.y,filledColor);
        p.y=p.y-1;
        s.push(p);
        p.y=p.y+2;
        s.push(p);
        p.y=p.y-2; //brg3 el y lel value el adema
        p.x=p.x-1;
        s.push(p);
        p.x=p.x+2;
        s.push(p);
        p.x=p.x-2; //brg3 el x lel value el 2aslya

    }

}

int currentFunction = -1;
vector<point> points;

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);
    switch (message) /* handle the messages */
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_EDIT_CHOOSECOLOR:
            if (ChooseColor(&colorChosen) == TRUE)
            {
                rgbCurrent = colorChosen.rgbResult;
            }

            break;
        case IDM_LINE_DDA:
        case IDM_LINE_MIDPOINT:
        case IDM_LINE_PARAMETRIC:
        case IDM_Recursive_Fill:
        case IDM_Non_Recursive_Fill:

            currentFunction = LOWORD(wParam);
            points.clear();
            currentCursor = cPlus;
            break;

        }

        break;
    case WM_SETCURSOR:
        SetCursor(currentCursor);
        break;
    case WM_LBUTTONUP:
    {
        int x = (int)LOWORD(lParam);
        int y = (int)HIWORD(lParam);
        point p = {x, y};

        switch (currentFunction)
        {
        case IDM_LINE_DDA:
            points.push_back(p);

            if (points.size() == 2)
            {
                lineDDA(hdc, points[0], points[1], rgbCurrent);
                currentCursor = cNormal;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_LINE_MIDPOINT:
            points.push_back(p);

            if (points.size() == 2)
            {
                MidPointLine(hdc, points[0], points[1], rgbCurrent);
                currentCursor = cNormal;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_LINE_PARAMETRIC:
            points.push_back(p);

            if (points.size() == 2)
            {
                paremetricLine(hdc, (double)points[0].x, (double)points[0].y, (double)points[1].x, (double)points[1].y, rgbCurrent);
                currentCursor = cNormal;
                currentFunction = -1;
                points.clear();
            }

            break;
        case IDM_Recursive_Fill:
            Recursive_FloodFill(hdc,p,GetPixel(hdc,p.x,p.y),rgbCurrent);
            currentCursor = cNormal;
            currentFunction = -1;
            points.clear();

            break;
        case IDM_Non_Recursive_Fill:
            non_recursiveFloodFill(hdc,p,rgbCurrent);

            currentCursor = cNormal;
            currentFunction = -1;
            points.clear();

            break;

        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0); /* send a WM_QUIT to the message queue */
        break;
    default: /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

HMENU CreateMenus()
{
    HMENU hMenubar = CreateMenu();
    HMENU fileMenu = CreateMenu();
    HMENU editMenu = CreateMenu();
    HMENU lineMenu = CreateMenu();
    HMENU fillingMenu = CreateMenu();



    AppendMenuW(fileMenu, MF_STRING, IDM_FILE_NEW, L"New");
    AppendMenuW(fileMenu, MF_STRING, IDM_FILE_OPEN, L"Open");
    AppendMenuW(fileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(fileMenu, MF_STRING, IDM_FILE_QUIT, L"Quit");

    AppendMenuW(editMenu, MF_STRING, IDM_EDIT_CHOOSECOLOR, L"Choose color");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)fileMenu, L"&File");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)editMenu, L"&Edit");

    AppendMenuW(lineMenu, MF_STRING, IDM_LINE_MIDPOINT, L"MidPoint");
    AppendMenuW(lineMenu, MF_STRING, IDM_LINE_DDA, L"DDA");
    AppendMenuW(lineMenu, MF_STRING, IDM_LINE_PARAMETRIC, L"Parmetric");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)lineMenu, L"&Line");

    AppendMenuW(fillingMenu, MF_STRING, IDM_Non_Recursive_Fill, L"Non-Recursive_Flood_fill");
    AppendMenuW(fillingMenu, MF_STRING, IDM_Recursive_Fill, L"Recursive_Flood_Fill");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)fillingMenu, L"&Filling");


    return hMenubar;
}
