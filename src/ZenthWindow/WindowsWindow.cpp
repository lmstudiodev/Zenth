#include "WindowsWindow.h"

ZenthEngine::WindowsWindow::WindowsWindow()
{
    m_hInstance = GetModuleHandle(nullptr);

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    
    WNDCLASSEXW wcex{};
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = &WindowsWindow::WindowProc_Setup;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"ZenthWndClass";
    wcex.hIconSm = LoadIconW(nullptr, IDI_APPLICATION);

    m_wndClass = RegisterClassExW(&wcex);

    if (m_wndClass == 0)
    {
        std::cout << "[ZENTH_ENGINE] Failed to register window class !!" << std::endl;
    }

    std::cout << "[ZENTH_ENGINE] Window class registered !!" << std::endl;

    POINT pos{ 0,0 };
    GetCursorPos(&pos);
    HMONITOR monitor = MonitorFromPoint(pos, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO monitorInfo{};
    monitorInfo.cbSize = sizeof(monitorInfo);
    GetMonitorInfoW(monitor, &monitorInfo);

    m_window = CreateWindowExW(
        WS_EX_OVERLAPPEDWINDOW,
        (LPCWSTR)m_wndClass,
        L"Zenth engine",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        monitorInfo.rcWork.left + 100,
        monitorInfo.rcWork.top + 100,
        m_width,
        m_height,
        nullptr,
        nullptr,
        m_hInstance,
        this);

    if (m_window == nullptr)
    {
        std::cout << "[ZENTH_ENGINE] Failed to create window !!" << std::endl;
    }

    std::cout << "[ZENTH_ENGINE] Window created !!" << std::endl;
}

ZenthEngine::WindowsWindow::~WindowsWindow()
{
    DestroyWindow(m_window);

    UnregisterClassW((LPCWSTR)m_wndClass, m_hInstance);
}

bool ZenthEngine::WindowsWindow::CloseRequested()
{
    bool closeRequested = m_shouldClose;
    m_shouldClose = false;

    return closeRequested;
}

void ZenthEngine::WindowsWindow::Update()
{
    MSG msg;

    while (PeekMessageW(&msg, m_window, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

size_t ZenthEngine::WindowsWindow::GetWidth()
{
	return m_width;
}

size_t ZenthEngine::WindowsWindow::GetHeight()
{
	return m_height;
}

LRESULT ZenthEngine::WindowsWindow::WindowProc_Setup(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_NCCREATE)
    {
        auto* createStruct = (CREATESTRUCTW*)lParam;
        SetWindowLongPtrW(wnd, GWLP_USERDATA, (LONG_PTR)createStruct->lpCreateParams);
        SetWindowLongPtrW(wnd, GWLP_WNDPROC, (LONG_PTR)&WindowsWindow::WindowProc_Redirect);

        return WindowProc_Redirect(wnd, msg, wParam, lParam);
    }

    return DefWindowProcW(wnd, msg, wParam, lParam);
}

LRESULT ZenthEngine::WindowsWindow::WindowProc_Redirect(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto* window = (WindowsWindow*)GetWindowLongPtrW(wnd, GWLP_USERDATA);

    return window->WindowProc(wnd, msg, wParam, lParam);
}

LRESULT ZenthEngine::WindowsWindow::WindowProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_SIZE:
        if (lParam && (HIWORD(lParam) != m_height || LOWORD(lParam) != m_width))
        {
            if (HIWORD(lParam) >= 64 || LOWORD(lParam) >= 64)
            {
                m_width = LOWORD(lParam);
                m_height = HIWORD(lParam);
            }
        }
        break;
    case WM_CLOSE:
        m_shouldClose = true;
        return 0;
    }
    
    return DefWindowProcW(wnd, msg, wParam, lParam);
}
