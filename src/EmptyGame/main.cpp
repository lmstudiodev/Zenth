#include <ZenthCore/ZenthContext.h>
#include <imgui/imgui.h>

class EmptygameLayer : public ZenthEngine::ILayer
{
public:
	bool Update(float dt) override
	{
		ImGui::GetIO().FontGlobalScale = 2.0;
		ImGui::ShowDemoWindow();

		return true;
	}
};

int main()
{
	ZenthEngine::Init();
	
	auto& wnd = *ZenthEngine::Get().Window;
	auto& gfx = *ZenthEngine::Get().Graphics;
	auto& stack = ZenthEngine::Get().LayerStack;

	stack.AttachLayer(std::make_shared<EmptygameLayer>());

	while (!wnd.CloseRequested())
	{
		wnd.Update();

		gfx.BeginFrame();

		stack.UpdateLayers(0.0f);

		gfx.EndFrame();
	}

	ZenthEngine::ShutDown();

	return 0;
}