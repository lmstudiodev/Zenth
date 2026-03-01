#include "WindowsEngine.h"
#include <ZenthWindow/WindowsWindow.h>
#include <ZenthWindow/D3D12/D3D12Graphics.h>

void ZenthEngine::OSInit()
{
	Get().Window = std::make_unique<WindowsWindow>();
	Get().Graphics = std::make_unique<D3D12Graphics>();
}

void ZenthEngine::OSShutdown()
{
	Get().Graphics.reset();
	Get().Window.reset();
}
