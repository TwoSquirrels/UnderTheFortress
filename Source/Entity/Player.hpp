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
	bool operable;

	explicit Player(
		const std::weak_ptr<World>& world = {}, const Vec3& pos = Vec3{ 0.0, 0.0, 2.0 },
		bool operable = false, double maxStamina = 10.0
	);

	[[nodiscard]] ObjectType type() const override;
	[[nodiscard]] double getUpdatePriority() const override;
	void update(const UtFInput& input) override;
	void draw(double accumulatorStep) const override;

	friend void Formatter(FormatData& formatData, const Player& value);
};
