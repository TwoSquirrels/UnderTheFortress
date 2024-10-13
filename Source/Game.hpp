# pragma once

# include <Siv3D.hpp>

// tile

enum class TileType : uint8
{
	Air,
	EnergyBlock,
	GlassBlock,
	StoneBlock,
	WoodWall,
};

class TileData
{
public:
	virtual ~TileData();

	friend void Formatter(FormatData& formatData, const TileData& value);
};

class MapTile
{
public:
	static constexpr uint8 LayerCount = 5;

	uint8 layer;
	Vec2 tilePos;
	TileType type;
	bool isSolid;
	double opacity;
	std::unique_ptr<TileData> data;

	explicit MapTile(uint8 layer = 1, const Vec2& tilePos = Vec2::Zero(), TileType type = TileType::Air);

	friend void Formatter(FormatData& formatData, const MapTile& value);
};

// object

class UtFObject
{
public:
	virtual ~UtFObject();

	[[nodiscard]] virtual String getName() const;

	[[nodiscard]] virtual int32 getUpdatePriority() const; // lower value is higher priority

	virtual void update();

	[[nodiscard]] virtual int32 getDrawZ() const; // lower value is drawn first

	virtual void draw() const;

	friend void Formatter(FormatData& formatData, const UtFObject& value);
};

class Entity : public UtFObject
{
public:
	Vec3 pos; // position
	Vec3 vel; // velocity
	Vec3 acc; // acceleration

	explicit Entity(const Vec3& pos = Vec3::Zero());

	[[nodiscard]] String getName() const override;

	void move(bool resetAcc = true);
};

class LivingEntity : public Entity
{
public:
	int64 hp;
	int64 maxHp;

	explicit LivingEntity(const Vec3& pos = Vec3::Zero(), int64 maxHp = 1);

	[[nodiscard]] String getName() const override;

	bool damage(int64 amount, const String& reason = U"unknown");
};

// world

class World final
{
public:
	Grid<std::array<MapTile, MapTile::LayerCount>> tileMap;
	Array<std::shared_ptr<UtFObject>> objects;

	void initCell(Point pos);

	void update();
	void draw() const;
};
