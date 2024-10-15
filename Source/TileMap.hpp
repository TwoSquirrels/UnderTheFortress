# pragma once

# include <Siv3D.hpp>

// tile

enum class TileType : uint8
{
	Error = 0,
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

class Tile final
{
public:
	static constexpr uint8 LayerCount = 5;

	TileType type;
	bool isSolid;
	std::unique_ptr<TileData> data;

	/// @param layer 0: underground, 1: ground, 2: wall, 3: wall, 4: roof
	/// @param tilePos tile position
	/// @param type tile type
	explicit Tile(uint8 layer = 1, const Point& tilePos = Point::Zero(), TileType type = TileType::Air);

	[[nodiscard]] uint8 getLayer() const;

	[[nodiscard]] Point getTilePos() const;

	void draw() const; // TODO: where I should put this function?

	friend void Formatter(FormatData& formatData, const Tile& value);

private:
	uint8 m_layer;
	Point m_tilePos;
};

// tile map

class TileMap : public Grid<std::array<Tile, Tile::LayerCount>>
{
public:
	explicit TileMap(Size size = { 0, 0 });

	explicit TileMap(Size size, const JSON& worldMap);

	void resetCell(Point pos);

	void draw(size_t layer) const;
};
