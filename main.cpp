#include<cmath>
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#define MAXENTRIES 2000;
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
#define IDM_Convex_Filling 22

#define CircleWindow 11
#define IDM_rectangleClipping 12
#define IDM_squareClipping 13

#define IDM_CIRCLE_DIRECT 14
#define IDM_CIRCLE_POLAR 15
#define IDM_CIRCLE_ITERATIVEPOLAR 16
#define IDM_CIRCLE_MIDPOINT 17
#define IDM_CIRCLE_MODIFIEDMIDPOINT 18

#define IDM_GENERATE_POLYGON 19
#define IDM_Cardinal_Spline   20
#define IDM_GENERATE_POINT 21

<<<<<<< Updated upstream
=======
#define IDM_GENERATE_POLYGON 20
#define IDM_CARDINAL_SPLINE 21
#define IDM_Bezier_Curve 23
#define IDM_GENERATE_POINT 22
>>>>>>> Stashed changes

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

HMENU hMenubar = CreateMenu();

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
void generatePolygon(HDC hdc,vector<point> p, COLORREF color)
{
    for(int i=0 ; i< p.size()-1 ; i++)
    {
        paremetricLine(hdc,p[i].x,p[i].y,p[i+1].x,p[i+1].y,color);

    }
    paremetricLine(hdc,p[0].x,p[0].y,p[p.size()-1].x,p[p.size()-1].y,color);

}

void generatePolygon(HDC hdc,vector<point> p, vector<point> window, COLORREF color)
{
    for(int i=0 ; i< p.size()-1 ; i++)
    {
        CohenSuth( hdc, p[i], p[i+1], window[0].x, window[0].y, window[1].x, window[1].y,2,rgbCurrent);

    }
    CohenSuth( hdc,p[0], p[p.size()-1], window[0].x, window[0].y, window[1].x, window[1].y,2,rgbCurrent);

}

void generateRectangle(HDC hdc, point p0, point p1, COLORREF color)
{
    vector<point> points;
    point px {p0.x, p1.y};
    point py {p1.x, p0.y};
    points.push_back(p0);
    points.push_back(px);
    points.push_back(p1);
    points.push_back(py);

    generatePolygon(hdc, points, color);
}
<<<<<<< Updated upstream


void DrawHermiteCurve(HDC hdc,point& p1, point& T1, point& p2, point& T2,COLORREF color)
=======
///-----------------curves
void DrawHermiteCurve(HDC hdc, point &p1, point &T1, point &p2, point &T2, COLORREF color)
>>>>>>> Stashed changes
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
        double t2 = t*t,
               t3 = t2*t;
        double x = alpha0 + alpha1*t + alpha2*t2 + alpha3*t3;
        double y = beta0 + beta1*t + beta2*t2 + beta3*t3;


        SetPixel(hdc, Round(x), Round(y), color);
    }
}

void DrawCardinalSpline(HDC hdc,vector<point> p,int n,double c,COLORREF color)
{
    vector<point>t(n);
    for(int i=1; i<n-1; i++)
    {
        t[i].x=(c/2)*(p[i+1].x-p[i-1].x);
        t[i].y=(c/2)*(p[i+1].y-p[i-1].y);

    }
    t[0].x=(c/2)*(p[1].x-p[0].x);
    t[0].y= (c/2)*(p[1].y-p[0].y);

    t[n-1].x=(c/2)*(p[n-1].x-p[n-2].x);
    t[n-1].y= (c/2)*(p[n-1].y-p[n-2].y);
    for(int i=0; i<n-1; i++)
    {
        DrawHermiteCurve(hdc,p[i],t[i],p[i+1],t[i+1],color);
    }
}
<<<<<<< Updated upstream
=======

void DrawBezierCurve(HDC hdc,point P0,point  P1,point  P2,point  P3,COLORREF color)
{
    point T0;
    T0.x=(3*(P1.x-P0.x));
    T0.y=(3*(P1.y-P0.y));
    point T1;
    T1.x=(3*(P3.x-P2.x));
    T1.y=(3*(P3.y-P2.y));
    DrawHermiteCurve(hdc,P0,T0,P3,T1,color);
}

///-----convex filing-----
>>>>>>> Stashed changes
struct Entry
{
    int xleft,xright;
};

void InitEntries(Entry table[])
{
    for(int i=0; i<800; i++)
    {
        table[i].xleft=99999999999;
        table[i].xright=-99999999999;
    }
}

