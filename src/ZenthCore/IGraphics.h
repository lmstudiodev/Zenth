#pragma once


namespace ZenthEngine
{
	class IGraphics
	{
	public:
		virtual ~IGraphics() = default;

		virtual const char* GetAPIName() = 0;
		virtual const char* GetGPUName() = 0;

	private:

	};
}