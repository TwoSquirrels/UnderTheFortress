# include "Player.hpp"

using namespace std;

Player::Player(const Vec3& pos)
	: LivingEntity(pos, 3)
{
}

int32 Player::getUpdatePriority() const
{
	return 0;
}

void Player::update()
{
	if (KeyW.pressed()) acc.y -= 0.1;
	if (KeyS.pressed()) acc.y += 0.1;
	if (KeyA.pressed()) acc.x -= 0.1;
	if (KeyD.pressed()) acc.x += 0.1;
	move();
}

int32 Player::getDrawZ() const
{
	return Floor(-pos.z);
}

void Player::draw() const
{
	Circle(pos.xy(), 10).draw(Palette::Red);
}
