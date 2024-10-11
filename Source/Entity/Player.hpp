# pragma once

# include "../Game.hpp"

# include <Siv3D.hpp>

class Player final : public LivingEntity
{
public:
	explicit Player(const Vec3& pos = Vec3::Zero());

	int32 getUpdatePriority() const override;
	void update() override;
	int32 getDrawZ() const override;
	void draw() const override;
};
