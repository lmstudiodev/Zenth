#pragma once


namespace ZenthEngine
{
	class IGraphics
	{
	public:
		virtual ~IGraphics() = default;

		virtual const char* GetAPIName() = 0;
		virtual const char* GetGPUName() = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual size_t GetCanvansWidth() = 0;
		virtual size_t GetCanvansHeight() = 0;

	private:

	};
}