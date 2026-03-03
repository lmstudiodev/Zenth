#include "ZenthContext.h"

namespace ZenthEngine
{
	void OSInit();
	void OSShutdown();
}

void ZenthEngine::Init()
{
	OSInit();

	auto& ctx = Get();

	ctx.Imgui->SetLayerIndex(90);
	ctx.LayerStack.AttachLayer(ctx.Imgui);
}

void ZenthEngine::ShutDown()
{
	Get().LayerStack.ClearLayers();
	
	OSShutdown();
}

ZenthEngine::ZenthContext& ZenthEngine::Get()
{
	static ZenthContext ctx;
	return ctx;
}