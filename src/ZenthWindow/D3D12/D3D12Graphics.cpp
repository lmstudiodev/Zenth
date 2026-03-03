#include "D3D12Graphics.h"

ZenthEngine::D3D12Graphics::D3D12Graphics(WindowsWindow& wnd) : 
	m_window(wnd), 
	m_bufferWidth(wnd.GetWidth()), 
	m_bufferHeight(wnd.GetHeight())
{
	m_debug.EnableDebug();

	CreateFactory();
	CreateAdapter();
	CreateDevice();
	CreateCommandQueue();
	CreateFence();

	m_rtvHeapIncrement = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CreateSwapchain();
	CreateRTV();

	GetBuffers();

	CreateCommandList();

	SetupViewPortAndScrissorRect();
}

void ZenthEngine::D3D12Graphics::CreateFactory()
{
	UINT factoryFlags = 0;

#ifdef CONF_Debug
	factoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	if (FAILED(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&m_dxgiFactory))))
	{
#ifdef CONF_Debug
		std::cout << "[ZENTH_ENGINE] DXGI Factory creation failed" << std::endl;
#endif
	}

#ifdef CONF_Debug
	std::cout << "[ZENTH_ENGINE] DXGI Factory created" << std::endl;
#endif
}

void ZenthEngine::D3D12Graphics::CreateAdapter()
{
	if (FAILED(m_dxgiFactory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&m_gpuAdapter))))
	{
#ifdef CONF_Debug
		std::cout << "[ZENTH_ENGINE] Enumerate adapters failed" << std::endl;
#endif
	}

#ifdef CONF_Debug
	std::cout << "[ZENTH_ENGINE] Enumerate adapters succeeded" << std::endl;
#endif

	DXGI_ADAPTER_DESC adapterDesc{};

	if (FAILED(m_gpuAdapter->GetDesc(&adapterDesc)))
	{
#ifdef CONF_Debug
		std::cout << "[ZENTH_ENGINE] Get adapter description failed " << std::endl;
#endif
	}

#ifdef CONF_Debug
	std::cout << "[ZENTH_ENGINE] Get adapter description succeeded" << std::endl;
#endif

	m_gpuName.reserve(128);
	WideCharToMultiByte(CP_UTF8, 0, adapterDesc.Description, -1, m_gpuName.data(), 128, nullptr, nullptr);

#ifdef CONF_Debug
	std::cout << "[ZENTH_ENGINE] Found " << m_gpuName.c_str() << std::endl;
#endif
}

void ZenthEngine::D3D12Graphics::CreateDevice()
{
	if (FAILED(D3D12CreateDevice(m_gpuAdapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device))))
	{
#ifdef CONF_Debug
		std::cout << "[ZENTH_ENGINE] D3D12 Device creation failed" << std::endl;
#endif
	}

#ifdef CONF_Debug
	std::cout << "[ZENTH_ENGINE] D3D12 Device created" << std::endl;
#endif

	m_gpuAdapter.Release();
}

void ZenthEngine::D3D12Graphics::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC cmdDesc{};
	cmdDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cmdDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	cmdDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdDesc.NodeMask = 0;
	
	if (FAILED(m_device->CreateCommandQueue(&cmdDesc, IID_PPV_ARGS(&m_directQueue))))
	{
#ifdef CONF_Debug
		std::cout << "[ZENTH_ENGINE] D3D12 Command queue creation failed" << std::endl;
#endif
	}

#ifdef CONF_Debug
	std::cout << "[ZENTH_ENGINE] D3D12 Command queue created" << std::endl;
#endif
}

void ZenthEngine::D3D12Graphics::CreateFence()
{
	if (FAILED(m_device->CreateFence(m_directQueueFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_directQueueFence))))
	{
#ifdef CONF_Debug
		std::cout << "[ZENTH_ENGINE] D3D12 Command queue fence creation failed" << std::endl;
#endif
	}

#ifdef CONF_Debug
	std::cout << "[ZENTH_ENGINE] D3D12 Command queue fence created" << std::endl;
#endif

	m_directQueueFenceEvent = CreateEventW(nullptr, false, false, nullptr);
}

void ZenthEngine::D3D12Graphics::CreateSwapchain()
{
	DXGI_SWAP_CHAIN_DESC1 scDesc{};
	scDesc.Width = m_bufferWidth;
	scDesc.Height = m_bufferHeight;
	scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.Stereo = FALSE;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = BufferCount;
	scDesc.Scaling = DXGI_SCALING_STRETCH;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC scFullScreenDesc{};
	scFullScreenDesc.RefreshRate.Numerator = 0;
	scFullScreenDesc.RefreshRate.Denominator = 0;
	scFullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scFullScreenDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	scFullScreenDesc.Windowed = TRUE;
	
	ComPointer<IDXGISwapChain1> tempSwapchain;

	if (FAILED(m_dxgiFactory->CreateSwapChainForHwnd(m_directQueue, m_window.GetWindowHandle(), &scDesc, &scFullScreenDesc, nullptr, &tempSwapchain)))
	{
#ifdef CONF_Debug
		std::cout << "[ZENTH_ENGINE] D3D12 Swapchain creation failed" << std::endl;
#endif
	}

	if (!tempSwapchain.QueryInterface(m_swapChain))
	{
#ifdef CONF_Debug
		std::cout << "[ZENTH_ENGINE] D3D12 Swapchain query interface failed" << std::endl;
#endif
	}

#ifdef CONF_Debug
	std::cout << "[ZENTH_ENGINE] D3D12 Swapchain created" << std::endl;
#endif

}

