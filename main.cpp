#include <Windows.h>
#include <cstdint>
#include <format>
#include <string>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
    WPARAM wparam, LPARAM lparam)
{

    switch (msg) {

    case WM_DESTROY:

        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Log(const std::string& message)
{

    OutputDebugStringA(message.c_str());
}

std::wstring ConvertString(const std::string& str)
{
    if (str.empty()) {
        return std::wstring();
    }

    auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
    if (sizeNeeded == 0) {
        return std::wstring();
    }
    std::wstring result(sizeNeeded, 0);
    MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
    return result;
}

std::string ConvertString(const std::wstring& str)
{
    if (str.empty()) {
        return std::string();
    }

    auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
    if (sizeNeeded == 0) {
        return std::string();
    }
    std::string result(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
    return result;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

    Log(ConvertString(std::format(L"WSTRING{}\n", L"abc")));

    OutputDebugStringA("Hello,CG!!\n");

    WNDCLASS wc {};

    wc.lpfnWndProc = WindowProc;

    wc.lpszClassName = L"CG2WindowClass";

    wc.hInstance = GetModuleHandle(nullptr);

    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClass(&wc);

    const int32_t KClienwidth = 1280;
    const int32_t KClienHeigt = 720;

    RECT wrc = { 0, 0, KClienwidth, KClienHeigt };

    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

    HWND hwnd = CreateWindow(
        wc.lpszClassName,
        L"CG2",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wrc.right - wrc.left,
        wrc.bottom - wrc.top,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr);

    ShowWindow(hwnd, SW_SHOW);

    MSG msg {};

    while (msg.message != WM_QUIT) {

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

            TranslateMessage(&msg);

            DispatchMessage(&msg);

        } else {
        }
    };

    return 0;
}