#include <ZenthCore/ZenthContext.h>

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