void ZenthEngine::D3D12Graphics::CreateRTV()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = BufferCount;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	
	m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));

	D3D12_CPU_DESCRIPTOR_HANDLE firstHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (size_t i = 0; i < BufferCount; i++)
	{
		m_rtvHandles[i] = { firstHandle.ptr + i * m_rtvHeapIncrement };
	}
}

void ZenthEngine::D3D12Graphics::CreateCommandList()
{
	m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_cmdAllocator));
	m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmdAllocator, nullptr, IID_PPV_ARGS(&m_cmdList));
}

void ZenthEngine::D3D12Graphics::SetupViewPortAndScrissorRect()
{
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = m_bufferWidth;
	viewPort.Height = m_bufferHeight;
	viewPort.MinDepth = 1.0f;
	viewPort.MaxDepth = 0.0f;

	m_fullRect.left = 0;
	m_fullRect.top = 0;
	m_fullRect.right = m_bufferWidth;
	m_fullRect.bottom = m_bufferHeight;
}

void ZenthEngine::D3D12Graphics::FlushQueue(size_t flushCount)
{
	for (size_t i = 0; i < flushCount; i++)
	{
		m_directQueue->Signal(m_directQueueFence, ++m_directQueueFenceValue);
		
		WaitForFence();
	}
}

void ZenthEngine::D3D12Graphics::WaitForFence(UINT64 fenceValue)
{
	if (fenceValue == -1)
		fenceValue = m_directQueueFenceValue;

	if (m_directQueueFence->GetCompletedValue() < fenceValue)
	{
		
		if (SUCCEEDED(m_directQueueFence->SetEventOnCompletion(fenceValue, m_directQueueFenceEvent)))
		{
			if (WaitForSingleObject(m_directQueueFenceEvent, 30000) == WAIT_OBJECT_0)
				return;
		}

		while (m_directQueueFence->GetCompletedValue() < fenceValue)
		{
			Sleep(100);
		}
	}
}

void ZenthEngine::D3D12Graphics::ResizeSwapchain()
{
	size_t newWidth = m_window.GetWidth();
	size_t newHeight = m_window.GetHeight();

	if (newWidth != m_bufferWidth || newHeight != m_bufferHeight)
	{
		FlushQueue(BufferCount);

		ReleaseBuffers();
		m_swapChain->ResizeBuffers(0, newWidth, newHeight, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING);
		GetBuffers();
		
		m_bufferWidth = newWidth;
		m_bufferHeight = newHeight;

		SetupViewPortAndScrissorRect();
	}
}

void ZenthEngine::D3D12Graphics::GetBuffers()
{
	for (size_t i = 0; i < BufferCount; i++)
	{
		m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_buffers[i]));
		m_device->CreateRenderTargetView(m_buffers[i], nullptr, m_rtvHandles[i]);
	}
}

void ZenthEngine::D3D12Graphics::ReleaseBuffers()
{
	for (size_t i = 0; i < BufferCount; i++)
	{
		m_buffers[i].Release();
	}
}

ZenthEngine::D3D12Graphics::~D3D12Graphics()
{
	FlushQueue(BufferCount);

	ReleaseBuffers();

	m_swapChain.Release();
	
	CloseHandle(m_directQueueFenceEvent);
	
	m_directQueueFence.Release();

	m_directQueue.Release();
	
	m_device.Release();
	
	m_dxgiFactory.Release();

	m_debug.DisableDebug();
}

const char* ZenthEngine::D3D12Graphics::GetAPIName()
{
	return "D3D12";
}

const char* ZenthEngine::D3D12Graphics::GetGPUName()
{
	return m_gpuName.c_str();
}

size_t ZenthEngine::D3D12Graphics::GetCanvansWidth()
{
	return m_bufferWidth;
}

size_t ZenthEngine::D3D12Graphics::GetCanvansHeight()
{
	return m_bufferHeight;
}

void ZenthEngine::D3D12Graphics::ExecuteCommandList()
{
	if (SUCCEEDED(m_cmdList->Close()))
	{
		ID3D12CommandList* lists[] = { m_cmdList };

		m_directQueue->ExecuteCommandLists(1, lists);

		WaitForFence();
	}
}

void ZenthEngine::D3D12Graphics::SetRTBarrier(bool isEndFrame)
{	
	D3D12_RESOURCE_BARRIER rtSetupBarrier{};
	rtSetupBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	rtSetupBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	rtSetupBarrier.Transition.pResource = m_buffers[m_currentBufferIndex];
	rtSetupBarrier.Transition.Subresource = 0;
	rtSetupBarrier.Transition.StateBefore = isEndFrame ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_PRESENT;
	rtSetupBarrier.Transition.StateAfter = isEndFrame ? D3D12_RESOURCE_STATE_PRESENT : D3D12_RESOURCE_STATE_RENDER_TARGET;

	m_cmdList->ResourceBarrier(1, &rtSetupBarrier);
}

void ZenthEngine::D3D12Graphics::BeginFrame()
{
	
	
	ResizeSwapchain();

	m_currentBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
	auto rtvHandle = m_rtvHandles[m_currentBufferIndex];

	float clearColor[] = { 0.4f, 0.4f, 0.8f, 1.0f };

	SetRTBarrier(false);

	m_cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	m_cmdList->RSSetViewports(1, &viewPort);
	m_cmdList->RSSetScissorRects(1, &m_fullRect);
	m_cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
}

void ZenthEngine::D3D12Graphics::EndFrame()
{
	SetRTBarrier(true);

	ExecuteCommandList();

	FlushQueue();
	
	m_swapChain->Present(1, 0);

	m_cmdAllocator->Reset();
	m_cmdList->Reset(m_cmdAllocator, nullptr);
}

