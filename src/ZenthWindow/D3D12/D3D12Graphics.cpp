#include "D3D12Graphics.h"

ZenthEngine::D3D12Graphics::D3D12Graphics()
{
	m_debug.EnableDebug();

	CreateFactory();
	CreateAdapter();
	CreateDevice();
	CreateCommandQueue();
	CreateFence();
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

ZenthEngine::D3D12Graphics::~D3D12Graphics()
{
	FlushQueue(BufferCount);
	
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
