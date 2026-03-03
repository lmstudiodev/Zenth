#pragma once
#include <ZenthCore/IGraphics.h>
#include <ZenthCore/IImGui.h>
#include <ZenthCore/ZenthContext.h>
#include <ZenthWindow/WinInclude.h>
#include <ZenthWindow/WindowsWindow.h>
#include <ZenthWindow/D3D12/D3D12Debug.h>

#include <string>

namespace ZenthEngine
{
	class D3D12Graphics : public IGraphics
	{
	public:
		static const size_t BufferCount = 2;

	public:
		D3D12Graphics();
		~D3D12Graphics();

	public:

		const char* GetAPIName() override;
		const char* GetGPUName() override;

		void BeginFrame() override;
		void EndFrame() override;

		size_t GetCanvansWidth() override;
		size_t GetCanvansHeight() override;

		auto GetDevice()
		{
			return m_device;
		}

		auto GetCommandList()
		{
			return m_cmdList;
		}

	private:
		void CreateFactory();
		void CreateAdapter();
		void CreateDevice();
		void CreateCommandQueue();
		void CreateFence();
		void CreateSwapchain();
		void CreateRTV();
		void CreateCommandList();
		void SetupViewPortAndScrissorRect();
		void SetRTBarrier(bool isEndFrame);
		void ExecuteCommandList();

		void FlushQueue(size_t flushCount = 1);
		void WaitForFence(UINT64 fenceValue = -1);

		void ResizeSwapchain();

		void GetBuffers();
		void ReleaseBuffers();

	private:
		//IImGui& m_imgui;
		//WindowsWindow& m_window;

		D3D12Debug m_debug;

		ComPointer<IDXGIFactory7> m_dxgiFactory;
		ComPointer<IDXGIAdapter> m_gpuAdapter;
		ComPointer<ID3D12Device> m_device;
		ComPointer<ID3D12CommandQueue> m_directQueue;
		ComPointer<ID3D12Fence> m_directQueueFence;
		ComPointer<IDXGISwapChain3> m_swapChain;
		ComPointer<ID3D12Resource> m_buffers[BufferCount];
		ComPointer<ID3D12DescriptorHeap> m_rtvHeap;
		ComPointer<ID3D12CommandAllocator> m_cmdAllocator;
		ComPointer<ID3D12GraphicsCommandList> m_cmdList;

		D3D12_CPU_DESCRIPTOR_HANDLE m_rtvHandles[BufferCount];
		D3D12_VIEWPORT viewPort;
		D3D12_RECT m_fullRect;

		UINT64 m_directQueueFenceValue = 0;
		HANDLE m_directQueueFenceEvent = nullptr;

		std::string m_gpuName;

		size_t m_bufferWidth;
		size_t m_bufferHeight;
		size_t m_activeBufferIndex;

		UINT64 m_rtvHeapIncrement = 0;
		UINT m_currentBufferIndex = -1;
	};
}


