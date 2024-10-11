# include "UtFScenes.hpp"

# include "Entity/Player.hpp"

using namespace std;

// Shared

const String Shared::title = U"Under the Fortress";
const String Shared::version = U"0.1.0";
const LicenseInfo Shared::license = {
	.title = Shared::title,
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

Field::Field(const InitData& init) : IScene(init)
{
	if (not getData().world.entities.empty()) return;
	getData().world.entities << make_shared<Player>();
}

void Field::update()
{
	getData().world.update();

}

void Field::draw() const
{
	getData().world.draw();
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

void initScenes(UtFScenes &scenes)
{
	scenes.add<Title>(U"title");
	scenes.add<Field>(U"field");
	scenes.add<Menu>(U"menu");
	scenes.add<Story>(U"story");
	scenes.add<Credit>(U"credit");

	scenes.init(U"field");
}
