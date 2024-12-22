# include "Game.hpp"

# include "Debug.ipp"

using namespace std;

// ObjectType

StringView ToId(const ObjectType type)
{
	static array<String, 0x100> idTable =
	{
		U"error", U"player"
	};
	return idTable[FromEnum(type)];
}

Optional<ObjectType> ToObjectType(const StringView id)
{
	static HashTable<String, ObjectType> typeTable;
	if (typeTable.empty())
	{
		for (const uint8 i : step(0x100))
		{
			const auto type = ToEnum<ObjectType>(i);
			typeTable[ToId(type)] = type;
		}
	}
	return typeTable.contains(id) ? Optional{ typeTable[id] } : none;
}

void Formatter(FormatData& formatData, const ObjectType value)
{
	formatData.string += ToId(value);
}

// UtFObject

UtFObject::UtFObject(const std::weak_ptr<World>& world) : world(world)
{
}

double UtFObject::getUpdatePriority() const { return 0.0; }
void UtFObject::update(const UtFInput&) {}
double UtFObject::getDrawZ() const { return 0.0; }
void UtFObject::draw(double) const {};

void Formatter(FormatData& formatData, const UtFObject& value)
{
	formatData.string +=
		U"<UtFObject({}) updatePriority={} drawZ={}>"_fmt(
			Format(value.type()), value.getUpdatePriority(), value.getDrawZ()
		);
}

// Entity

Entity::Entity(const std::weak_ptr<World>& world, const Vec3& pos) : UtFObject(world), pos(pos), vel(Vec3::Zero()), acc(Vec3::Zero())
{
}

double Entity::getDrawZ() const
{
	return pos.z;
}

void Entity::move(const bool resetAcc)
{
	vel += acc;
	pos += vel;
	if (resetAcc) acc = Vec3::Zero();
}

void Entity::gravitate()
{
	acc.z -= world.lock()->gravity;
}

void Entity::decelerate(const Vec3& coefficient)
{
	acc -= vel * coefficient;
}

void Entity::decelerate(const double coefficient)
{
	decelerate(Vec3::All(coefficient));
}

// LivingEntity

LivingEntity::LivingEntity(const std::weak_ptr<World>& world, const Vec3& pos, const int64 maxHp) : Entity(world, pos), hp(maxHp), maxHp(maxHp)
{
}

bool LivingEntity::damage(const int64 amount, const String& reason)
{
	if (amount < 0) return true;
	hp -= Min(hp, amount);
	return hp > 0;
}

// World

World::World(const String& worldName) : gravity(1.0 / 64.0)
{
	const JSON world = JSON::Load(Resource(U"UnderTheFortress/worlds/{}.json"_fmt(worldName)), AllowExceptions::Yes);
	tileMap = TileMap{ Size{ world[U"size"][1].get<size_t>(), world[U"size"][0].get<size_t>() }, world[U"map"] };
}

void World::update(const UtFInput& input)
{
	if (input.down(KeyF3)) DEBUG_PRINT(U"World::update()");

	objects.sort_by([](const auto& a, const auto& b) { return a->getUpdatePriority() < b->getUpdatePriority(); });
	objects.each([&input](const auto& object) { object->update(input); });
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
		for (; objectIter != sortedObjects.end() && (*objectIter)->getDrawZ() >= tileMap.layerHeight * layer; ++objectIter)
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
