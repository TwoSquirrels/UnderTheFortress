# include "Game.hpp"

# include "Debug.ipp"

using namespace std;

// UtFObject

UtFObject::~UtFObject() = default;
String UtFObject::getName() const { return U"UtFObject"; }
int32 UtFObject::getUpdatePriority() const { return 0; }
void UtFObject::update(const UtFInput&) {}
int32 UtFObject::getDrawZ() const { return 0; }
void UtFObject::draw(double) const {};

void Formatter(FormatData& formatData, const UtFObject& value)
{
	formatData.string +=
		U"<UtFObject({}) updatePriority={} drawZ={}>"_fmt(
			value.getName(), value.getUpdatePriority(), value.getDrawZ()
		);
}

// Entity

Entity::Entity(const Vec3& pos) : pos(pos), vel(Vec3::Zero()), acc(Vec3::Zero())
{
}

String Entity::getName() const
{
	return U"Entity";
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

String LivingEntity::getName() const
{
	return U"LivingEntity";
}

bool LivingEntity::damage(const int64 amount, const String& reason)
{
	if (amount < 0) return true;
	hp -= Min(hp, amount);
	return hp > 0;
}

// World

World::World(const String& worldName)
{
	const JSON world = JSON::Load(Resource(U"UnderTheFortress/worlds/{}.json"_fmt(worldName)), AllowExceptions::Yes);
	tileMap = TileMap(Size{ world[U"size"][1].get<size_t>(), world[U"size"][0].get<size_t>() }, world[U"map"]);
}

void World::update(const UtFInput& input)
{
	if (input.down(KeyF3)) DEBUG_PRINT(U"World::update()");

	objects
		.stable_sorted_by([](const auto& a, const auto& b) { return a->getUpdatePriority() < b->getUpdatePriority(); })
		.each([&input](const auto& object) { object->update(input); });
}

void World::draw(const double accumulatorStep) const
{
	DEBUG_PRINT_F3(U"World::draw()");

	const auto sortedObjects = objects
		.stable_sorted_by([](const auto& a, const auto& b) { return a->getDrawZ() > b->getDrawZ(); });
	auto objectIter = sortedObjects.begin();

	for (const auto layer : step_backward(Tile::LayerCount))
	{
		// entity
		for (; objectIter != sortedObjects.end() && (*objectIter)->getDrawZ() >= static_cast<int32>(layer); ++objectIter)
		{
			DEBUG_DUMP_F3((*objectIter)->getDrawZ());
			(*objectIter)->draw(accumulatorStep);
		}

		// tile
		DEBUG_DUMP_F3(layer);
		tileMap.draw(layer);
	}

	// object
	for (; objectIter != sortedObjects.end(); ++objectIter)
	{
		DEBUG_DUMP_F3((*objectIter)->getDrawZ());
		(*objectIter)->draw(accumulatorStep);
	}
}
