#pragma once

namespace ZenthEngine
{
	class ILayer
	{
	public:
		virtual ~ILayer() = default;

		virtual bool Update(float dt) = 0;

		virtual void Draw() {};

		virtual void OnAttached() {};

		virtual void OnDetached() {};
	};
}