#pragma once
#include <ZenthCore/BasicLayer.h>
#include <set>
#include <memory>

namespace ZenthEngine
{
	class LayerStackManager
	{
	public:
		~LayerStackManager();

		void AttachLayer(const std::shared_ptr<BasicLayer>& layer);
		bool DetachLayer(const std::shared_ptr<BasicLayer>& layer);
		bool UpdateLayers(float dt);
		void ClearLayers();

	private:
		struct  CompareStruct
		{
			bool operator()(const std::shared_ptr<BasicLayer>& lhs, const std::shared_ptr<BasicLayer>& rhs) const
			{
				return lhs->GetLayerIndex() < rhs->GetLayerIndex();
			}
		};

	private:
		std::set<std::shared_ptr<BasicLayer>, CompareStruct> m_layers;
	};
}