void Edge2Table(point v1, point v2, Entry table[])
{
    if(v1.y==v2.y)
        return;
    if(v1.y>v2.y)
        swap(v1,v2);
    double minv=(double)(v2.x-v1.x)/(v2.y-v1.y);
    double x=v1.x;
    int y=v1.y;
    while(y<v2.y)
    {
        if(x<table[y].xleft)
            table[y].xleft=(int)ceil(x);
        if(x>table[y].xright)
            table[y].xright=(int)floor(x);
        y++;
        x+=minv;
    }

}
void polygon2Table(vector <point> p, int n ,Entry table[])
{
point v1;
v1.x=p[n-1].x;
v1.y=p[n-1].y;
point v2;
for(int i=0 ; i< n ; i++){
    v2.x=p[i].x;
    v2.y=p[i].y;

Edge2Table(v1,v2,table);
v1=v2;
}}

void DrawSanLines(HDC hdc,Entry table[],COLORREF color)
{
    for(int y=0; y<800; y++)
        if(table[y].xleft<table[y].xright)
           paremetricLine(hdc,table[y].xleft,y,table[y].xright,y,color);
           }
void ConvexFill(HDC hdc,vector<point> p,int n,COLORREF color)
{   cout<<" iam convex Fill"<<endl;
    Entry *table=new Entry[2000];
    InitEntries(table);
    polygon2Table(p,p.size(),table);
    DrawSanLines(hdc,table,color);
    delete table;
}

<<<<<<< Updated upstream
=======
///--general-Polygon-Filling
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
>>>>>>> Stashed changes


///------------polygon clipping
struct Vertex
{
    double x,y;
    Vertex(int x1=0,int y1=0)
    {
        x=x1;
        y=y1;
    }
};
typedef vector<Vertex> VertexList;
typedef bool (*IsInFunc)(Vertex& v,int edge);
typedef Vertex (*IntersectFunc)(Vertex& v1,Vertex& v2,int edge);

VertexList ClipWithEdge(VertexList p,int edge,IsInFunc In,IntersectFunc Intersect)
{
    VertexList OutList;
    Vertex v1=p[p.size()-1];
    bool v1_in=In(v1,edge);
    for(int i=0; i<(int)p.size(); i++)
    {
        Vertex v2=p[i];
        bool v2_in=In(v2,edge);
        if(!v1_in && v2_in)
        {
            OutList.push_back(Intersect(v1,v2,edge));
            OutList.push_back(v2);
        }
        else if(v1_in && v2_in)
            OutList.push_back(v2);
        else if(v1_in)
            OutList.push_back(Intersect(v1,v2,edge));
        v1=v2;
        v1_in=v2_in;
    }
    return OutList;
}

bool InLeft(Vertex& v,int edge)
{
    return v.x>=edge;
}
bool InRight(Vertex& v,int edge)
{
    return v.x<=edge;
}
bool InTop(Vertex& v,int edge)
{
    return v.y>=edge;
}
bool InBottom(Vertex& v,int edge)
{
    return v.y<=edge;
}
Vertex VIntersect(Vertex& v1,Vertex& v2,int xedge)
{
    Vertex res;
    res.x=xedge;
    res.y=v1.y+(xedge-v1.x)*(v2.y-v1.y)/(v2.x-v1.x);
    return res;
}

Vertex HIntersect(Vertex& v1,Vertex& v2,int yedge)
{
    Vertex res;
    res.y=yedge;
    res.x=v1.x+(yedge-v1.y)*(v2.x-v1.x)/(v2.y-v1.y);
    return res;
}
void PolygonClip(HDC hdc,POINT *p,int n,int xleft,int ytop,int xright,int ybottom)
{
    VertexList vlist;
    for(int i=0; i<n; i++)
        vlist.push_back(Vertex(p[i].x,p[i].y));
    vlist=ClipWithEdge(vlist,xleft,InLeft,VIntersect);
    vlist=ClipWithEdge(vlist,ytop,InTop,HIntersect);
    vlist=ClipWithEdge(vlist,xright,InRight,VIntersect);
    vlist=ClipWithEdge(vlist,ybottom,InBottom,HIntersect);
    Vertex v1=vlist[vlist.size()-1];
    for(int i=0; i<(int)vlist.size(); i++)
    {
        Vertex v2=vlist[i];
        MoveToEx(hdc,Round(v1.x),Round(v1.y),NULL);
        LineTo(hdc,Round(v2.x),Round(v2.y));
        v1=v2;
    }
}

