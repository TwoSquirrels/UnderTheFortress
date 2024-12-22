# include "UtFScenes.hpp"

# include "Entity/Player.hpp"

using namespace std;

// Shared

const String Shared::Title = U"Under the Fortress";
const String Shared::Version = U"0.1.0";
const LicenseInfo Shared::License = {
	.title = Shared::Title,
	.copyright = U"Copyright (c) 2024 TwoSquirrels",
	.text = String{ UR"(

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

)" }.trim(),
};

// Field scene

Field::Field(const InitData& init)
	: IScene(init), stepSec(1.0 / 59.94), scale(48), m_camera({ 0.0, 0.0 }, scale, CameraControl::None_), m_accumulatorSec(0.0)
{
	if (getData().world) return;

	getData().world = make_shared<World>(U"fortress");
	getData().world->objects << make_shared<Player>(getData().world, Vec3{ 0.0, 0.0, 1.0 }, true);
	player = dynamic_pointer_cast<Player>(getData().world->objects.back());
}

void Field::update()
{
	input.update();
	for (m_accumulatorSec += Scene::DeltaTime(); m_accumulatorSec >= stepSec; m_accumulatorSec -= stepSec)	// NOLINT(cert-flp30-c)
	{
		getData().world->update(input);

		m_camera.setTargetCenter(player.lock()->pos.xy());
		input.reset();
	}
	m_camera.update();
}

void Field::draw() const
{
	Rect{ Scene::Size() }.draw(Palette::Skyblue);
	{
		const auto roundedScale = Round(m_camera.getScale());
		const Transformer2D transformed{
			Mat3x2::Scale(roundedScale).translated(Round(Graphics2D::GetRenderTargetSize() / 2 - m_camera.getCenter() * roundedScale)),
			TransformCursor::Yes,
			Transformer2D::Target::PushCamera,
		};

		getData().world->draw(m_accumulatorSec / stepSec);
	}
	m_camera.draw(Color::Zero());
}

// Title scene

Title::Title(const InitData& init) : IScene(init)
{
}

void Title::update()
{
}

void Title::draw() const
{
}

// Menu scene

Menu::Menu(const InitData& init) : IScene(init)
{
}

void Menu::update()
{
}

void Menu::draw() const
{
}

// Story scene

Story::Story(const InitData& init) : IScene(init)
{
}

void Story::update()
{
}

void Story::draw() const
{
}

// Credit scene

Credit::Credit(const InitData& init) : IScene(init)
{
}

void Credit::update()
{
}

void Credit::draw() const
{
}

// UtFObject

void InitScenes(UtFScenes& scenes)
{
	scenes.add<Title>(U"title");
	scenes.add<Field>(U"field");
	scenes.add<Menu>(U"menu");
	scenes.add<Story>(U"story");
	scenes.add<Credit>(U"credit");

	scenes.init(U"field");
}
