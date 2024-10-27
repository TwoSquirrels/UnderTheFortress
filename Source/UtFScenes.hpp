# pragma once

# include "Entity/Player.hpp"
# include "Game.hpp"
# include "UtFInput.hpp"

# include <Siv3D.hpp>

struct Shared final
{
	static const String Title;
	static const String Version;
	static const LicenseInfo License;

	Optional<World> world;
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
	UtFInput input;
	double stepSec;
	double scale;
	std::weak_ptr<Player> player;

	explicit Field(const InitData& init);
	void update() override;
	void draw() const override;

private:
	Camera2D m_camera;
	double m_accumulatorSec;
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

/// @brief Initialize the scenes.
/// @param scenes The scene manager
void InitScenes(UtFScenes& scenes);
