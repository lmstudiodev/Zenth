#pragma once

#include <ZenthWindow/WinInclude.h>

namespace ZenthEngine
{
	class D3D12Debug
	{
	public:
		void EnableDebug();
		void DisableDebug();

	private:
#ifdef CONF_Debug
		ComPointer<ID3D12Debug> m_d3d12Debug = nullptr;
		ComPointer<IDXGIDebug1> m_dxgiDebug = nullptr;
#endif



	};
}



