#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#define IDM_FILE_SAVE 1
#define IDM_FILE_OPEN 2

#define IDM_EDIT_CHOOSECOLOR 3
#define IDM_EDIT_CLEAR 4

#define IDM_LINE_MIDPOINT 5
#define IDM_LINE_DDA 6
#define IDM_LINE_PARAMETRIC 7

#define IDM_RECURSIVE_FILL 8
#define IDM_NON_RECURSIVE_FILL 9

#define IDM_CONVEX_FILLING 10
#define IDM_NON_CONVEX_FILLING 11

#define IDM_CIRCLE_CLIPPING 12
#define IDM_RECTANGLE_CLIPPING 13
#define IDM_SQUARE_CLIPPING 14

#define IDM_CIRCLE_DIRECT 15
#define IDM_CIRCLE_POLAR 16
#define IDM_CIRCLE_ITERATIVEPOLAR 17
#define IDM_CIRCLE_MIDPOINT 18
#define IDM_CIRCLE_MODIFIEDMIDPOINT 19



#define IDM_GENERATE_POLYGON 20
#define IDM_CARDINAL_SPLINE 21
#define IDM_GENERATE_POINT 22

#define IDM_ELLIPSE_DIRECT 23
#define IDM_ELLIPSE_POLAR 24
#define IDM_ELLIPSE_MIDPOINT 25

#define IDM_FILLQUARTERLINE_FIRST 26
#define IDM_FILLQUARTERLINE_SECOND 27
#define IDM_FILLQUARTERLINE_THIRD 28
#define IDM_FILLQUARTERLINE_FOURTH 29

#define IDM_FILLQUARTERCIRCLE_FIRST 30
#define IDM_FILLQUARTERCIRCLE_SECOND 31
#define IDM_FILLQUARTERCIRCLE_THIRD 32
#define IDM_FILLQUARTERCIRCLE_FOURTH 33

#include <cmath>
#include <list>
#include <tchar.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <stack>
using namespace std;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Declare procedure to add menus  */

HMENU CreateMenus();

bool HDCToFile(const wchar_t *FilePath, HDC Context, RECT Area, uint16_t BitsPerPixel);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

HMENU hMenubar = CreateMenu();

HCURSOR cNormal = LoadCursor(NULL, IDC_ARROW);
HCURSOR cPlus = LoadCursor(NULL, IDC_CROSS);

HCURSOR *currentCursor = NULL;

CHOOSECOLOR colorChosen;
COLORREF acrCustClr[16];
COLORREF rgbCurrent;

OPENFILENAMEW ofn;
wchar_t szFileName[MAX_PATH] = L"";

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

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"BMP files (*.bmp)\0*.bmp\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"bmp";

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
void swap(point &p1, point &p2)
{
    point temp = p1;
    p1 = p2;
    p2 = temp;
}

///----------------LineAlgorithms--------------------------
void lineDDA(HDC hdc, point p1, point p2, COLORREF c)
{
    cout << " LineDDA with X1 = " << p1.x << " Y1 = " << p1.y << " X2 = " << p2.x << " Y2 = " << p2.y << endl;
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    if (abs(dy) <= abs(dx)) /// slope < 1
    {
        if (p1.x > p2.x)
        {
            swap(p1, p2);
        }
        int x = p1.x;
        double y = p1.y;
        double m = (double)dy / (double)dx;
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
            swap(p1, p2);
        }
        double x = p1.x;
        int y = p1.y;
        double minV = (double)dx / (double)dy;
        SetPixel(hdc, Round(x), y, c);
        while (y < p2.y)
        {
            y++;
            x += minV;
            SetPixel(hdc, Round(x), y, c);
        }
    }
}

