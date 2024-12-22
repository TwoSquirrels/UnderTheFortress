# pragma once

# include <Siv3D.hpp>

// tile

enum class TileType : uint8
{
	Error = 0,
	Air = 1,
	Energy = 2,
	Glass = 3,
	Stone = 4,
	WoodWall = 5,
};

StringView ToId(TileType type);

Optional<TileType> ToTileType(StringView id);

void Formatter(FormatData& formatData, TileType value);

class TileData	// NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	virtual ~TileData() = default;

	[[nodiscard]] virtual TileType type() const = 0;

	/// @brief Update the tile data.
	/// @return whether to propagate updates around (true ONLY IF the update was successful and should be propagated)
	virtual bool update();

	friend void Formatter(FormatData& formatData, const TileData& value);
};

class Tile final
{
public:
	static constexpr uint8 LayerCount = 5;
	static constexpr uint32 TileImageSize = 96;
	static constexpr double AnimationFps = 6.0;

	TileType type;
	bool isSolid;
	std::unique_ptr<TileData> data; // NOTE: Avoid frequent access to data in most tiles.

	/// @param layer 0: underground, 1: ground, 2: wall, 3: wall, 4: roof
	/// @param tilePos tile position
	/// @param type tile type
	explicit Tile(uint8 layer = 1, const Point& tilePos = Point::Zero(), TileType type = TileType::Air);

	[[nodiscard]] uint8 getLayer() const;

	[[nodiscard]] Point getTilePos() const;

	[[nodiscard]] uint8& getAroundBits()&;

	/// @brief Get the same flag.
	///	@param direction { 0: R, 1: DR, 2: D, 3: DL, 4: L, 5: UL, 6: U, 7: UR } (mod 8)
	/// @return whether the tile is the same as the tile in the specified direction
	[[nodiscard]] bool checkSame(int8 direction) const;

	/// @brief Update the same flag.
	/// @param direction { 0: R, 1: DR, 2: D, 3: DL, 4: L, 5: UL, 6: U, 7: UR } (mod 8)
	/// @param newSame new same flag
	/// @return whether the same flag was updated
	bool updateSame(int8 direction, bool newSame);

	void draw() const;

	friend void Formatter(FormatData& formatData, const Tile& value);

private:
	uint8 m_layer;
	Point m_tilePos;
	uint8 m_aroundBits; // [ R, DR, D, DL, L, UL, U, UR ]
};

// tile map

class TileMap
{
public:
	uint32 cellUpdateLimit;
	double layerHeight;

	explicit TileMap(Size size = { 0, 0 }, uint32 cellUpdateLimit = 256);

	explicit TileMap(Size size, const JSON& worldMap, uint32 cellUpdateLimit = 256);

	void update();

	void draw(uint8 layer) const;

private:
	Grid<std::array<Tile, Tile::LayerCount>> m_grid;
	Array<Point> m_updateQueue;

	bool autoTile(Tile& tile);
};
