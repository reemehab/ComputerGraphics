#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3

#define IDM_EDIT_CHOOSECOLOR 4

#include <tchar.h>
#include <iostream>
#include <windows.h>

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
    colorChosen.lpCustColors = (LPDWORD) acrCustClr;
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

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);
    switch (message) /* handle the messages */
    {
    case WM_COMMAND:
          switch(LOWORD(wParam)) {
              case IDM_EDIT_CHOOSECOLOR:
                if (ChooseColor(&colorChosen) == TRUE) {
                    rgbCurrent = colorChosen.rgbResult;
                }
                
                break;
           }
           
           break;
    case WM_SETCURSOR:
        SetCursor(currentCursor);
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
    HMENU fileMenu;
    HMENU editMenu;

    hMenubar = CreateMenu();
    fileMenu = CreateMenu();
    editMenu = CreateMenu();

    AppendMenuW(fileMenu, MF_STRING, IDM_FILE_NEW, L"&New");
    AppendMenuW(fileMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
    AppendMenuW(fileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(fileMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

    AppendMenuW(editMenu, MF_STRING, IDM_EDIT_CHOOSECOLOR, L"Choose color");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)fileMenu, L"&File");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)editMenu, L"&Edit");
    return hMenubar;
}