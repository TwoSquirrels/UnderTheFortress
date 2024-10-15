# include "Assets.hpp"

const FilePath Assets::rootDir = U"UnderTheFortress/";

void Assets::RegisterTexture(const AssetNameView name)
{
	TextureAsset::Register(name, Resource(rootDir + name));
}

void Assets::Register()
{
	RegisterTexture(U"tiles/glass.png");
}
