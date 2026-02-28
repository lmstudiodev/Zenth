#pragma once

namespace ZenthEngine
{
	class IWindow
	{
	public:
		~IWindow() = default;


		virtual void Update() = 0;
		virtual bool CloseRequested() = 0;
		virtual size_t GetWidth() = 0;
		virtual size_t GetHeight() = 0;
	};
}