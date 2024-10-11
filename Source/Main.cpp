//# include "Title.hpp"
# include "Game.hpp"
//# include "Menu.hpp"
//# include "Story.hpp"
//# include "Credit.hpp"

# include "Shared.hpp"

# include <Siv3D.hpp>

void Main()
{
	LicenseManager::SetApplicationLicense(Shared::title, Shared::license);

	Window::SetMinimumFrameBufferSize(Size{ 256, 144 });
	Window::Resize(1280, 720);
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::SetStyle(WindowStyle::Sizable);
	Window::Resize(640, 360);
	Window::SetFullscreen(true);

	Window::SetTitle(U"{} v{}"_fmt(Shared::title, Shared::version));

	Scene::SetBackground(Palette::Black);

	UtFScenes scenes;
	//scenes.add<Title>(U"title");
	scenes.add<Game>(U"game");
	//scenes.add<Menu>(U"menu");
	//scenes.add<Story>(U"story");
	//scenes.add<Credit>(U"credit");

	while (System::Update() && scenes.update())
	{
	}
}
