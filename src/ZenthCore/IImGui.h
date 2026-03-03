#pragma once

namespace ZenthEngine
{
	class IImGui
	{
	public:
		virtual ~IImGui() =default;

		virtual void NewFrame() = 0;
		virtual void Draw() = 0;
	};
}