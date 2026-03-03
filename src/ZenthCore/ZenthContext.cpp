#include "ZenthContext.h"

namespace ZenthEngine
{
	void OSInit();
	void OSShutdown();
}

void ZenthEngine::Init()
{
	OSInit();

	Get().LayerStack.AttachLayer(Get().Imgui);
}

void ZenthEngine::ShutDown()
{
	OSShutdown();
}

ZenthEngine::ZenthContext& ZenthEngine::Get()
{
	static ZenthContext ctx;
	return ctx;
}