# include "TileMap.hpp"

# include "Debug.ipp"

using namespace std;

// TileType

StringView ToId(const TileType type)
{
	static array<String, 0x100> idTable =
	{
		U"error", U"air", U"energy", U"glass", U"stone", U"wood_wall"
	};
	return idTable[FromEnum(type)];
}

Optional<TileType> ToTileType(const StringView id)
{
	static HashTable<String, TileType> typeTable;
	if (typeTable.empty())
	{
		for (const uint8 i : step(0x100))
		{
			const auto type = ToEnum<TileType>(i);
			typeTable[ToId(type)] = type;
		}
	}
	return typeTable.contains(id) ? Optional{ typeTable[id] } : none;
}

void Formatter(FormatData& formatData, const TileType value)
{
	formatData.string += ToId(value);
}

// TileData

bool TileData::update() { return false; }

void Formatter(FormatData& formatData, const TileData& value)
{
	formatData.string += U"<TileData({})>"_fmt(Format(value.type()));
}

// Tile

Tile::Tile(const uint8 layer, const Point& tilePos, const TileType type)
	: type(type), isSolid(false), data(nullptr), m_layer(layer), m_tilePos(tilePos), m_aroundBits(0)
{
}

uint8 Tile::getLayer() const
{
	return m_layer;
}

Point Tile::getTilePos() const
{
	return m_tilePos;
}

uint8& Tile::getAroundBits()&
{
	return m_aroundBits;
}

bool Tile::checkSame(int8 direction) const
{
	direction %= 8;
	direction += 8;
	direction %= 8;
	return m_aroundBits >> direction & 1;
}

bool Tile::updateSame(int8 direction, const bool newSame)
{
	direction %= 8;
	direction += 8;
	direction %= 8;
	const bool isUpdated = (m_aroundBits >> direction & 1) != newSame;
	if (isUpdated) m_aroundBits ^= 1 << direction;
	return isUpdated;
}

void Tile::draw() const
{
	if (type == TileType::Air) return;

	constexpr int32 S = TileImageSize;

	// TODO: preload textures
	if (const TextureAsset texture{ U"tiles/{}.png"_fmt(ToId(type)) })
	{
		TextureRegion region;

		if (texture.size() == Size{ S, S })
		{
			region = texture;
		}
		else if (texture.size() == Size{ S * 4, S * 4 })
		{
			if (m_aroundBits == 0b11111111) region = texture(0, 0, S);
			else if ((m_aroundBits & 0b01011111) == 0b00011111) region = texture(S * 0, S * 1, S);
			else if ((m_aroundBits & 0b01111101) == 0b01111100) region = texture(S * 1, S * 1, S);
			else if ((m_aroundBits & 0b11110101) == 0b11110001) region = texture(S * 2, S * 1, S);
			else if ((m_aroundBits & 0b11010111) == 0b11000111) region = texture(S * 3, S * 1, S);
			else if ((m_aroundBits & 0b01011101) == 0b00011100) region = texture(S * 0, S * 2, S);
			else if ((m_aroundBits & 0b01110101) == 0b01110000) region = texture(S * 1, S * 2, S);
			else if ((m_aroundBits & 0b11010101) == 0b11000001) region = texture(S * 2, S * 2, S);
			else if ((m_aroundBits & 0b01010111) == 0b00000111) region = texture(S * 3, S * 2, S);
			else if (m_aroundBits == 0b01111111) region = texture(S * 0, S * 3, S);
			else if (m_aroundBits == 0b11111101) region = texture(S * 1, S * 3, S);
			else if (m_aroundBits == 0b11110111) region = texture(S * 2, S * 3, S);
			else if (m_aroundBits == 0b11011111) region = texture(S * 3, S * 3, S);
		}

		if (region.texture)
		{
			region.resized(1.0).draw(getTilePos());
			return;
		}
	}

	// fallback
	RectF{ getTilePos(), 1.0 }.drawFrame(0.125, 0.0, Palette::Magenta);
}

void Formatter(FormatData& formatData, const Tile& value)
{
	formatData.string +=
		U"<Tile layer={} tilePos={} type={} solid={} around=0b{:0>8b} data={}>"_fmt(
			value.m_layer, value.m_tilePos, Format(value.type), value.isSolid, value.m_aroundBits,
			value.data ? Format(*value.data) : U"null"
		);
}

// TileMap

TileMap::TileMap(const Size size, const uint32 cellUpdateLimit)
	: cellUpdateLimit(cellUpdateLimit), m_grid(size)
{
	for (const auto y : step(size.y))
	{
		for (const auto x : step(size.x))
		{
			for (const auto layer : step(Tile::LayerCount))
			{
				m_grid[y][x][layer] = Tile{ layer, { x, y } };
			}
		}
	}

	for (auto& cell : m_grid)
	{
		for (auto& tile : cell) autoTile(tile);
	}
}

TileMap::TileMap(const Size size, const JSON& worldMap, const uint32 cellUpdateLimit)
	: cellUpdateLimit(cellUpdateLimit), m_grid(size)
{
	for (const auto y : step(size.y))
	{
		for (const auto x : step(size.x))
		{
			Array<TileType> tileTypes;

			// if cell exists, parse it and fill tileTypes
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

			for (const auto layer : step(Tile::LayerCount))
			{
				m_grid[y][x][layer] = Tile{ layer, { x, y }, tileTypes.fetch(layer, TileType::Air) };

				if (m_grid[y][x][layer].type == TileType::Error)
				{
					DEBUG_PRINT(U"the tile (x={} y={} layer={}) could not be parsed!", x, y, layer);
				}
			}
		}
	}

	for (auto& cell : m_grid)
	{
		for (auto& tile : cell) autoTile(tile);
	}
}

void TileMap::update()
{
	// BFS-like update
	for ([[maybe_unused]] const auto i : step(cellUpdateLimit))
	{
		if (m_updateQueue.empty()) break;
		auto& cell = m_grid[m_updateQueue.front()];
		m_updateQueue.pop_front();

		bool propagate = false;
		for (auto& tile : cell)
		{
			propagate |= tile.data && tile.data->update();
			propagate |= autoTile(tile);
		}
		if (not propagate) continue;

		for (const auto delta : { Point{ 1, 0 }, Point{ 0, 1 }, Point{ -1, 0 }, Point{ 0, -1 } })
		{
			const auto nextPos = cell[0].getTilePos() + delta;
			if (not m_grid.inBounds(nextPos)) continue;
			m_updateQueue << nextPos;
		}
	}
}

void TileMap::draw(const uint8 layer) const
{
	for (const auto& cell : m_grid)
	{
		cell[layer].draw();
	}
}

bool TileMap::autoTile(Tile& tile)
{
	bool propagate = false;
	Array<Point>
	{
		{ 1, 0 }, { 1,1 }, { 0,1 }, { -1, 1 },
		{ -1,0 }, { -1,-1 }, { 0,-1 }, { 1,-1 },
	}
	.each_index(
		[&](const int8 direction, const Point delta)
		{
			const auto nextPos = tile.getTilePos() + delta;
			const auto nextType = m_grid.inBounds(nextPos) ? m_grid[nextPos][tile.getLayer()].type : TileType::Air;
			propagate |= tile.updateSame(direction, nextType == tile.type);
		}
	);
	return propagate;
}
