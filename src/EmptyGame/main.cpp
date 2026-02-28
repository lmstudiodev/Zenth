#include <ZenthCore/ZenthContext.h>

int main()
{
	ZenthEngine::Init();
	
	auto& wnd = *ZenthEngine::Get().Window;

	while (!wnd.CloseRequested())
	{
		wnd.Update();
	}

	ZenthEngine::ShutDown();

	return 0;
}