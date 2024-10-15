# include "TileMap.hpp"

# include "Debug.ipp"

// tile

TileData::~TileData() = default;

void Formatter(FormatData& formatData, const TileData& value)
{
	formatData.string += U"<TileData>";
}

Tile::Tile(const uint8 layer, const Point& tilePos, const TileType type)
	: type(type), isSolid(false), data(nullptr), m_layer(layer), m_tilePos(tilePos)
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

void Tile::draw() const
{
	if (type == TileType::Air) return;
	RectF{ getTilePos(), 1.0 }
		.stretched(-1)
		.draw(type == TileType::EnergyBlock ? Palette::Yellow : type == TileType::GlassBlock ? Palette::Skyblue : type == TileType::StoneBlock ? Palette::Gray : type == TileType::WoodWall ? Palette::Brown : Palette::Black);
}

void Formatter(FormatData& formatData, const Tile& value)
{
	formatData.string +=
		U"<Tile layer={} tilePos={} typeId={} isSolid={} data={}>"_fmt(
			value.m_layer, value.m_tilePos, FromEnum(value.type), value.isSolid,
			value.data ? Format(*value.data) : U"null"
		);
}

// tile map

TileMap::TileMap(const Size size) : Grid(size)
{
}

TileMap::TileMap(const Size size, const JSON& worldMap) : Grid(size)
{
	for (const auto y : step(height()))
	{
		for (const auto x : step(width()))
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
				(*this)[y][x][layer] = Tile{ layer, { x, y }, tileTypes.fetch(layer, TileType::Air) };
			}
		}
	}
}

void TileMap::resetCell(const Point pos)
{
	for (const auto layer : step(Tile::LayerCount))
	{
		(*this)[pos][layer] = Tile{ layer, pos };
	}
}

void TileMap::draw(const size_t layer) const
{
	for (const auto& cell : *this)
	{
		cell[layer].draw();
	}
}
