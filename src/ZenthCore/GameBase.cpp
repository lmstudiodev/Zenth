#include "GameBase.h"

ZenthEngine::GameBase::GameBase()
{
	ZenthEngine::Init();

	SetLayerIndex(100);
}

ZenthEngine::GameBase::~GameBase()
{
	ZenthEngine::ShutDown();
}

int ZenthEngine::GameBase::Run()
{
	ZenthEngine::Get().LayerStack.AttachLayer(shared_from_this());
	
	auto& wnd = *ZenthEngine::Get().Window;
	auto& gfx = *ZenthEngine::Get().Graphics;
	auto& stack = ZenthEngine::Get().LayerStack;

	bool shouldExit = false;

	while (!shouldExit)
	{
		wnd.Update();

		shouldExit |= wnd.CloseRequested();

		gfx.BeginFrame();

		shouldExit |= !stack.UpdateLayers(0.0f);

		gfx.EndFrame();
	}
	
	return 0;
}

int ZenthEngine::GameBase::Run(int argc, char** argv)
{	
	return Run();
}

bool ZenthEngine::GameBase::Update(float dt)
{
	return true;
}
