#pragma once
#include <ZenthCore/IWindow.h>
#include <ZenthCore/IGraphics.h>
#include <ZenthCore/LayerStackManager.h>

#include <memory>

namespace ZenthEngine
{
	struct ZenthContext
	{
		std::shared_ptr<IWindow> Window;
		std::shared_ptr<IGraphics> Graphics;
		std::shared_ptr<BasicLayer> Imgui;

		LayerStackManager LayerStack;
	};

	void Init();
	void ShutDown();
	ZenthContext& Get();
}