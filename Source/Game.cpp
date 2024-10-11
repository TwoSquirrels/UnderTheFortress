# include "Game.hpp"

using namespace std;

// UtFObject

UtFObject::~UtFObject() = default;

int32 UtFObject::getUpdatePriority() const { return 0; }

void UtFObject::update() {}

int32 UtFObject::getDrawZ() const { return 0; }

void UtFObject::draw() const {}

// Entity

Entity::Entity(const Vec3& pos) : pos(pos), vel(Vec3::Zero()), acc(Vec3::Zero())
{
}

void Entity::move(const bool resetAcc)
{
	vel += acc;
	pos += vel;
	if (resetAcc) acc = Vec3::Zero();
}

// LivingEntity

LivingEntity::LivingEntity(const Vec3& pos, const int64 maxHp) : Entity(pos), hp(maxHp), maxHp(maxHp)
{
}

bool LivingEntity::damage(const int64 amount, const String& reason)
{
	if (amount < 0) return true;
	hp -= Min(hp, amount);
	return hp > 0;
}

// World

void World::update()
{
	entities
		.map([](const auto& entity) -> pair<int32, shared_ptr<UtFObject>> { return { entity->getUpdatePriority(), entity }; })
		.stable_sorted_by([](const auto& a, const auto& b) { return a.first < b.first; })
		.each([](const auto& entityWithPriority) { entityWithPriority.second->update(); });
}

void World::draw() const
{
	entities
		.map([](const auto& entity) -> pair<int32, shared_ptr<UtFObject>> { return { entity->getDrawZ(), entity }; })
		.stable_sorted_by([](const auto& a, const auto& b) { return a.first < b.first; })
		.each([](const auto& entityWithPriority) { entityWithPriority.second->draw(); });
}
