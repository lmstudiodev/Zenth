#pragma once
#include <ZenthCore/IWindow.h>

#include <memory>

namespace ZenthEngine
{
	struct ZenthContext
	{
		std::unique_ptr<IWindow> Window;
	};

	void Init();
	void ShutDown();
	ZenthContext& Get();
}