#include "Precompiled.h"
#include "Window.h"

// x86 architecture -> 32 bit -> 2^32 -> 00000000 00000000 00000000 00000000 to 11111111 11111111 11111111 11111111 -> 4GB
// x64 architecture -> 64 bit -> 2^64 -> 18EB
// 
// char -> 1 byte -> 8 bits -> 00000000 to 11111111 -> 0 to 255
// 
// 0000 - 0
// 0001 - 1
// 0010 - 2
// 0011 - 3
// 0100 - 4
// 0101 - 5
// ...
// 
// 27 -> 16 + 8 + 2 + 1 -> 0001 1011 -> 0x1b
// 85 -> 64 + 16 + 4 + 1 -> 0101 0101 -> 0x55
// 133 -> 128 + 4 + 1 -> 1000 0101 -> 0x85v
// 
// 11001101 -> 1 + 4 + 8 + 64 + 128 = 205 -> 0xcd

using namespace WallG;
using namespace WallG::Core;

LRESULT CALLBACK WinProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(handle, message, wParam, lParam);
}



void Window::Initialize(HINSTANCE instance, std::wstring appName, uint32_t width, uint32_t height, bool maximize)
{
    mInstance = instance;
    mAppName = std::move(appName);

    // Every Windows Window requires at least one window object. Three things are involved:
    // 1)    Register a window class (where we can specify the style of window we want).
    // 2)    Create a window object (spawning the actual window).
    // 3)    Retrieve and dispatch messages for this window.

    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WinProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = instance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = mAppName.c_str();
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    // Compute the correct window dimension
    RECT rc = { 0, 0, (LONG)width, (LONG)height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    const int winWidth = std::min(static_cast<int>(rc.right - rc.left), screenWidth);
    const int winHeight = std::min(static_cast<int>(rc.bottom - rc.top), screenHeight);
    const int left = (screenWidth - winWidth) / 2;
    const int top = (screenHeight - winHeight) / 2;

    // Create window
    mWindow = CreateWindow
    (
        mAppName.c_str(), mAppName.c_str(),
        WS_OVERLAPPEDWINDOW,
        left, top,
        winWidth, winHeight,
        nullptr, nullptr,
        instance, nullptr
    );

    ShowWindow(mWindow, maximize ? SW_MAXIMIZE : SW_SHOWNORMAL);
    SetCursorPos(screenWidth / 2, screenHeight / 2);

    mActive = (mWindow != nullptr);
}

// 128 + 4 +1 -> 1000 0101 
// 11001101 -> 1+4+8+64+128= 192+13=205 

//clearn up
void Window::Terminate()
{
    DestroyWindow(mWindow);

    UnregisterClass(mAppName.c_str(), mInstance);

    mWindow = nullptr;
    mInstance = nullptr;
}


void Window::ProcessMessage()
{
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (WM_QUIT == msg.message)
            mActive = false;
    }
}


