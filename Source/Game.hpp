# pragma once

# include "Shared.hpp"

# include <Siv3D.hpp>

class Game final : public UtFScenes::Scene
{
public:
	explicit Game(const InitData& init);
	void update() override;
	void draw() const override;

private:
};
