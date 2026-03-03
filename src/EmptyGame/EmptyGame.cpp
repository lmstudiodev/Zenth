#include "EmptyGame.h"


bool EmptyGame::Update(float dt)
{
	ImGui::GetIO().FontGlobalScale = 2.0;
	ImGui::ShowDemoWindow();

	return true;
}