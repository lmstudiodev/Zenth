#pragma once
#include <ZenthCore/ILayer.h>
#include <vector>
#include <memory>

namespace ZenthEngine
{
	class LayerStackManager
	{
	public:
		void AttachLayer(const std::shared_ptr<ILayer>& layer);
		bool DetachLayer(const std::shared_ptr<ILayer>& layer);
		bool UpdateLayers(float dt);

	private:
		std::vector<std::shared_ptr<ILayer>> m_layers;
	};
}


