#include "WindowsEngine.h"
#include <ZenthWindow/WindowsWindow.h>
#include <ZenthWindow/D3D12/D3D12Graphics.h>
#include <ZenthWindow/D3D12/D3D12ImGui.h>

void ZenthEngine::OSInit()
{
	Get().Window = std::make_unique<WindowsWindow>();
	Get().Graphics = std::make_unique<D3D12Graphics>(*dynamic_cast<WindowsWindow*>(Get().Window.get()));
	Get().Imgui = std::make_unique<D3D12ImGui>(*dynamic_cast<WindowsWindow*>(Get().Window.get()), *dynamic_cast<D3D12Graphics*>(Get().Graphics.get()));
}

void ZenthEngine::OSShutdown()
{
	Get().Graphics.reset();
	Get().Window.reset();
}
