#pragma once
#include <ZenthCore/IWindow.h>
#include <ZenthWindow/WinInclude.h>

namespace ZenthEngine
{
	class WindowsWindow : public IWindow
	{
	public:
			WindowsWindow();
			~WindowsWindow();

	public:
		bool CloseRequested() override;
		void Update() override;
		size_t GetWidth() override;
		size_t GetHeight() override;

	private:
		static LRESULT WindowProc_Setup(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WindowProc_Redirect(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT WindowProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		void Resize();

	private:
		HINSTANCE m_hInstance = nullptr;
		ATOM m_wndClass = 0;
		HWND m_window = nullptr;

		size_t m_width = 1920;
		size_t m_height = 1080;

		bool m_shouldClose = false;
		bool m_shouldResize;
	};
}