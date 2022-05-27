#include<cmath>
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3

#define IDM_EDIT_CHOOSECOLOR 4
#define IDM_EDIT_CLEAR 5

#define IDM_LINE_MIDPOINT 6
#define IDM_LINE_DDA 7
#define IDM_LINE_PARAMETRIC 8

#define IDM_Recursive_Fill 9
#define IDM_Non_Recursive_Fill 10

#define CircleWindow 11
#define IDM_rectangleClipping 12
#define IDM_squareClipping 13

#define IDM_CIRCLE_DIRECT 14
#define IDM_CIRCLE_POLAR 15
#define IDM_CIRCLE_ITERATIVEPOLAR 16
#define IDM_CIRCLE_MIDPOINT 17
#define IDM_CIRCLE_MODIFIEDMIDPOINT 18

#define IDM_GENERATE_POLYGON 19

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

HCURSOR* currentCursor = NULL;

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
///---------------------------------Flood-Fill-------------------------------------
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
///--------------------------------------------circle----------------------------------------
void Draw8points(HDC hdc,int xc,int yc, int a, int b,COLORREF color)
{

    SetPixel(hdc, xc+a, yc+b, color);
    SetPixel(hdc, xc-a, yc+b, color);
    SetPixel(hdc, xc-a, yc-b, color);
    SetPixel(hdc, xc+a, yc-b, color);
    SetPixel(hdc, xc+b, yc+a, color);
    SetPixel(hdc, xc-b, yc+a, color);
    SetPixel(hdc, xc-b, yc-a, color);
    SetPixel(hdc, xc+b, yc-a, color);
}

void circleDirect(HDC hdc,int xc,int yc,int R,COLORREF color) //Direct
{
    double dtheta=1.0/R;
    for(double theta =0; theta<=6.28319; theta+=dtheta)//360 degrees in redians
    {
        int x= Round ( xc + R * cos(theta) );
        int y = Round (yc + R * sin (theta));
        SetPixel( hdc, x,y,color );
    }

}