void MidPointLine(HDC hdc, point p1, point p2, COLORREF c)
{
    cout << " Midpoint Line with X1 = " << p1.x << " Y1 = " << p1.y << " X2 = " << p2.x << " Y2 = " << p2.y << endl;
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    if ((dx >= 0 && 0 <= dy && dy <= dx) || (dx < 0 && 0 >= dy && dy >= dx)) /// 0 < slope < 1
    {
        if (p1.x > p2.x)
        {
            swap(p1, p2);
            dx = p2.x - p1.x;
            dy = p2.y - p1.y;
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
                d += d1;
                y++;
            }
            else
            {
                d += d2;
            }
            x++;
            SetPixel(hdc, x, y, c);
        }
    }
    else if ((dx >= 0 && dy > dx) || (dx < 0 && dy < dx)) /// slope > 1
    {
        if (p1.y > p2.y)
        {
            swap(p1, p2);
            dx = p2.x - p1.x;
            dy = p2.y - p1.y;
        }
        int x = p1.x, y = p1.y;
        int d = 2 * dx - dy;
        int d1 = 2 * dx;
        int d2 = 2 * dx - 2 * dy;
        SetPixel(hdc, x, y, c);

        while (y < p2.y)
        {
            if (d <= 0)
            {
                d += d1;
            }
            else
            {
                d += d2;
                x++;
            }
            y++;

            SetPixel(hdc, x, y, c);
        }
    }
    else if ((dx >= 0 && dy < -dx) || (dx < 0 && dy > -dx)) /// slope < -1
    {
        if (p1.y > p2.y)
        {
            swap(p1, p2);
            dx = p2.x - p1.x;
            dy = p2.y - p1.y;
        }
        int x = p1.x, y = p1.y;
        int d = 2 * dx + dy;
        int d1 = 2 * (dx + dy);
        int d2 = 2 * dx;
        SetPixel(hdc, x, y, c);

        while (y < p2.y)
        {
            if (d <= 0)
            {
                d += d1;
                x--;
            }
            else
            {
                d += d2;
            }
            y++;

            SetPixel(hdc, x, y, c);
        }
    }
    else
    {
        if (p1.x > p2.x)
        {
            swap(p1, p2);
            dx = p2.x - p1.x;
            dy = p2.y - p1.y;
        }
        int x = p1.x, y = p1.y;
        int d = -dx - 2 * dy;
        int d1 = -2 * dy;
        int d2 = 2 * (-dx - dy);
        SetPixel(hdc, x, y, c);
        while (x < p2.x)
        {
            if (d <= 0)
            {
                d += d1;
            }
            else
            {
                d += d2;
                y--;
            }
            x++;
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
///---------------------------------Flood-Fill-------------------------------------
void Recursive_FloodFill(HDC hdc, point p, COLORREF currentColor, COLORREF filledColor)
{
    COLORREF c = GetPixel(hdc, p.x, p.y);
    if (c != currentColor)
        return;
    SetPixel(hdc, p.x, p.y, filledColor);
    Recursive_FloodFill(hdc, {p.x + 1, p.y}, currentColor, filledColor);
    Recursive_FloodFill(hdc, {p.x, p.y + 1}, currentColor, filledColor);
    Recursive_FloodFill(hdc, {p.x - 1, p.y}, currentColor, filledColor);
    Recursive_FloodFill(hdc, {p.x, p.y - 1}, currentColor, filledColor);
}

void non_recursiveFloodFill(HDC hdc, point p, COLORREF filledColor)
{
    stack<point> s;
    s.push(p);
    COLORREF c;
    COLORREF currentColor = GetPixel(hdc, p.x, p.y);
    while (!s.empty())
    {
        p = s.top();
        s.pop();
        c = GetPixel(hdc, p.x, p.y);
        if (currentColor != c)
            continue;
        SetPixel(hdc, p.x, p.y, filledColor);
        s.push({p.x + 1, p.y});
        s.push({p.x, p.y + 1});
        s.push({p.x - 1, p.y});
        s.push({p.x, p.y - 1});
    }
}
///--------------------------------------------ellipse----------------------------------------
void Draw4points(HDC hdc,int xc,int yc,int x,int y,COLORREF color)
{
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
}

void ellipseDirect(HDC hdc, int xc, int yc, int a, int b, COLORREF c) {
	int x = 0;
	double y = b;
	Draw4points(hdc, xc, yc, x, Round(y), c);
	while (x * b * b < y * a * a) {
		x++;
		y = b * sqrt(1 - (x * x * 1.0) / (a * a));
		Draw4points(hdc,xc, yc, x, Round(y), c);
	}
	int y1 = 0;
	double x1 = a;
	Draw4points(hdc,xc, yc, Round(x1), y1, c);
	while (x1 * b * b > y1 * a * a) {
		y1++;
		x1 = a * sqrt(1 - (y1 * y1 * 1.0) / (b * b));
		Draw4points(hdc,xc, yc, Round(x1), y1, c);
	}
}
void ellipsePolar(HDC hdc, int xc, int yc, int a, int b, COLORREF c) {
	double x = a;
	double y = 0;
	double theta = 0;
	double dtheta = 1.0 / ((a+b));
	double cd = cos(dtheta);
	double sd = sin(dtheta);

	Draw4points(hdc,xc, yc, Round(x), Round(y),  c);
	while (x>0) {
		x = a * cos(theta);
		y = b * sin(theta);
		theta += dtheta;
		Draw4points(hdc,xc, yc, Round(x), Round(y),c);
	}
}

void ellipseMidpoint(HDC hdc, int xc, int yc, int a, int b, COLORREF c) {
	int x = 0, y = b;
	int b2 = b * b;
	int a2 = a * a;
	double d = b2 + a2 * pow((b - 0.5), 2) - a2 * b2;
	Draw4points(hdc,xc, yc, x, y, c);
	while (x * b * b < y * a * a) {
		if (d <= 0) {
			d += b2 * (2 * x + 3);
			x++;
		}
		else {
			d += b2 * (2 * x + 3) + a2 * (-2 * y + 2);
			x++;
			y--;
		}
		Draw4points(hdc,xc, yc, x, y, c);
	}
	x = a;
	y = 0;
	d = b2 * pow((a - 0.5), 2) + a2 - a2 * b2;
	Draw4points(hdc,xc, yc, x, y, c);
	while (x * b * b > y * a * a) {
		if (d <= 0) {
			d += a2 * (2 * y + 3);
			y++;
		}
		else {
			d += a2 * (2 * y + 3) + b2 * (-2 * x + 2);
			x--;
			y++;
		}
		Draw4points(hdc,xc, yc, x, y, c);
	}
}
///--------------------------------------------circle----------------------------------------
void Draw8points(HDC hdc, int xc, int yc, int a, int b, COLORREF color)
{

    SetPixel(hdc, xc + a, yc + b, color);
    SetPixel(hdc, xc - a, yc + b, color);
    SetPixel(hdc, xc - a, yc - b, color);
    SetPixel(hdc, xc + a, yc - b, color);
    SetPixel(hdc, xc + b, yc + a, color);
    SetPixel(hdc, xc - b, yc + a, color);
    SetPixel(hdc, xc - b, yc - a, color);
    SetPixel(hdc, xc + b, yc - a, color);
}

void circleDirect(HDC hdc, int xc, int yc, int R, COLORREF color) // Direct
{
    double dtheta = 1.0 / R;
    for (double theta = 0; theta <= 6.28319; theta += dtheta) // 360 degrees in redians
    {
        int x = Round(xc + R * cos(theta));
        int y = Round(yc + R * sin(theta));
        SetPixel(hdc, x, y, color);
    }
}

void circleMidPoint(HDC hdc, int xc, int yc, int R, COLORREF color) // midpoint
{
    int x = 0, y = R, d = 1 - R;
    Draw8points(hdc, xc, yc, x, y, color);
    while (x < y)
    {
        if (d <= 0)
        {
            d += 2 * x + 3;
        }
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
        Draw8points(hdc, xc, yc, x, y, color);
    }
}
void circleMidPointModified(HDC hdc, int xc, int yc, int R, COLORREF color) // modified Midpoint
{
    int x = 0, y = R;
    int d = 1 - R;
    int d1 = 3, d2 = 5 - 2 * R;
    Draw8points(hdc, xc, yc, x, y, color);
    while (x < y)
    {
        if (d <= 0)
        {
            d += d1;
            d2 += 2;
        }
        else
        {
            d += d2;
            d2 += 4;
            y--;
        }
        x++;
        d1 += 2;
        Draw8points(hdc, xc, yc, x, y, color);
    }
}
void circlePolar(HDC hdc, int xc, int yc, int R, COLORREF color) // Polar
{
    double theta = 0.0, x = R, y = 0;
    Draw8points(hdc, xc, yc, x, y, color);
    while (x > y)
    {
        theta += 1.0 / R;
        x = R * cos(theta);
        y = R * sin(theta);
        Draw8points(hdc, xc, yc, Round(x), Round(y), color);
    }
}

void circleIterativePolar(HDC hdc, int xc, int yc, int R, COLORREF color) // Iterative Polar
{
    double dtheta = 1.0 / R, x = R, y = 0, c = cos(dtheta), s = sin(dtheta);
    Draw8points(hdc, xc, yc, x, y, color);
    while (x > y)
    {
        double x1 = x * c - y * s;
        y = x * s + y * c;
        x = x1;
        Draw8points(hdc, xc, yc, Round(x), Round(y), color);
    }
}
double CalcRadius(int Xc, int Yc, int X, int Y)
{
    return sqrt((X - Xc) * (X - Xc) + (Y - Yc) * (Y - Yc));
}

///----------------------------------------------Fill Quarter Circle with Lines-------------------------------------
void Draw8PointsQuarter1Line(HDC hdc,int xc,int yc,int x,int y,COLORREF color)
{
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    paremetricLine(hdc,xc,yc,xc+x,yc-y,color);
    SetPixel(hdc,xc+y,yc+x,color);
    SetPixel(hdc,xc-y,yc+x,color);
    SetPixel(hdc,xc-y,yc-x,color);
    SetPixel(hdc,xc+y,yc-x,color);
    paremetricLine(hdc,xc,yc,xc+y,yc-x,color);


}
void Draw8PointsQuarter2Line(HDC hdc,int xc,int yc,int x,int y,COLORREF color)
{
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
    paremetricLine(hdc,xc,yc,xc-x,yc-y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc+y,yc+x,color);
    SetPixel(hdc,xc-y,yc+x,color);
    SetPixel(hdc,xc-y,yc-x,color);
    paremetricLine(hdc,xc,yc,xc-y,yc-x,color);
    SetPixel(hdc,xc+y,yc-x,color);


}
void Draw8PointsQuarter3Line(HDC hdc,int xc,int yc,int x,int y,COLORREF color)
{
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    paremetricLine(hdc,xc,yc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc+y,yc+x,color);
    SetPixel(hdc,xc-y,yc+x,color);
    paremetricLine(hdc,xc,yc,xc-y,yc+x,color);
    SetPixel(hdc,xc-y,yc-x,color);
    SetPixel(hdc,xc+y,yc-x,color);


}
void Draw8PointsQuarter4Line(HDC hdc,int xc,int yc,int x,int y,COLORREF color)
{
    SetPixel(hdc,xc+x,yc+y,color);
    paremetricLine(hdc,xc,yc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc+y,yc+x,color);
    paremetricLine(hdc,xc,yc,xc+y,yc+x,color);
    SetPixel(hdc,xc-y,yc+x,color);
    SetPixel(hdc,xc-y,yc-x,color);
    SetPixel(hdc,xc+y,yc-x,color);


}

void circleIterativePolarLines(HDC hdc, int xc, int yc, int R,int quarter ,COLORREF color) // Iterative Polar
{
    double dtheta = 1.0 / R, x = R, y = 0, c = cos(dtheta), s = sin(dtheta);
    while (x > y)
    {
        double x1 = x * c - y * s;
        y = x * s + y * c;
        x = x1;
        if(quarter==1){
            Draw8PointsQuarter1Line(hdc, xc, yc, Round(x), Round(y), color);
        }
        else if (quarter==2){
             Draw8PointsQuarter2Line(hdc, xc, yc, Round(x), Round(y), color);
        }
        else if (quarter==3){
            Draw8PointsQuarter3Line(hdc, xc, yc, Round(x), Round(y), color);
        }
        else{
            Draw8PointsQuarter4Line(hdc, xc, yc, Round(x), Round(y), color);
        }

    }
}
///----------------------------------------------Fill Quarter Circle with circles-------------------------------------
void Draw8pointsCircles(HDC hdc, int xc, int yc, int a, int b,int quad,COLORREF color)
{

    if(quad==1){
    SetPixel(hdc, xc + a, yc - b, color);
    SetPixel(hdc, xc + b, yc - a, color);
    }
    else if(quad==2){
    SetPixel(hdc, xc - b, yc - a, color);
    SetPixel(hdc, xc - a, yc - b, color);
    }
    else if(quad==3){
    SetPixel(hdc, xc - a, yc + b, color);
    SetPixel(hdc, xc - b, yc + a, color);
    }
    else{
    SetPixel(hdc, xc + a, yc + b, color);
    SetPixel(hdc, xc + b, yc + a, color);
    }
}
void circleIterativePolarCircles(HDC hdc, int xc, int yc, int R, int quad,COLORREF color) // Iterative Polar
{
    double dtheta = 1.0 / R, x = R, y = 0, c = cos(dtheta), s = sin(dtheta);
    Draw8points(hdc, xc, yc, x, y, color);
    while (x > y)
    {
        double x1 = x * c - y * s;
        y = x * s + y * c;
        x = x1;
        Draw8pointsCircles(hdc, xc, yc, Round(x), Round(y), quad,color);

    }
}




///----------------------------------------------clipping-------------------------------------
/// point clipping with a rectangular window
void PointClipping(HDC hdc, point p, int xleft, int ytop, int xright, int ybottom, COLORREF color)
{
    if (p.x >= xleft && p.x <= xright && p.y >= ytop && p.y <= ybottom)
        SetPixel(hdc, p.x, p.y, color);
}

/// line clipping with a rectangular window

union OutCode
{
    unsigned All : 4;
    struct
    {
        unsigned left : 1, top : 1, right : 1, bottom : 1;
    };
};
OutCode GetOutCode(point p1, int xleft, int ytop, int xright, int ybottom)
{
    OutCode out;
    out.All = 0;
    if (p1.x < xleft)
        out.left = 1;
    else if (p1.x > xright)
        out.right = 1;
    if (p1.y < ytop)
        out.top = 1;
    else if (p1.y > ybottom)
        out.bottom = 1;
    return out;
}

void VIntersect(point p1, point p2, int x, double *xi, double *yi)
{
    *xi = x;
    *yi = p1.y + (x - p1.x) * (p2.y - p1.y) / (p2.x - p1.x);
}
void HIntersect(point p1, point p2, int y, double *xi, double *yi)
{
    *yi = y;
    *xi = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
}
void CohenSuth(HDC hdc, point p1, point p2, int xleft, int ytop, int xright, int ybottom, int choice, COLORREF c)
{
    point pStart;
    pStart.x = p1.x;
    pStart.y = p1.y;
    point pEnd;
    pEnd.x = p2.x;
    pEnd.y = p2.y;

    OutCode out1 = GetOutCode(pStart, xleft, ytop, xright, ybottom);
    OutCode out2 = GetOutCode(pEnd, xleft, ytop, xright, ybottom);
    while ((out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi, yi;
        if (out1.All)
        {
            if (out1.left)
                VIntersect(pStart, pEnd, xleft, &xi, &yi);
            else if (out1.top)
                HIntersect(pStart, pEnd, ytop, &xi, &yi);
            else if (out1.right)
                VIntersect(pStart, pEnd, xright, &xi, &yi);
            else
                HIntersect(pStart, pEnd, ybottom, &xi, &yi);
            pStart.x = xi;
            pStart.y = yi;

            out1 = GetOutCode(pStart, xleft, ytop, xright, ybottom);
        }
        else
        {
            if (out2.left)
                VIntersect(pStart, pEnd, xleft, &xi, &yi);
            else if (out2.top)
                HIntersect(pStart, pEnd, ytop, &xi, &yi);
            else if (out2.right)
                VIntersect(pStart, pEnd, xright, &xi, &yi);
            else
                HIntersect(pStart, pEnd, ybottom, &xi, &yi);
            pEnd.x = xi;
            pEnd.y = yi;
            out2 = GetOutCode(pEnd, xleft, ytop, xright, ybottom);
        }
    }
    if (!out1.All && !out2.All)
    {
        if (choice == 1) /// DDA
            lineDDA(hdc, pStart, pEnd, c);
        else if (choice == 2) /// parametric line
            paremetricLine(hdc, Round(pStart.x), Round(pStart.y), Round(pEnd.x), Round(pEnd.y), c);

        else if (choice == 3) /// midpoint Line
            MidPointLine(hdc, pStart, pEnd, c);
    }
}
///------generating polygon--------------
void generatePolygon(HDC hdc, vector<point> p, COLORREF color)
{
    for (int i = 0; i < p.size() - 1; i++)
    {
        MidPointLine(hdc, p[i], p[i + 1], color);
    }
    MidPointLine(hdc, p[0], p[p.size() - 1], color);
}

void generatePolygon(HDC hdc, vector<point> p, vector<point> window, COLORREF color)
{
    for (int i = 0; i < p.size() - 1; i++)
    {
        CohenSuth(hdc, p[i], p[i + 1], window[0].x, window[0].y, window[1].x, window[1].y, 3, rgbCurrent);
    }
    CohenSuth(hdc, p[0], p[p.size() - 1], window[0].x, window[0].y, window[1].x, window[1].y, 3, rgbCurrent);
}

void generateRectangle(HDC hdc, point p0, point p1, COLORREF color)
{
    vector<point> points;
    point px{p0.x, p1.y};
    point py{p1.x, p0.y};
    points.push_back(p0);
    points.push_back(px);
    points.push_back(p1);
    points.push_back(py);

    generatePolygon(hdc, points, color);
}

void DrawHermiteCurve(HDC hdc, point &p1, point &T1, point &p2, point &T2, COLORREF color)
{

    double alpha0 = p1.x,
           alpha1 = T1.x,
           alpha2 = -3 * p1.x - 2 * T1.x + 3 * p2.x - T2.x,
           alpha3 = 2 * p1.x + T1.x - 2 * p2.x + T2.x;
    double beta0 = p1.y,
           beta1 = T1.y,
           beta2 = -3 * p1.y - 2 * T1.y + 3 * p2.y - T2.y,
           beta3 = 2 * p1.y + T1.y - 2 * p2.y + T2.y;
    for (double t = 0; t <= 1; t += 0.001)
    {
        double t2 = t * t,
               t3 = t2 * t;
        double x = alpha0 + alpha1 * t + alpha2 * t2 + alpha3 * t3;
        double y = beta0 + beta1 * t + beta2 * t2 + beta3 * t3;

        SetPixel(hdc, Round(x), Round(y), color);
    }
}

void DrawCardinalSpline(HDC hdc, vector<point> p, int n, double c, COLORREF color)
{
    vector<point> t(n);
    for (int i = 1; i < n - 1; i++)
    {
        t[i].x = (c / 2) * (p[i + 1].x - p[i - 1].x);
        t[i].y = (c / 2) * (p[i + 1].y - p[i - 1].y);
    }
    t[0].x = (c / 2) * (p[1].x - p[0].x);
    t[0].y = (c / 2) * (p[1].y - p[0].y);

    t[n - 1].x = (c / 2) * (p[n - 1].x - p[n - 2].x);
    t[n - 1].y = (c / 2) * (p[n - 1].y - p[n - 2].y);
    for (int i = 0; i < n - 1; i++)
    {
        DrawHermiteCurve(hdc, p[i], t[i], p[i + 1], t[i + 1], color);
    }
}
//-----convef fily-----
struct Entry
{
    int xmin, xmax;
};

void InitEntries(Entry table[])
{
    for (int i = 0; i < 800; i++)
    {
        table[i].xmin = 1e9;
        table[i].xmax = -1e9;
    }
}
void ScanEdge(point v1, point v2, Entry table[])
{
    if (v1.y == v2.y)
        return;
    if (v1.y > v2.y)
        swap(v1, v2);
    double minv = (double)(v2.x - v1.x) / (v2.y - v1.y);
    double x = v1.x;
    int y = v1.y;
    while (y < v2.y)
    {
        if (x < table[y].xmin)
            table[y].xmin = (int)ceil(x);
        if (x > table[y].xmax)
            table[y].xmax = (int)floor(x);
        y++;
        x += minv;
    }
}
void DrawSanLines(HDC hdc, Entry table[], COLORREF color)
{
    for (int y = 0; y < 800; y++)
        if (table[y].xmin < table[y].xmax)
            for (int x = table[y].xmin; x <= table[y].xmax; x++)
                SetPixel(hdc, x, y, color);
}

void ConvexFill(HDC hdc, vector<point> p, COLORREF color)
{
    Entry *table = new Entry[1920];
    InitEntries(table);
    point v1 = p[p.size() - 1];
    for (int i = 0; i < p.size(); i++)
    {
        point v2 = p[i];
        ScanEdge(v1, v2, table);
        v1 = p[i];
    }
    DrawSanLines(hdc, table, color);
    delete table;
}

// general
struct EdgeRec
{
    double x;
    double minv;
    int ymax;
    bool operator<(EdgeRec r)
    {
        return x < r.x;
    }
};
typedef list<EdgeRec> EdgeList;
EdgeRec InitEdgeRec(point &v1, point &v2)
{
    if (v1.y > v2.y)
        swap(v1, v2);
    EdgeRec rec;
    rec.x = v1.x;
    rec.ymax = v2.y;
    rec.minv = (double)(v2.x - v1.x) / (v2.y - v1.y);
    return rec;
}
void InitEdgeTable(vector<point> polygon, EdgeList table[])
{
    point v1 = polygon[polygon.size() - 1];
    for (int i = 0; i < polygon.size(); i++)
    {
        point v2 = polygon[i];
        if (v1.y == v2.y)
        {
            v1 = v2;
            continue;
        }
        EdgeRec rec = InitEdgeRec(v1, v2);
        table[v1.y].push_back(rec);
        v1 = polygon[i];
    }
}

void GeneralPolygonFill(HDC hdc, vector<point> polygon, COLORREF c)
{
    EdgeList *table = new EdgeList[1920];
    InitEdgeTable(polygon, table);
    int y = 0;
    while (y < 1920 && table[y].size() == 0)
        y++;
    if (y == 1920)
        return;
    EdgeList ActiveList = table[y];
    while (ActiveList.size() > 0)
    {
        ActiveList.sort();
        for (EdgeList::iterator it = ActiveList.begin(); it != ActiveList.end(); it++)
        {
            int x1 = (int)ceil(it->x);
            it++;
            int x2 = (int)floor(it->x);
            for (int x = x1; x <= x2; x++)
                SetPixel(hdc, x, y, c);
        }
        y++;
        EdgeList::iterator it = ActiveList.begin();
        while (it != ActiveList.end())
            if (y == it->ymax)
                it = ActiveList.erase(it);
            else
                it++;
        for (EdgeList::iterator it = ActiveList.begin(); it != ActiveList.end(); it++)
            it->x += it->minv;
        ActiveList.insert(ActiveList.end(), table[y].begin(), table[y].end());
    }
    delete[] table;
}

int currentFunction = -1;
vector<point> points;
vector<point> window;
bool rectangleWindow = false;
bool circleWindow = false;
int R;
/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);
    switch (message) /* handle the messages */
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_FILE_SAVE:
            if (GetSaveFileNameW(&ofn))
            {
                RECT rect;
                GetClientRect(hwnd, &rect);

                HDCToFile(szFileName, hdc, {0, 0, rect.right - rect.left, rect.bottom - rect.top}, 24);
            }
            break;
        case IDM_FILE_OPEN:
            if (GetOpenFileNameW(&ofn))
            {
                RECT rect;
                GetClientRect(hwnd, &rect);

                HBRUSH brush = CreatePatternBrush((HBITMAP)LoadImageW(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
                FillRect(hdc, &rect, brush);
                DeleteObject(brush);
            }
            break;
        case IDM_EDIT_CHOOSECOLOR:
            if (ChooseColor(&colorChosen) == TRUE)
            {
                rgbCurrent = colorChosen.rgbResult;
            }

            break;
        case IDM_EDIT_CLEAR:
        {
            RECT rect;
            GetClientRect(hwnd, &rect);
            rectangleWindow = false;
            circleWindow = false;
            currentFunction = -1;
            currentCursor = NULL;
            CheckMenuItem(hMenubar, IDM_RECTANGLE_CLIPPING, MF_UNCHECKED);
            CheckMenuItem(hMenubar, IDM_SQUARE_CLIPPING, MF_UNCHECKED);
            CheckMenuItem(hMenubar, IDM_CIRCLE_CLIPPING, MF_UNCHECKED);
            points.clear();
            window.clear();

            Rectangle(hdc, -1, -1, rect.right - rect.left + 1, rect.bottom - rect.top + 1);
        }
        break;

        case IDM_RECTANGLE_CLIPPING:
            if (rectangleWindow)
            {
                rectangleWindow = false;
                currentFunction = -1;
                currentCursor = NULL;
                CheckMenuItem(hMenubar, IDM_RECTANGLE_CLIPPING, MF_UNCHECKED);
                window.clear();
            }
            else
            {
                currentFunction = LOWORD(wParam);
                points.clear();
                window.clear();
                currentCursor = &cPlus;
                break;
            }
            break;
        case IDM_SQUARE_CLIPPING:
            if (rectangleWindow)
            {
                rectangleWindow = false;
                currentFunction = -1;
                currentCursor = NULL;
                CheckMenuItem(hMenubar, IDM_SQUARE_CLIPPING, MF_UNCHECKED);
                window.clear();
            }
            else
            {
                currentFunction = LOWORD(wParam);
                points.clear();
                window.clear();
                currentCursor = &cPlus;
                break;
            }
            break;
        case IDM_CIRCLE_CLIPPING:
            if (circleWindow)
            {
                circleWindow = false;
                currentFunction = -1;
                currentCursor = NULL;
                CheckMenuItem(hMenubar, IDM_CIRCLE_CLIPPING, MF_UNCHECKED);
            }
            else
            {
                currentFunction = LOWORD(wParam);
                points.clear();
                currentCursor = &cPlus;
                break;
            }
            break;
        case IDM_LINE_DDA:
        case IDM_LINE_MIDPOINT:
        case IDM_LINE_PARAMETRIC:
        case IDM_RECURSIVE_FILL:
        case IDM_NON_RECURSIVE_FILL:
        case IDM_CIRCLE_DIRECT:
        case IDM_CIRCLE_POLAR:
        case IDM_CIRCLE_ITERATIVEPOLAR:
        case IDM_CIRCLE_MIDPOINT:
        case IDM_CIRCLE_MODIFIEDMIDPOINT:
        case IDM_ELLIPSE_DIRECT:
        case IDM_ELLIPSE_POLAR:
        case IDM_ELLIPSE_MIDPOINT:
        case IDM_GENERATE_POLYGON:
        case IDM_CARDINAL_SPLINE:
        case IDM_GENERATE_POINT:
        case IDM_CONVEX_FILLING:
        case IDM_FILLQUARTERLINE_FIRST:
        case IDM_FILLQUARTERLINE_SECOND:
        case IDM_FILLQUARTERLINE_THIRD:
        case IDM_FILLQUARTERLINE_FOURTH:
        case IDM_FILLQUARTERCIRCLE_FIRST:
        case IDM_FILLQUARTERCIRCLE_SECOND:
        case IDM_FILLQUARTERCIRCLE_THIRD:
        case IDM_FILLQUARTERCIRCLE_FOURTH:
        case IDM_NON_CONVEX_FILLING:
            currentFunction = LOWORD(wParam);
            points.clear();
            currentCursor = &cPlus;
            break;
        }


        break;
    case WM_SETCURSOR:
        if (currentCursor != NULL)
            SetCursor(*currentCursor);
        else
        {
            ReleaseDC(hwnd, hdc);
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        break;
    case WM_RBUTTONUP:
        switch (currentFunction)
        {
        case IDM_GENERATE_POLYGON:
            if (!rectangleWindow)
            {
                generatePolygon(hdc, points, rgbCurrent);
            }
            else
            {
                generatePolygon(hdc, points, window, rgbCurrent);
            }
            currentCursor = NULL;
            currentFunction = -1;
            points.clear();
            break;
        case IDM_CONVEX_FILLING:
            ConvexFill(hdc, points, rgbCurrent);
            currentCursor = NULL;
            currentFunction = -1;
            points.clear();
            break;
        case IDM_NON_CONVEX_FILLING:
            GeneralPolygonFill(hdc, points, rgbCurrent);
            currentCursor = NULL;
            currentFunction = -1;
            points.clear();
            break;
        case IDM_CARDINAL_SPLINE:
            for (int i = 0; i < points.size(); i++)
            {
                cout << points[i].x << " " << points[i].y << endl;
            }
            DrawCardinalSpline(hdc, points, points.size(), 1, rgbCurrent);
            currentCursor = NULL;
            currentFunction = -1;
            points.clear();
            break;
        default:
            break;
        }
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
                if (rectangleWindow)
                {
                    CohenSuth(hdc, points[0], points[1], window[0].x, window[0].y, window[1].x, window[1].y, 1, rgbCurrent);
                }
                else
                {
                    lineDDA(hdc, points[0], points[1], rgbCurrent);
                }
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }

            break;
        case IDM_LINE_MIDPOINT:
            points.push_back(p);

            if (points.size() == 2)
            {

                if (rectangleWindow)
                {
                    CohenSuth(hdc, points[0], points[1], window[0].x, window[0].y, window[1].x, window[1].y, 3, rgbCurrent);
                }
                else
                {

                    MidPointLine(hdc, points[0], points[1], rgbCurrent);
                }
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_LINE_PARAMETRIC:
            points.push_back(p);
            if (points.size() == 2)
            {
                if (rectangleWindow)
                {
                    CohenSuth(hdc, points[0], points[1], window[0].x, window[0].y, window[1].x, window[1].y, 2, rgbCurrent);
                }
                else
                {
                    paremetricLine(hdc, (double)points[0].x, (double)points[0].y, (double)points[1].x, (double)points[1].y, rgbCurrent);
                }
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }

            break;
        case IDM_CONVEX_FILLING:
        case IDM_NON_CONVEX_FILLING:
        case IDM_GENERATE_POLYGON:
            points.push_back(p);
            break;
        case IDM_GENERATE_POINT:
            points.push_back(p);
            if (rectangleWindow)
            {
                PointClipping(hdc, points[0], window[0].x, window[0].y, window[1].x, window[1].y, rgbCurrent);
            }
            else
            {
                SetPixel(hdc, points[0].x, points[0].y, rgbCurrent);
            }
            currentCursor = NULL;
            currentFunction = -1;
            points.clear();
            break;

        case IDM_CIRCLE_DIRECT:
            points.push_back(p);
            if (points.size() == 2)
            {
                R = CalcRadius(points[0].x, points[0].y, points[1].x, points[1].y);
                circleDirect(hdc, points[0].x, points[0].y, R, rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;

        case IDM_CIRCLE_POLAR:
            points.push_back(p);
            if (points.size() == 2)
            {
                R = CalcRadius(points[0].x, points[0].y, points[1].x, points[1].y);
                circlePolar(hdc, points[0].x, points[0].y, R, rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;

        case IDM_CIRCLE_ITERATIVEPOLAR:
            points.push_back(p);
            if (points.size() == 2)
            {
                R = CalcRadius(points[0].x, points[0].y, points[1].x, points[1].y);
                circleIterativePolar(hdc, points[0].x, points[0].y, R, rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;

        case IDM_CIRCLE_MIDPOINT:
            points.push_back(p);

            if (points.size() == 2)
            {
                R = CalcRadius(points[0].x, points[0].y, points[1].x, points[1].y);
                circleMidPoint(hdc, points[0].x, points[0].y, R, rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }

            break;

        case IDM_CIRCLE_MODIFIEDMIDPOINT:
            points.push_back(p);

            if (points.size() == 2)
            {
                R = CalcRadius(points[0].x, points[0].y, points[1].x, points[1].y);
                circleMidPointModified(hdc, points[0].x, points[0].y, R, rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_ELLIPSE_DIRECT:
            points.push_back(p);
            if (points.size() == 3)
            {
                int a=CalcRadius(points[0].x,points[0].y,points[1].x, points[1].y);
                int b=CalcRadius(points[0].x,points[0].y,points[2].x, points[2].y);
                ellipseDirect(hdc, points[0].x, points[0].y, a,b, rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                cout << "Drawing direct ellipse" << endl;
                points.clear();
            }
            break;
        case IDM_ELLIPSE_MIDPOINT:
            points.push_back(p);
            if (points.size() == 3)
            {
                int a=CalcRadius(points[0].x,points[0].y,points[1].x, points[1].y);
                int b=CalcRadius(points[0].x,points[0].y,points[2].x, points[2].y);
                ellipseMidpoint(hdc, points[0].x, points[0].y, a,b, rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                cout << "Drawing midpoint ellipse" << endl;
                points.clear();
            }
            break;
        case IDM_ELLIPSE_POLAR:
            points.push_back(p);
            if (points.size() == 3)
            {
                int a=CalcRadius(points[0].x,points[0].y,points[1].x, points[1].y);
                int b=CalcRadius(points[0].x,points[0].y,points[2].x, points[2].y);
                ellipsePolar(hdc, points[0].x, points[0].y, a,b, rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                cout << "Drawing polar ellipse" << endl;
                points.clear();
            }
            break;
        case IDM_FILLQUARTERLINE_FIRST:
            points.push_back(p);
            if (points.size() == 2)
            {
                R = CalcRadius(points[0].x, points[0].y, points[1].x, points[1].y);
                circleIterativePolarLines(hdc, points[0].x, points[0].y, R, 1,rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_FILLQUARTERLINE_SECOND:
            points.push_back(p);
            if (points.size() == 2)
            {
                R = CalcRadius(points[0].x, points[0].y, points[1].x, points[1].y);
                circleIterativePolarLines(hdc, points[0].x, points[0].y, R, 2,rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_FILLQUARTERLINE_THIRD:
            points.push_back(p);
            if (points.size() == 2)
            {
                R = CalcRadius(points[0].x, points[0].y, points[1].x, points[1].y);
                circleIterativePolarLines(hdc, points[0].x, points[0].y, R, 3,rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_FILLQUARTERLINE_FOURTH:
            points.push_back(p);
            if (points.size() == 2)
            {
                R = CalcRadius(points[0].x, points[0].y, points[1].x, points[1].y);
                circleIterativePolarLines(hdc, points[0].x, points[0].y, R, 4,rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_FILLQUARTERCIRCLE_FIRST:
            points.push_back(p);
            if (points.size() == 2)
            {
                R = CalcRadius(points[0].x, points[0].y, points[1].x, points[1].y);
                circleIterativePolar(hdc, points[0].x, points[0].y, R,rgbCurrent);
                for(int i=0;i<=R;i++){
                circleIterativePolarCircles(hdc,points[0].x,points[0].y,i,1,rgbCurrent);
                }
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_FILLQUARTERCIRCLE_SECOND:
            points.push_back(p);
            if (points.size() == 2)
            {
                R = CalcRadius(points[0].x, points[0].y, points[1].x, points[1].y);
                circleIterativePolar(hdc, points[0].x, points[0].y, R,rgbCurrent);
                for(int i=0;i<=R;i++){
                circleIterativePolarCircles(hdc,points[0].x,points[0].y,i,2,rgbCurrent);
                }
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_FILLQUARTERCIRCLE_THIRD:
            points.push_back(p);
            if (points.size() == 2)
            {
                R = CalcRadius(points[0].x, points[0].y, points[1].x, points[1].y);
                circleIterativePolar(hdc, points[0].x, points[0].y, R,rgbCurrent);
                for(int i=0;i<=R;i++){
                circleIterativePolarCircles(hdc,points[0].x,points[0].y,i,3,rgbCurrent);
                }
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_FILLQUARTERCIRCLE_FOURTH:
            points.push_back(p);
            if (points.size() == 2)
            {
                R = CalcRadius(points[0].x, points[0].y, points[1].x, points[1].y);
                circleIterativePolar(hdc, points[0].x, points[0].y, R,rgbCurrent);
                for(int i=0;i<=R;i++){
                circleIterativePolarCircles(hdc,points[0].x,points[0].y,i,4,rgbCurrent);
                }
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_RECURSIVE_FILL:
            Recursive_FloodFill(hdc, p, GetPixel(hdc, p.x, p.y), rgbCurrent);
            currentCursor = NULL;
            currentFunction = -1;
            points.clear();

            break;
        case IDM_NON_RECURSIVE_FILL:
            non_recursiveFloodFill(hdc, p, rgbCurrent);
            currentCursor = NULL;
            currentFunction = -1;
            points.clear();
            break;
        case IDM_SQUARE_CLIPPING:
            window.push_back(p);
            if (window.size() == 2)
            {
                int edge;
                if (abs(window[0].x - window[1].x) > abs(window[0].y - window[1].y))
                {
                    edge = window[1].y - window[0].y;
                }
                else
                {
                    edge = window[1].x - window[0].x;
                }
                window[1] = {window[0].x + edge, window[0].y + edge};
                generateRectangle(hdc, window[0], window[1], rgbCurrent);
                rectangleWindow = true;
                CheckMenuItem(hMenubar, IDM_SQUARE_CLIPPING, MF_CHECKED);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_RECTANGLE_CLIPPING:
            window.push_back(p);
            if (window.size() == 2)
            {
                generateRectangle(hdc, window[0], window[1], rgbCurrent);
                rectangleWindow = true;
                CheckMenuItem(hMenubar, IDM_RECTANGLE_CLIPPING, MF_CHECKED);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_CIRCLE_CLIPPING:
            points.push_back(p);
            if (points.size() == 2)
            {
                R = CalcRadius(points[0].x, points[0].y, points[1].x, points[1].y);
                circleDirect(hdc, points[0].x, points[0].y, R, rgbCurrent);
                circleWindow = true;
                CheckMenuItem(hMenubar, IDM_CIRCLE_CLIPPING, MF_CHECKED);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_CARDINAL_SPLINE:
            points.push_back(p);
            break;
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0); /* send a WM_QUIT to the message queue */
        break;
    default: /* for messages that we don't deal with */
        ReleaseDC(hwnd, hdc);
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    ReleaseDC(hwnd, hdc);
    return 0;
}

HMENU CreateMenus()
{
    HMENU fileMenu = CreateMenu();
    HMENU editMenu = CreateMenu();
    HMENU lineMenu = CreateMenu();
    HMENU fillingMenu = CreateMenu();
    HMENU clippingMenu = CreateMenu();
    HMENU circleMenu = CreateMenu();
    HMENU fillQuarterLine = CreateMenu();
    HMENU fillQuarterCircle = CreateMenu();
    HMENU ellipseMenu = CreateMenu();
    HMENU curvesMenu = CreateMenu();
    HMENU shapesMenu = CreateMenu();
    HMENU filledPolygonMenu = CreateMenu();

    AppendMenuW(fileMenu, MF_STRING, IDM_FILE_SAVE, L"Save");
    AppendMenuW(fileMenu, MF_STRING, IDM_FILE_OPEN, L"Open");

    AppendMenuW(editMenu, MF_STRING, IDM_EDIT_CHOOSECOLOR, L"Choose color");
    AppendMenuW(editMenu, MF_STRING, IDM_EDIT_CLEAR, L"Clear");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)fileMenu, L"File");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)editMenu, L"Edit");

    AppendMenuW(lineMenu, MF_STRING, IDM_LINE_MIDPOINT, L"MidPoint");
    AppendMenuW(lineMenu, MF_STRING, IDM_LINE_DDA, L"DDA");
    AppendMenuW(lineMenu, MF_STRING, IDM_LINE_PARAMETRIC, L"Parmetric");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)lineMenu, L"Line");

    AppendMenuW(fillingMenu, MF_STRING, IDM_NON_RECURSIVE_FILL, L"Non-recursive flood fill");
    AppendMenuW(fillingMenu, MF_STRING, IDM_RECURSIVE_FILL, L"Recursive flood fill");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)fillingMenu, L"Filling");

    AppendMenuW(filledPolygonMenu, MF_STRING, IDM_NON_CONVEX_FILLING, L"Non-convex Filling");
    AppendMenuW(filledPolygonMenu, MF_STRING, IDM_CONVEX_FILLING, L"Convex Filling");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)filledPolygonMenu, L"Filling");

    AppendMenuW(clippingMenu, MF_STRING, IDM_CIRCLE_CLIPPING, L"Circle clipping");
    AppendMenuW(clippingMenu, MF_STRING, IDM_RECTANGLE_CLIPPING, L"Rectangle clipping");
    AppendMenuW(clippingMenu, MF_STRING, IDM_SQUARE_CLIPPING, L"Square clipping");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)clippingMenu, L"&Clipping");

    AppendMenuW(circleMenu, MF_STRING, IDM_CIRCLE_DIRECT, L"Direct");
    AppendMenuW(circleMenu, MF_STRING, IDM_CIRCLE_POLAR, L"Polar");
    AppendMenuW(circleMenu, MF_STRING, IDM_CIRCLE_ITERATIVEPOLAR, L"Iterative Polar");
    AppendMenuW(circleMenu, MF_STRING, IDM_CIRCLE_MIDPOINT, L"Midpoint");
    AppendMenuW(circleMenu, MF_STRING, IDM_CIRCLE_MODIFIEDMIDPOINT, L"Modified Midpoint");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)circleMenu, L"Circle");


    AppendMenuW(ellipseMenu, MF_STRING, IDM_ELLIPSE_DIRECT, L"Direct");
    AppendMenuW(ellipseMenu, MF_STRING, IDM_ELLIPSE_MIDPOINT, L"Midpoint");
    AppendMenuW(ellipseMenu, MF_STRING, IDM_ELLIPSE_POLAR, L"Polar");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)ellipseMenu, L"Ellipse");

    AppendMenuW(curvesMenu, MF_STRING, IDM_CARDINAL_SPLINE, L"Cardinal Spline");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)curvesMenu, L"Curve");

    AppendMenuW(shapesMenu, MF_STRING, IDM_GENERATE_POINT, L"Point");
    AppendMenuW(shapesMenu, MF_STRING, IDM_GENERATE_POLYGON, L"Polygon");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)shapesMenu, L"Shapes");

    AppendMenuW(fillQuarterLine, MF_STRING, IDM_FILLQUARTERLINE_FIRST, L"First");
    AppendMenuW(fillQuarterLine, MF_STRING, IDM_FILLQUARTERLINE_SECOND, L"Second");
    AppendMenuW(fillQuarterLine, MF_STRING, IDM_FILLQUARTERLINE_THIRD, L"Third");
    AppendMenuW(fillQuarterLine, MF_STRING, IDM_FILLQUARTERLINE_FOURTH, L"Fourth");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)fillQuarterLine, L"FillQuarterLines");

    AppendMenuW(fillQuarterCircle, MF_STRING, IDM_FILLQUARTERCIRCLE_FIRST, L"First");
    AppendMenuW(fillQuarterCircle, MF_STRING, IDM_FILLQUARTERCIRCLE_SECOND, L"Second");
    AppendMenuW(fillQuarterCircle, MF_STRING, IDM_FILLQUARTERCIRCLE_THIRD, L"Third");
    AppendMenuW(fillQuarterCircle, MF_STRING, IDM_FILLQUARTERCIRCLE_FOURTH, L"Fourth");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)fillQuarterCircle, L"FillQuarterCircles");
    return hMenubar;
}

bool HDCToFile(const wchar_t *FilePath, HDC Context, RECT Area, uint16_t BitsPerPixel = 24)
{
    uint32_t Width = Area.right - Area.left;
    uint32_t Height = Area.bottom - Area.top;

    BITMAPINFO Info;
    BITMAPFILEHEADER Header;
    memset(&Info, 0, sizeof(Info));
    memset(&Header, 0, sizeof(Header));
    Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    Info.bmiHeader.biWidth = Width;
    Info.bmiHeader.biHeight = Height;
    Info.bmiHeader.biPlanes = 1;
    Info.bmiHeader.biBitCount = BitsPerPixel;
    Info.bmiHeader.biCompression = BI_RGB;
    Info.bmiHeader.biSizeImage = Width * Height * (BitsPerPixel > 24 ? 4 : 3);
    Header.bfType = 0x4D42;
    Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    char *Pixels = NULL;
    HDC MemDC = CreateCompatibleDC(Context);
    HBITMAP Section = CreateDIBSection(Context, &Info, DIB_RGB_COLORS, (void **)&Pixels, 0, 0);
    DeleteObject(SelectObject(MemDC, Section));
    BitBlt(MemDC, 0, 0, Width, Height, Context, Area.left, Area.top, SRCCOPY);
    DeleteDC(MemDC);

/*    std::fstream hFile(FilePath, std::ios::out | std::ios::binary);
    if (hFile.is_open())
    {
        hFile.write((char *)&Header, sizeof(Header));
        hFile.write((char *)&Info.bmiHeader, sizeof(Info.bmiHeader));
        hFile.write(Pixels, (((BitsPerPixel * Width + 31) & ~31) / 8) * Height);
        hFile.close();
        DeleteObject(Section);
        return true;
    }
*/
    DeleteObject(Section);
    return false;
}
