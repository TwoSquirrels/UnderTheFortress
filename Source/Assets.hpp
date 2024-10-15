# pragma once

# include <Siv3D.hpp>

namespace Assets
{
	extern const FilePath rootDir;

	void RegisterTexture(AssetNameView name);

	//void RegisterFont(AssetNameView name);

	//void RegisterAudio(AssetNameView name);
	
	void Register();
}
