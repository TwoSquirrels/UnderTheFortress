# include "Player.hpp"

# include "../Debug.ipp"

using namespace std;

Player::Player(const std::weak_ptr<World>& world, const Vec3& pos, const bool operable, const double maxStamina)
	: LivingEntity(world, pos, 3), walkSpeed(0.125), dashSpeed(0.25), friction(0.25),
	stamina(maxStamina), maxStamina(maxStamina), operable(operable)
{
}

ObjectType Player::type() const
{
	return ObjectType::Player;
}

double Player::getUpdatePriority() const
{
	return 0.0;
}

void Player::update(const UtFInput& input)
{
	DEBUG_PRINT_F3(U"Player::update()");

	// input movement
	if (operable)
	{
		Vec3 force = Vec3::Zero();
		if (input.pressed(KeyW | KeyUp)) force.y -= 1;
		if (input.pressed(KeyS | KeyDown)) force.y += 1;
		if (input.pressed(KeyA | KeyLeft)) force.x -= 1;
		if (input.pressed(KeyD | KeyRight)) force.x += 1;
		acc += force.normalized() * (input.pressed(KeyShift) ? dashSpeed : walkSpeed) * friction;
		if (input.down(KeySpace)) acc.z = 0.375;
	}

	decelerate(Vec3{ friction, friction, 0.0 });
	gravitate();

	move();

	// temporary floor collision
	if (pos.z < 2.0)
	{
		pos.z = 2.0;
		vel.z = 0;
	}

	DEBUG_DUMP_F3(*this);
}

void Player::draw(const double accumulatorStep) const
{
	DEBUG_PRINT_F3(U"Player::draw()");

	Circle{ pos.xy() + vel.xy() * accumulatorStep * 0.75, 0.75 - pos.z / 24 }.draw(Palette::Red);
}

void Formatter(FormatData& formatData, const Player& value)
{
	formatData.string +=
		U"<Player updatePriority={} drawZ={} pos={} vel={} acc={} hp={} maxHp={} walkSpeed={} dashSpeed={} friction={} stamina={}>"_fmt(
			value.getUpdatePriority(), value.getDrawZ(),
			value.pos, value.vel, value.acc,
			value.hp, value.maxHp,
			value.walkSpeed, value.dashSpeed, value.friction, value.stamina
		);
}