int currentFunction = -1;
vector<point> points;
vector<point>window;
bool rectangleWindow=false;
bool circleWindow=false;
int R;
bool fillingSquare=false;
/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);
    switch (message) /* handle the messages */
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
<<<<<<< Updated upstream
=======
        /*      case IDM_FILE_SAVE:
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
                  break;*/
>>>>>>> Stashed changes
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
            Rectangle(hdc,-1,-1,w,h);

        }
        break;

        case IDM_rectangleClipping:
            if (rectangleWindow)
            {
                rectangleWindow=false;
                CheckMenuItem(hMenubar, IDM_rectangleClipping, MF_UNCHECKED);
            }
            else
            {
                currentFunction = LOWORD(wParam);
                points.clear();
                currentCursor = &cPlus;
                break;
            }
            break;
        case IDM_squareClipping:
            if (rectangleWindow)
            {
                rectangleWindow=false;
                CheckMenuItem(hMenubar, IDM_squareClipping, MF_UNCHECKED);
            }
            else
            {
                currentFunction = LOWORD(wParam);
                points.clear();
                currentCursor = &cPlus;
                break;
            }
            break;
        case CircleWindow:
            if (circleWindow)
            {
                circleWindow=false;
                CheckMenuItem(hMenubar, CircleWindow, MF_UNCHECKED);
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
        case IDM_Recursive_Fill:
        case IDM_Non_Recursive_Fill:
        case IDM_CIRCLE_DIRECT:
        case IDM_CIRCLE_POLAR :
        case IDM_CIRCLE_ITERATIVEPOLAR :
        case IDM_CIRCLE_MIDPOINT :
        case IDM_CIRCLE_MODIFIEDMIDPOINT:
        case IDM_GENERATE_POLYGON:
        case IDM_Cardinal_Spline:
        case IDM_GENERATE_POINT:
<<<<<<< Updated upstream
        case IDM_Convex_Filling:
=======
        case IDM_CONVEX_FILLING:
        case IDM_NON_CONVEX_FILLING:
        case IDM_Bezier_Curve:
>>>>>>> Stashed changes
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
    case WM_RBUTTONUP:
        switch (currentFunction)
        {
        case IDM_GENERATE_POLYGON:
            if(!rectangleWindow)
            {
                generatePolygon(hdc,points,rgbCurrent);
                ConvexFill(hdc,points,points.size(),RGB(0,0,1));
            }
            else
            {
                generatePolygon(hdc,points,window,rgbCurrent);
                ConvexFill(hdc,points,points.size(),RGB(0,0,1));

            }
            currentCursor = NULL;
            currentFunction = -1;
            points.clear();
            break;
        case IDM_Cardinal_Spline:
            for(int i=0; i<points.size(); i++)
            {
                cout<<points[i].x<<" "<<points[i].y<<endl;
            }
            DrawCardinalSpline(hdc,points,points.size(),1,rgbCurrent);
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
                points.clear();
            }
            break;
        case IDM_LINE_PARAMETRIC:
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
            break;
        case IDM_GENERATE_POINT:
            points.push_back(p);
            if(rectangleWindow)
            {
                PointClipping(hdc, points[0], window[0].x, window[0].y, window[1].x, window[1].y,rgbCurrent);
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
<<<<<<< Updated upstream
        case IDM_Convex_Filling:
=======
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

>>>>>>> Stashed changes
            break;
        case IDM_rectangleClipping:
            points.push_back(p);
            window.push_back(p);
            if (points.size() == 2)
            {
                generateRectangle(hdc,points[0],points[1], rgbCurrent);
                rectangleWindow=true;
                // CheckMenuItem(hMenubar, IDM_rectangleClipping, MF_CHECKED);
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
        case IDM_Cardinal_Spline:
            points.push_back(p);
            break;
        case IDM_Bezier_Curve:


            points.push_back(p);

            if(points.size()==2)
            {
                generateRectangle(hdc,points[0],points[1],rgbCurrent);
                point p1;
                if(points[1].x<points[0].x){
                    swap(points[0],points[1]);

                }
                p1.x=points[0].x;
                p1.y=points[0].y;
                point p2;
                p2.x=points[1].x;
                p2.y=points[0].y;
                point p3;
                p3.x=points[1].x;
                p3.y=points[1].y;
                point p4;
                p4.y=points[1].y;
                p4.x=points[0].x;
                point tmp1;
                point tmp2;
                tmp1.x=p1.x+10;
                tmp1.y=p1.y-10;
                tmp2.x= p2.x-10;
                tmp2.y=p2.y-10;

                point tmp3;
                point tmp4;
                tmp3.x=p4.x+10;
                tmp3.y=p4.y+10;
                tmp4.x=p3.x-10;
                tmp4.y=p3.y+10;

                cout<<p1.x<<"     "<<p1.y<<endl;
                cout<<p2.x<<"     "<<p2.y<<endl;
                cout<<p3.x<<"     "<<p3.y<<endl;
                cout<<p4.x<<"     "<<p4.y<<endl;
                cout<<tmp1.x<<"     "<<tmp1.y<<endl;
                cout<<tmp2.x<<"     "<<tmp2.y<<endl;

                for(;tmp2.y>tmp3.y; )
                {
                    cout<<"hi";
                    DrawBezierCurve(hdc,p1,tmp1,tmp2,p2,rgbCurrent);
                    DrawBezierCurve(hdc,p4,tmp3,tmp4,p3,rgbCurrent);
                    tmp1.y-=0.1;
                    tmp2.y-=0.1;
                    tmp3.y+=0.1;
                    tmp4.y+=0.1;
                    p1.y-=0.1;
                    p2.y-=0.1;
                    p3.y+=0.1;
                    p4.y+=0.1;
                }
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
    HMENU fileMenu = CreateMenu();
    HMENU editMenu = CreateMenu();
    HMENU lineMenu = CreateMenu();
    HMENU fillingMenu = CreateMenu();
    HMENU clippingMenu = CreateMenu();
    HMENU circleMenu = CreateMenu();
    HMENU curvesMenu = CreateMenu();
    HMENU shapesMenu = CreateMenu();


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
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)lineMenu, L"&Line");

    AppendMenuW(fillingMenu, MF_STRING, IDM_Non_Recursive_Fill, L"Non-RecursiveFlood_fill");
    AppendMenuW(fillingMenu, MF_STRING, IDM_Recursive_Fill, L"Recursive_FloodFill");
    AppendMenuW(fillingMenu, MF_STRING, IDM_Convex_Filling, L"Convex Filling");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)fillingMenu, L"&Filling");

