#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3

#include <tchar.h>
#include <windows.h>
#include <iostream>
using namespace std;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Declare procedure to add menus  */
HMENU CreateMenus();

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

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
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

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
    int x,y;

};
void swap(double x1,double y1, double x2, double y2)
{
    double temp=0;
    temp=x1;
    x2=x1;
    x1=temp;
    temp=y1;
    y1=y2;
    y2=temp;
}
void lineDDA(HDC hdc,int x1, int y1, int x2, int y2, COLORREF c)
{
    int dx=x2-x1;
    int dy=y2-y1;
    if(abs(dy)<=abs(dx))///slope < 1
    {
    if(x1>x2)
        {
            swap(x1,y1,x2,y2);
        }
        int x=x1;
        double y=y1;
        double m=(double)dy/dx;
        SetPixel(hdc,x,y1,c);
        while(x<x2)
        {
            x++;
            y+=m;
            SetPixel(hdc,x,Round(y),c);

        }

    }
    else ///slope > 1
    {
        if(y1>y2)
        {
            swap(x1,y1,x2,y2);
        }
        double x=x1;
        int y= y2;
        double minV=dx/dy;
        SetPixel(hdc,x1,y1,c);
        while(y<y2)
        {
            y++;;
            x+=minV;
            SetPixel(hdc,Round(x),y,c);
        }
    }


}
void MidPointLine(HDC hdc,int x1, int y1, int x2, int y2,COLORREF c)
{   cout<<"X1 "<<x1<<" Y1 "<<y1<<"X2"<<x2<<"Y2"<<y2<<endl;
    int dx = x2 - x1;
    int dy = y2 - y1;

    if(dy<=dx) ///slope  <1
    {
        if(x1>x2)
        {
            swap(x1,y1,x2,y2);
        }

        int d = dx - 2*dy;
        int x = x1, y = y1;
        int d1=-2*(dx-dy);
        int d2=-2*dy;
        SetPixel(hdc,x,y,c);
        while (x < x2)
        {
            if (d <= 0)
            {
                x++;
                y++;
                d+=d1;
            }
            else
            {
                d += d2;
                x++;
            }

            SetPixel(hdc,x,y,c);
        }
    }
    else if(dx<dy)  /// slope > 1
    {
        int d = 2*dx - dy;
        if(y1>y2)
        {
            swap(x1,y1,x2,y2);
        }
        int x = x1, y = y1;
        while(y1<y2)
        {
            int d=2*dx-dy;
            int d1=2*dx;
            int d2=2*dx-2*dy;
            SetPixel(hdc,x,y,c);
            while(y<y2)
            {
                if (d <= 0)
                {
                    y++;
                    d+=d1;
                }
                else
                {
                    d += d2;
                    x++;
                    y++;
                }

                SetPixel(hdc,x,y,c);


            }

        }
    }
}
void paremetricLine(HDC hdc ,double x1, double y1, double x2, double y2, COLORREF c)
{   double x,y;
    for(double t=0; t<1 ; t+=0.0001)
    {
        x=x1+t*(x2-x1);
        y=y1+t*(y2-y1);
        SetPixel(hdc,Round(x),Round(y),c);
    }
}

/*  This function is called by the Windows function DispatchMessage()  */
int index=1;
HDC hdc;
int Xc, Yc, Y1, Y2, X1, X2,R1,R2;
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) /* handle the messages */
    {
    case WM_LBUTTONDBLCLK:
        hdc = GetDC(hwnd);
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
    HMENU hMenubar;
    HMENU hMenu;

    hMenubar = CreateMenu();
    hMenu = CreateMenu();

    AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&File");


    AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&midPoint");
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_OPEN, L"&DDA");
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Parmetric");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&Line");
    return hMenubar;
}
