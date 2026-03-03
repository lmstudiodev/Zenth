#include "WindowsEngine.h"
#include <ZenthWindow/WindowsWindow.h>
#include <ZenthWindow/D3D12/D3D12Graphics.h>
#include <ZenthWindow/D3D12/D3D12ImGui.h>

void ZenthEngine::OSInit()
{
	Get().Window = std::make_unique<WindowsWindow>();
	Get().Graphics = std::make_unique<D3D12Graphics>();
	Get().Imgui = std::make_unique<D3D12ImGui>();
}

void ZenthEngine::OSShutdown()
{
	Get().Imgui.reset();
	Get().Graphics.reset();
	Get().Window.reset();
}