<<<<<<< Updated upstream
    AppendMenuW(clippingMenu, MF_STRING, CircleWindow, L"CircleWindow");
    AppendMenuW(clippingMenu, MF_STRING, IDM_rectangleClipping, L"RectangleWindow");
    AppendMenuW(clippingMenu, MF_STRING, IDM_squareClipping, L"SquareWindow");
=======
    AppendMenuW(filledPolygonMenu, MF_STRING, IDM_NON_CONVEX_FILLING, L"Non-convex Filling");
    AppendMenuW(filledPolygonMenu, MF_STRING, IDM_CONVEX_FILLING, L"Convex Filling");
    AppendMenuW(filledPolygonMenu, MF_STRING, IDM_Bezier_Curve, L"Bezier Curve");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)filledPolygonMenu, L"Filling");

    AppendMenuW(clippingMenu, MF_STRING, IDM_CIRCLE_CLIPPING, L"Circle clipping");
    AppendMenuW(clippingMenu, MF_STRING, IDM_RECTANGLE_CLIPPING, L"Rectangle clipping");
    AppendMenuW(clippingMenu, MF_STRING, IDM_SQUARE_CLIPPING, L"Square clipping");
>>>>>>> Stashed changes
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)clippingMenu, L"&Clipping");

    AppendMenuW(circleMenu, MF_STRING, IDM_CIRCLE_DIRECT, L"Direct");
    AppendMenuW(circleMenu, MF_STRING, IDM_CIRCLE_POLAR, L"Polar");
    AppendMenuW(circleMenu, MF_STRING, IDM_CIRCLE_ITERATIVEPOLAR, L"Iterative Polar");
    AppendMenuW(circleMenu, MF_STRING, IDM_CIRCLE_MIDPOINT, L"Midpoint");
    AppendMenuW(circleMenu, MF_STRING, IDM_CIRCLE_MODIFIEDMIDPOINT, L"Modified Midpoint");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)circleMenu, L"&Circle");

    AppendMenuW(curvesMenu, MF_STRING, IDM_Cardinal_Spline, L"Cardinal Spline");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)curvesMenu, L"&Curve");

    AppendMenuW(shapesMenu, MF_STRING, IDM_GENERATE_POINT,L"point");
    AppendMenuW(shapesMenu, MF_STRING, IDM_GENERATE_POLYGON, L"Polygon");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)shapesMenu, L"&shapes");


<<<<<<< Updated upstream

    return hMenubar;
=======
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
>>>>>>> Stashed changes
}
