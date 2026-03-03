#include "LayerStackManager.h"

ZenthEngine::LayerStackManager::~LayerStackManager()
{
	ClearLayers();
}

void ZenthEngine::LayerStackManager::AttachLayer(const std::shared_ptr<BasicLayer>& layer)
{
	DetachLayer(layer);

	m_layers.emplace(layer);

	layer->OnAttached();
}

bool ZenthEngine::LayerStackManager::DetachLayer(const std::shared_ptr<BasicLayer>& layer)
{
	auto existingLayerIt = std::find(m_layers.begin(), m_layers.end(), layer);

	if (existingLayerIt != m_layers.end())
	{
		m_layers.erase(existingLayerIt);

		layer->OnDetached();

		return true;
	}
	
	return false;
}

bool ZenthEngine::LayerStackManager::UpdateLayers(float dt)
{
	for (auto& layer : m_layers)
	{
		if (!layer->Update(dt))
			return false;
	}

	for (auto& layer : m_layers)
	{
		layer->Draw();
	}
	
	return true;
}

void ZenthEngine::LayerStackManager::ClearLayers()
{
	while (!m_layers.empty())
	{
		m_layers.begin()->get()->OnDetached();
		m_layers.erase(m_layers.begin());
	}
}
