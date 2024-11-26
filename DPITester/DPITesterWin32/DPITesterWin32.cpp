// DPITesterWin32.cpp : Defines the entry point for the application.
//

#include "framework.h"


#include "../../../lsMisc/CommandLineParser.h"
#include "../../../lsMisc/DebugMacro.h"
#include "../../../lsMisc/stdosd/stdosd.h"
#include "../../../lsMisc/OpenCommon.h"

#include "DPITesterWin32.h"

#pragma comment(lib, "shlwapi.lib")

#define MAX_LOADSTRING 100
#define APPNAME L"DPITesterWin32"

using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
std::wstring gDpi;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void ErrorClose(std::wstring errorMessage)
{
    MessageBox(nullptr,
        errorMessage.c_str(),
        APPNAME,
        MB_ICONERROR);
    exit(1);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    CCommandLineParser parser;
    
    parser.AddOption({ L"-dpi" },
        ArgCount::ArgCount_One,
        &gDpi,
        ArgEncodingFlags_Default,
        L"DPI type: 'none', 'system', 'pmv2'");

    parser.Parse();

    if (!gDpi.empty())
    {
        if (gDpi == L"none")
        {

        }
        else if (gDpi == L"system")
        {
            DVERIFY(SetProcessDPIAware());
        }
        else if (gDpi == L"pmv2")
        {
            DVERIFY(SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2));
        }
        else
        {
            ErrorClose(L"Unknown dpi:" + gDpi);
        }
    }

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DPITESTERWIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DPITESTERWIN32));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DPITESTERWIN32));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DPITESTERWIN32);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      100, 100, 640, 480, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void OnRefresh(HWND hWnd)
{
    InvalidateRect(hWnd, nullptr, TRUE);
}
void OnReLaunch(HWND hWnd, int wmID)
{
    std::wstring commandLineOption = L"-dpi";
    if (wmID == ID_FILE_LAUNCHWITHNONE)
        commandLineOption += L"";
    else if (wmID == ID_FILE_LAUNCHWITHSYSTEM)
        commandLineOption += L" system";
    else if (wmID == ID_FILE_LAUNCHWITHPMV2)
        commandLineOption += L" pmv2";
    else
        DASSERT(false);

    OpenCommon(hWnd,
        stdGetModuleFileName().c_str(),
        commandLineOption.c_str());
}
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            std::wstring title;
            stdGetWindowText(hWnd, &title);
            title = stdFormat(L"%s (%s)",
                title.c_str(),
                gDpi.c_str());
            SetWindowText(hWnd, title.c_str());
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_FILE_REFRESH:
                OnRefresh(hWnd);
                break;
            case ID_FILE_LAUNCHWITHNONE:
            case ID_FILE_LAUNCHWITHSYSTEM:
            case ID_FILE_LAUNCHWITHPMV2:
                OnReLaunch(hWnd, wmId);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            RECT rect;
            GetClientRect(hWnd, &rect);
            std::wstringstream ss;
            ss << L"SystemDPI=" << GetDpiForSystem() << L", WindowDPI=" << GetDpiForWindow(hWnd);

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            DrawText(hdc, ss.str().c_str(), (int)ss.str().size(), &rect, DT_LEFT);
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

// Message handler for about box.
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
