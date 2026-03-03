#pragma once
#include <ZenthCore/IWindow.h>
#include <ZenthCore/IGraphics.h>
#include <ZenthCore/IImGui.h>

#include <memory>

namespace ZenthEngine
{
	struct ZenthContext
	{
		std::unique_ptr<IWindow> Window;
		std::unique_ptr<IGraphics> Graphics;
		std::unique_ptr<IImGui> Imgui;
	};

	void Init();
	void ShutDown();
	ZenthContext& Get();
}