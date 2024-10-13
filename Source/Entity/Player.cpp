# include "Player.hpp"

# include "../Debug.ipp"

using namespace std;

Player::Player(const Vec3& pos)
	: LivingEntity(pos, 3), maxSpeed(0.125), friction(0.25)
{
}

String Player::getName() const
{
	return U"Player";
}

int32 Player::getUpdatePriority() const
{
	return 0;
}

void Player::update()
{
	DEBUG_PRINT_F3(U"Player::update()");

	// input movement
	maxSpeed = KeyShift.pressed() ? 0.25 : 0.125;
	Vec3 force = Vec3::Zero();
	if (KeyW.pressed()) force.y -= 1;
	if (KeyS.pressed()) force.y += 1;
	if (KeyA.pressed()) force.x -= 1;
	if (KeyD.pressed()) force.x += 1;
	acc += force.normalized() * maxSpeed * friction;

	// friction
	acc += vel * -friction;

	move();

	DEBUG_DUMP_F3(*this);
}

int32 Player::getDrawZ() const
{
	return static_cast<int32>(pos.z);
}

void Player::draw() const
{
	DEBUG_PRINT_F3(U"Player::draw()");

	Circle{ pos.xy(), 0.75 }.draw(Palette::Red);
}

void Formatter(FormatData& formatData, const Player& value)
{
	formatData.string +=
		U"<Player updatePriority={} drawZ={} pos={} vel={} acc={} hp={} maxHp={} maxSpeed={} friction={}>"_fmt(
			value.getUpdatePriority(), value.getDrawZ(),
			value.pos, value.vel, value.acc,
			value.hp, value.maxHp,
			value.maxSpeed, value.friction
		);
}
