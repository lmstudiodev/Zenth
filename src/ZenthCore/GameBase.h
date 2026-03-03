#pragma once
#include <ZenthCore/ZenthContext.h>
#include <memory>

#define ZENTH_ENGINE_DEFINE_GAME_CLASS(type) \
int main(int argc, char** argv) \
{ \
	return ZenthEngine::RunGame<type>(argc, argv); \
}

namespace ZenthEngine
{
	class GameBase : public ZenthEngine::BasicLayer, public std::enable_shared_from_this<GameBase>
	{
	public:
		GameBase();
		~GameBase();

		int Run();
		int Run(int argc, char** argv);

		bool Update(float dt) override;

	private:

	};

	template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<GameBase, T>>>
	int RunGame(Args&&... args)
	{
		return std::make_shared<T>()->Run(std::forward<Args>(args)...);
	}
}


