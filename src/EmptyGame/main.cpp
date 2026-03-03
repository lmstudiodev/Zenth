#include <ZenthCore/ZenthContext.h>
#include <imgui/imgui.h>

int main()
{
	ZenthEngine::Init();
	
	auto& wnd = *ZenthEngine::Get().Window;
	auto& gfx = *ZenthEngine::Get().Graphics;

	while (!wnd.CloseRequested())
	{
		wnd.Update();

		gfx.BeginFrame();

		ImGui::ShowDemoWindow();

		gfx.EndFrame();
	}

	ZenthEngine::ShutDown();

	return 0;
}