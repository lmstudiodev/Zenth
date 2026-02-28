#include "WindowsEngine.h"
#include <ZenthWindow/WindowsWindow.h>

void ZenthEngine::OSInit()
{
	Get().Window = std::make_unique<WindowsWindow>();
}

void ZenthEngine::OSShutdown()
{
	Get().Window.reset();
}
