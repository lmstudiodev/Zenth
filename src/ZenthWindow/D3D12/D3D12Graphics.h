#pragma once
#include <ZenthCore/IGraphics.h>
#include <ZenthWindow/WinInclude.h>
#include <ZenthWindow/D3D12/D3D12Debug.h>

#include <string>

namespace ZenthEngine
{
	class D3D12Graphics : public IGraphics
	{
	public:
		static const int BufferCount = 2;

	public:
		D3D12Graphics();
		~D3D12Graphics();

	public:

		const char* GetAPIName() override;
		const char* GetGPUName() override;

	private:
		void CreateFactory();
		void CreateAdapter();
		void CreateDevice();
		void CreateCommandQueue();
		void CreateFence();

		void FlushQueue(size_t flushCount = 1);
		void WaitForFence(UINT64 fenceValue = -1);

	private:
		D3D12Debug m_debug;

		ComPointer<IDXGIFactory7> m_dxgiFactory;
		ComPointer<IDXGIAdapter> m_gpuAdapter;
		ComPointer<ID3D12Device> m_device;
		ComPointer<ID3D12CommandQueue> m_directQueue;
		ComPointer<ID3D12Fence> m_directQueueFence;

		UINT64 m_directQueueFenceValue = 0;
		HANDLE m_directQueueFenceEvent = nullptr;

		std::string m_gpuName;

	};
}