void circleMidPoint(HDC hdc, int xc, int yc, int R, COLORREF color )// midpoint
{
    int x = 0, y = R,d = 1 - R;
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
void circleMidPointModified(HDC hdc, int xc, int yc, int R, COLORREF color )// modified Midpoint
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
void circlePolar(HDC hdc,int xc,int yc,int R,COLORREF color) //Polar
{
    double theta = 0.0, x=R, y=0;
    Draw8points(hdc,xc,yc,x,y,color);
    while(x>y)
    {
        theta+=1.0/R;
        x=R*cos(theta);
        y=R*sin(theta);
        Draw8points(hdc,xc,yc,Round(x),Round(y),color);
    }
}

void circleIterativePolar(HDC hdc,int xc,int yc,int R,COLORREF color) // Iterative Polar
{
    double dtheta = 1.0 / R, x=R, y=0, c=cos(dtheta), s=sin(dtheta);
    Draw8points(hdc,xc,yc,x,y,color);
    while(x>y)
    {
        double x1=x*c-y*s;
        y=x*s+y*c;
        x=x1;
        Draw8points(hdc,xc,yc,Round(x),Round(y),color);
    }
}
double CalcRadius(int Xc, int Yc, int X,int Y  )
{
    return sqrt((X-Xc)*(X-Xc)+(Y-Yc)*(Y-Yc));
}

///----------------------------------------------clipping-------------------------------------
///point clipping with a rectangular window
void PointClipping(HDC hdc,point p,int xleft,int ytop,int xright,int ybottom,COLORREF color)
{
    if(p.x>=xleft && p.x<= xright && p.y>=ytop && p.y<=ybottom)
        SetPixel(hdc,p.x,p.y,color);
}

///line clipping with a rectangular window

union OutCode
{
    unsigned All : 4;
    struct
    {
        unsigned left : 1,top : 1,right : 1,bottom : 1;
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

void VIntersect(point p1,point p2, int x, double* xi, double* yi)
{
    *xi = x;
    *yi = p1.y + (x - p1.x) * (p2.y - p1.y) / (p2.x - p1.x);
}
void HIntersect(point p1, point p2, int y, double* xi, double* yi)
{
    *yi = y;
    *xi = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y -p1.y);
}
void CohenSuth(HDC hdc, point p1, point p2, int xleft, int ytop, int xright, int ybottom,int choice, COLORREF c)
{
    point pStart;
    pStart.x = p1.x;
    pStart.y = p1.y;
    point pEnd ;
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
        if(choice==1) ///DDA
            lineDDA( hdc, pStart, pEnd,  c);
        else if (choice==2)///parametric line
            paremetricLine(hdc, Round(pStart.x), Round(pStart.y),Round(pEnd.x), Round(pEnd.y), c);

        else if (choice==3)///midpoint Line
            MidPointLine(hdc,pStart,pEnd,c);
    }
}
///------generating polygon--------------
void generatePolygon(HDC hdc ,vector<point> p ,int n, COLORREF color){
for(int i=0 ; i< n-1 ; i++){
    paremetricLine(hdc,p[i].x,p[i].y,p[i+1].x,p[i+1].y,color);

}
    paremetricLine(hdc,p[0].x,p[0].y,p[n-1].x,p[n-1].y,color);

}

void generatePolygon(HDC hdc ,vector<point> p ,int n, vector<point> window, COLORREF color){
for(int i=0 ; i< n-1 ; i++){
    CohenSuth( hdc, p[i], p[i+1], window[0].x, window[0].y, window[1].x, window[1].y,2,rgbCurrent);

}
    CohenSuth( hdc,p[0], p[n-1], window[0].x, window[0].y, window[1].x, window[1].y,2,rgbCurrent);

}

int currentFunction = -1;
vector<point> points;
vector<point>window;
bool rectangleWindow=false;
bool circleWindow=false;
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
        case IDM_EDIT_CHOOSECOLOR:
            if (ChooseColor(&colorChosen) == TRUE)
            {
                rgbCurrent = colorChosen.rgbResult;
            }

            break;
        case IDM_EDIT_CLEAR:
            {
                int h = GetDeviceCaps(hdc, VERTRES);
                int w = GetDeviceCaps(hdc, HORZRES);

                for (int y = 0; y < h; y++)
                {
                    for (int x = 0; x < w; x++)
                    {
                        SetPixel(hdc, x, y, RGB(255, 255, 255));
                    }
                }
                
            }
            break;
        case IDM_LINE_DDA:
        case IDM_LINE_MIDPOINT:
        case IDM_LINE_PARAMETRIC:
        case IDM_Recursive_Fill:
        case IDM_Non_Recursive_Fill:
        case IDM_rectangleClipping:
        case IDM_squareClipping:
        case IDM_CIRCLE_DIRECT:
        case IDM_CIRCLE_POLAR :
        case IDM_CIRCLE_ITERATIVEPOLAR :
        case IDM_CIRCLE_MIDPOINT :
        case IDM_CIRCLE_MODIFIEDMIDPOINT:
        case IDM_GENERATE_POLYGON:
        case CircleWindow:
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
            return DefWindowProc(hwnd, message, wParam, lParam);
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
                if(rectangleWindow)
                {
                    CohenSuth( hdc, points[0], points[1], window[0].x, window[0].y, window[1].x, window[1].y,1,rgbCurrent);
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

                if(rectangleWindow)
                {
                    CohenSuth( hdc, points[0], points[1], window[0].x, window[0].y, window[1].x, window[1].y,3,rgbCurrent);
                }
                else
                {

                    MidPointLine(hdc, points[0], points[1], rgbCurrent);
                }
                currentCursor = NULL;
                currentFunction = -1;
            points.push_back(p);
            if (points.size() == 2)
            {
                if(rectangleWindow)
                {
                    CohenSuth( hdc, points[0], points[1], window[0].x, window[0].y, window[1].x, window[1].y,2,rgbCurrent);

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
            case IDM_GENERATE_POLYGON:
                points.push_back(p);
                if(points.size()==5){
                if(!rectangleWindow){
                generatePolygon(hdc,points,5,rgbCurrent);}
                else {
                     generatePolygon(hdc,points,5,window,rgbCurrent);

                }
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();

                }
            break;
            case IDM_CIRCLE_DIRECT:
            points.push_back(p);
            if(points.size() == 2)
            {
                R= CalcRadius(points[0].x,points[0].y,points[1].x,points[1].y);
                circleDirect(hdc,points[0].x,points[0].y,R,rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;

        case IDM_CIRCLE_POLAR :
            points.push_back(p);
            if(points.size() == 2)
            {
                R= CalcRadius(points[0].x,points[0].y,points[1].x,points[1].y);
                circlePolar(hdc,points[0].x,points[0].y,R,rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;

        case IDM_CIRCLE_ITERATIVEPOLAR :
            points.push_back(p);
            if(points.size() == 2)
            {
                R= CalcRadius(points[0].x,points[0].y,points[1].x,points[1].y);
                circleIterativePolar(hdc,points[0].x,points[0].y,R,rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;

        case IDM_CIRCLE_MIDPOINT :
            points.push_back(p);

            if(points.size() == 2)
            {
                R= CalcRadius(points[0].x,points[0].y,points[1].x,points[1].y);
                circleMidPoint(hdc,points[0].x,points[0].y,R,rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }

            break;

        case IDM_CIRCLE_MODIFIEDMIDPOINT:
            points.push_back(p);

            if(points.size() == 2)
            {
                R= CalcRadius(points[0].x,points[0].y,points[1].x,points[1].y);
                circleMidPointModified(hdc,points[0].x,points[0].y,R,rgbCurrent);
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;
        case IDM_Recursive_Fill:
            Recursive_FloodFill(hdc,p,GetPixel(hdc,p.x,p.y),rgbCurrent);
            currentCursor = NULL;
            currentFunction = -1;
            points.clear();

            break;
        case IDM_Non_Recursive_Fill:
            non_recursiveFloodFill(hdc,p,rgbCurrent);
            currentCursor = NULL;
            currentFunction = -1;
            points.clear();
            break;
        case IDM_rectangleClipping:
            points.push_back(p);
            window.push_back(p);
            if (points.size() == 2)
            {
                Rectangle(hdc,points[0].x,points[0].y,points[1].x,points[1].y);
                rectangleWindow=true;
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
            break;
        case CircleWindow:
        points.push_back(p);
               if (points.size() == 2)
            {
                R= CalcRadius(points[0].x,points[0].y,points[1].x,points[1].y);
                circleDirect(hdc,points[0].x,points[0].y,R,rgbCurrent);
                circleWindow=true;
                currentCursor = NULL;
                currentFunction = -1;
                points.clear();
            }
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
    HMENU clippingMenu = CreateMenu();
    HMENU circleMenu = CreateMenu();

    AppendMenuW(fileMenu, MF_STRING, IDM_FILE_NEW, L"New");
    AppendMenuW(fileMenu, MF_STRING, IDM_FILE_OPEN, L"Open");
    AppendMenuW(fileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(fileMenu, MF_STRING, IDM_FILE_QUIT, L"Quit");

    AppendMenuW(editMenu, MF_STRING, IDM_EDIT_CHOOSECOLOR, L"Choose color");
    AppendMenuW(editMenu, MF_STRING, IDM_EDIT_CLEAR, L"Clear");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)fileMenu, L"&File");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)editMenu, L"&Edit");

    AppendMenuW(lineMenu, MF_STRING, IDM_LINE_MIDPOINT, L"MidPoint");
    AppendMenuW(lineMenu, MF_STRING, IDM_LINE_DDA, L"DDA");
    AppendMenuW(lineMenu, MF_STRING, IDM_LINE_PARAMETRIC, L"Parmetric");
    AppendMenuW(lineMenu, MF_STRING, IDM_GENERATE_POLYGON, L"Polygon");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)lineMenu, L"&Line");

    AppendMenuW(fillingMenu, MF_STRING, IDM_Non_Recursive_Fill, L"Non-RecursiveFlood_fill");
    AppendMenuW(fillingMenu, MF_STRING, IDM_Recursive_Fill, L"Recursive_FloodFill");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)fillingMenu, L"&Filling");

    AppendMenuW(clippingMenu, MF_STRING, CircleWindow, L"CircleWindow");
    AppendMenuW(clippingMenu, MF_STRING, IDM_rectangleClipping, L"RectangleWindow");
    AppendMenuW(clippingMenu, MF_STRING, IDM_squareClipping, L"SquareWindow");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)clippingMenu, L"&Clipping");

    AppendMenuW(circleMenu, MF_STRING, IDM_CIRCLE_DIRECT, L"Direct");
    AppendMenuW(circleMenu, MF_STRING, IDM_CIRCLE_POLAR, L"Polar");
    AppendMenuW(circleMenu, MF_STRING, IDM_CIRCLE_ITERATIVEPOLAR, L"Iterative Polar");
    AppendMenuW(circleMenu, MF_STRING, IDM_CIRCLE_MIDPOINT, L"Midpoint");
    AppendMenuW(circleMenu, MF_STRING, IDM_CIRCLE_MODIFIEDMIDPOINT, L"Modified Midpoint");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)circleMenu, L"&Circle");



    return hMenubar;
}
