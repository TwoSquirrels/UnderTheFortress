# pragma once

# include "Game.hpp"

# include <Siv3D.hpp>

struct Shared final
{
	static const String title;
	static const String version;
	static const LicenseInfo license;

	World world;
};

using UtFScenes = SceneManager<String, Shared>;

class Title final : public UtFScenes::Scene
{
public:
	explicit Title(const InitData& init);
	void update() override;
	void draw() const override;
};

class Field final : public UtFScenes::Scene
{
public:
	explicit Field(const InitData& init);
	void update() override;
	void draw() const override;
};

class Menu final : public UtFScenes::Scene
{
public:
	explicit Menu(const InitData& init);
	void update() override;
	void draw() const override;
};

class Story final : public UtFScenes::Scene
{
public:
	explicit Story(const InitData& init);
	void update() override;
	void draw() const override;
};

class Credit final : public UtFScenes::Scene
{
public:
	explicit Credit(const InitData& init);
	void update() override;
	void draw() const override;
};

void initScenes(UtFScenes& scenes);
