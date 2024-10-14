﻿# include "Game.hpp"

# include "Debug.ipp"

using namespace std;

// Tile

TileData::~TileData() = default;

void Formatter(FormatData& formatData, const TileData& value)
{
	formatData.string += U"<TileData>";
}

MapTile::MapTile(const uint8 layer, const Vec2& tilePos, const TileType type)
	: layer(layer), tilePos(tilePos), type(type), isSolid(false), opacity(1.0), data(nullptr)
{
	switch (type)
	{
	case TileType::Error:
		break;
	case TileType::Air:
		break;
	case TileType::EnergyBlock:
		isSolid = true;
		opacity = 0.5;
		break;
	case TileType::GlassBlock:
		isSolid = true;
		opacity = 0.5;
		break;
	case TileType::StoneBlock:
		isSolid = true;
		break;
	case TileType::WoodWall:
		isSolid = true;
		break;
	}
}

void Formatter(FormatData& formatData, const MapTile& value)
{
	formatData.string +=
		U"<MapTile layer={} tilePos={} typeId={} isSolid={} opacity={} data={}>"_fmt(
			value.layer, value.tilePos, FromEnum(value.type), value.isSolid, value.opacity,
			value.data ? Format(*value.data) : U"null"
		);
}

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

void World::initCell(const Point pos)
{
	for (const auto layer : step(MapTile::LayerCount))
	{
		tileMap[pos][layer] = MapTile{ layer, pos };
	}
}

void World::load(const String& worldName)
{
	const JSON world = JSON::Load(Resource(U"UnderTheFortress/worlds/{}.json"_fmt(worldName)), AllowExceptions::Yes);
	const JSON worldMap = world[U"map"];
	tileMap.resize(world[U"size"][1].get<size_t>(), world[U"size"][0].get<size_t>());

	for (const auto y : step(tileMap.height()))
	{
		for (const auto x : step(tileMap.width()))
		{
			Array<TileType> tileTypes;
			if (y < worldMap.size() && x < worldMap[y].size())
			{
				const String cellStr = worldMap[y][x].getString();
				tileTypes = Array<char32>{ cellStr.begin(), cellStr.end() }.chunk(2).map(
					[](const Array<char32>& chunk)
					{
						return ToEnum<TileType>(ParseIntOpt<uint8>(
							String{ chunk.begin(), chunk.end() }, Arg::radix = 16
						).value_or(0));
					});
			}
	
			for (const auto layer : step(MapTile::LayerCount))
			{
				tileMap[y][x][layer] = MapTile{ layer, { x, y }, tileTypes.fetch(layer, TileType::Air) };
			}
		}
	}
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

	for (const auto layer : step_backward(MapTile::LayerCount))
	{
		// entity
		for (; objectIter != sortedObjects.end() && (*objectIter)->getDrawZ() >= static_cast<int32>(layer); ++objectIter)
		{
			DEBUG_DUMP_F3((*objectIter)->getDrawZ());
			(*objectIter)->draw(accumulatorStep);
		}

		// tile
		DEBUG_DUMP_F3(layer);
		for (const auto y : step(tileMap.height()))
		{
			for (const auto x : step(tileMap.width()))
			{
				const auto& tile = tileMap[y][x][layer];
				if (tile.type == TileType::Air) continue;
				RectF{ tile.tilePos * 1.0, 1.0 }
					.stretched(-1)
					.draw(tile.type == TileType::EnergyBlock ? Palette::Yellow : tile.type == TileType::GlassBlock ? Palette::Skyblue : tile.type == TileType::StoneBlock ? Palette::Gray : tile.type == TileType::WoodWall ? Palette::Brown : Palette::Black);
			}
		}
	}

	// object
	for (; objectIter != sortedObjects.end(); ++objectIter)
	{
		DEBUG_DUMP_F3((*objectIter)->getDrawZ());
		(*objectIter)->draw(accumulatorStep);
	}
}
