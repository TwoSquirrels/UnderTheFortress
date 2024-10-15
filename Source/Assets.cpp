# include "Assets.hpp"

const FilePath Assets::rootDir = U"UnderTheFortress/";

void Assets::RegisterTexture(const FilePath& path)
{
	TextureAsset::Register(path, Resource(rootDir + path));
}

void Assets::Register()
{
	RegisterTexture(U"tiles/glass.png");
}
