# include "Player.hpp"

# include "../Debug.ipp"

using namespace std;

Player::Player(const Vec3& pos, const double maxStamina)
	: LivingEntity(pos, 3), walkSpeed(0.125), dashSpeed(0.25), friction(0.25), stamina(maxStamina), maxStamina(maxStamina)
{
}

ObjectType Player::type() const
{
	return ObjectType::Player;
}

int32 Player::getUpdatePriority() const
{
	return 0;
}

void Player::update(const UtFInput& input)
{
	if (input.down(KeyF3)) DEBUG_PRINT(U"Player::update()");

	// input movement
	Vec3 force = Vec3::Zero();
	if (input.pressed(KeyW | KeyUp)) force.y -= 1;
	if (input.pressed(KeyS | KeyDown)) force.y += 1;
	if (input.pressed(KeyA | KeyLeft)) force.x -= 1;
	if (input.pressed(KeyD | KeyRight)) force.x += 1;
	acc += force.normalized() * (input.pressed(KeyShift) ? dashSpeed : walkSpeed) * friction;

	// friction
	acc += vel * -friction;

	move();

	if (input.down(KeyF3)) DEBUG_DUMP(*this);
}

int32 Player::getDrawZ() const
{
	return static_cast<int32>(pos.z);
}

void Player::draw(const double accumulatorStep) const
{
	DEBUG_PRINT_F3(U"Player::draw()");

	Circle{ pos.xy() + vel.xy() * accumulatorStep * 0.75, 0.75 }.draw(Palette::Red);
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
