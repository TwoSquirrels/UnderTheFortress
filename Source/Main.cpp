# include "UtFScenes.hpp"
# include "Game.hpp"

# include <Siv3D.hpp>

using namespace std;

void Main()
{
	LicenseManager::SetApplicationLicense(Shared::title, Shared::license);

	Window::SetMinimumFrameBufferSize(Size{ 256, 144 });
	Window::Resize(1280, 720);
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::SetStyle(WindowStyle::Sizable);
	Window::Resize(640, 360);
	//Window::SetFullscreen(true);

	Window::SetTitle(U"{} v{}"_fmt(Shared::title, Shared::version));

	Scene::SetBackground(Palette::Black);

	UtFScenes scenes;
	initScenes(scenes);

	while (System::Update() && scenes.update())
	{
	}
}
