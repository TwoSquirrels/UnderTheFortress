# include "Assets.hpp"
# include "UtFScenes.hpp"
# include "Game.hpp"
# include "Debug.ipp"

# include <Siv3D.hpp>

using namespace std;

void Main()
{
	LicenseManager::SetApplicationLicense(Shared::Title, Shared::License);

	Window::SetMinimumFrameBufferSize(Size{ 256, 144 });
	Window::Resize(1280, 720);
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::SetStyle(WindowStyle::Sizable);
	Window::Resize(640, 360);
	//Window::SetFullscreen(true);

	Window::SetTitle(U"{} v{}"_fmt(Shared::Title, Shared::Version));

	Scene::SetBackground(Palette::Black);
	Scene::SetLetterbox(Debug::Enabled ? Palette::Red : Palette::Black);

	UtFScenes scenes;
	InitScenes(scenes);

	Assets::Register();

	while (System::Update() && scenes.update())
	{
		if (KeyDelete.down()) ClearPrint();
	}
}
