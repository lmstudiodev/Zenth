#pragma once
#include <ZenthCore/GameBase.h>
#include <imgui/imgui.h>

class EmptyGame : public ZenthEngine::GameBase
{
public:
	bool Update(float dt) override;
};
