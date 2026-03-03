#pragma once

namespace ZenthEngine
{
	class BasicLayer
	{
	public:
		virtual ~BasicLayer() = default;

		inline int GetLayerIndex()
		{
			return m_index;
		}

		inline void SetLayerIndex(int index)
		{
			m_index = index;
		}

		virtual bool Update(float dt) = 0;

		virtual void Draw() {};

		virtual void OnAttached() {};

		virtual void OnDetached() {};

	private:
		int m_index = 0;
	};
}