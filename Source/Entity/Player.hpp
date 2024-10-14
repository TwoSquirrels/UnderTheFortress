# pragma once

# include "../Game.hpp"

# include <Siv3D.hpp>

class Player final : public LivingEntity
{
public:
	double maxSpeed;
	double friction;

	explicit Player(const Vec3& pos = Vec3{ 0.0, 0.0, 1.0 });

	[[nodiscard]] String getName() const override;
	[[nodiscard]] int32 getUpdatePriority() const override;
	void update(const UtFInput& input) override;
	[[nodiscard]] int32 getDrawZ() const override;
	void draw(double accumulatorStep) const override;

	friend void Formatter(FormatData& formatData, const Player& value);
};
