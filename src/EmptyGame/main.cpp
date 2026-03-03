#include <ZenthCore/ZenthContext.h>
#include <imgui/imgui.h>
#include <Imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx12.h>

int main()
{
	ZenthEngine::Init();
	
	auto& wnd = *ZenthEngine::Get().Window;
	auto& gfx = *ZenthEngine::Get().Graphics;

	while (!wnd.CloseRequested())
	{
		wnd.Update();

		gfx.BeginFrame();

		gfx.EndFrame();
	}

	ZenthEngine::ShutDown();

	return 0;
}