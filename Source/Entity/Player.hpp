# pragma once

# include "../Game.hpp"

# include <Siv3D.hpp>

class Player final : public LivingEntity
{
public:
	double walkSpeed;
	double dashSpeed;
	double friction;
	double stamina; // TODO: implement stamina
	double maxStamina; // TODO: implement stamina

	explicit Player(const Vec3& pos = Vec3{ 0.0, 0.0, 1.0 }, double maxStamina = 10.0);

	[[nodiscard]] ObjectType type() const override;
	[[nodiscard]] int32 getUpdatePriority() const override;
	void update(const UtFInput& input) override;
	[[nodiscard]] int32 getDrawZ() const override;
	void draw(double accumulatorStep) const override;

	friend void Formatter(FormatData& formatData, const Player& value);
};
