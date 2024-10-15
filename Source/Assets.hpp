# pragma once

# include <Siv3D.hpp>

namespace Assets
{
	extern const FilePath rootDir;

	void RegisterTexture(const FilePath& path);

	//void RegisterFont(const FilePath& path);

	//void RegisterAudio(const FilePath& path);

	void Register();
}
