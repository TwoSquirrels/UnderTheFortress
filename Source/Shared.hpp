# pragma once

# include <Siv3D.hpp>

struct Shared
{
	static const String title;
	static const String version;
	static const LicenseInfo license;
};

using UtFScenes = SceneManager<String, Shared>;
