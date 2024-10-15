# include "TileMap.hpp"

# include "Debug.ipp"

// TileType

void Formatter(FormatData& formatData, const TileType value)
{
	switch (value)
	{
	case TileType::Error:
		formatData.string += U"Error";
		break;
	case TileType::Air:
		formatData.string += U"Air";
		break;
	case TileType::EnergyBlock:
		formatData.string += U"EnergyBlock";
		break;
	case TileType::GlassBlock:
		formatData.string += U"GlassBlock";
		break;
	case TileType::StoneBlock:
		formatData.string += U"StoneBlock";
		break;
	case TileType::WoodWall:
		formatData.string += U"WoodWall";
		break;
	}
}

// TileData

bool TileData::update()
{
	return false;
}

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
	RectF{ getTilePos(), 1.0 }
		.stretched(-1)
		.draw(type == TileType::EnergyBlock ? Palette::Yellow : type == TileType::GlassBlock ? Palette::Skyblue : type == TileType::StoneBlock ? Palette::Gray : type == TileType::WoodWall ? Palette::Brown : Palette::Black);
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

TileMap::TileMap(const Size size) : m_grid(size)
{
}

TileMap::TileMap(const Size size, const JSON& worldMap) : m_grid(size)
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
			}
		}
	}
}

void TileMap::resetCell(const Point pos)
{
	for (const auto layer : step(Tile::LayerCount))
	{
		m_grid[pos][layer] = Tile{ layer, pos };
	}
}

void TileMap::update()
{
	const auto tiles = m_scheduledUpdateTiles;
	m_scheduledUpdateTiles.clear();

	for (const Tile* tile : m_scheduledUpdateTiles)
	{
		if (not tile->data) continue;
		if (tile->data->update())
		{
			if (m_grid.inBounds(tile->getTilePos() + Point{ 1, 0 }))
			{
				m_scheduledUpdateTiles.insert(&m_grid[tile->getTilePos() + Point{ 1, 0 }][tile->getLayer()]);
			}
			if (m_grid.inBounds(tile->getTilePos() + Point{ 0, 1 }))
			{
				m_scheduledUpdateTiles.insert(&m_grid[tile->getTilePos() + Point{ 0, 1 }][tile->getLayer()]);
			}
			if (m_grid.inBounds(tile->getTilePos() + Point{ -1, 0 }))
			{
				m_scheduledUpdateTiles.insert(&m_grid[tile->getTilePos() + Point{ -1, 0 }][tile->getLayer()]);
			}
			if (m_grid.inBounds(tile->getTilePos() + Point{ 0, -1 }))
			{
				m_scheduledUpdateTiles.insert(&m_grid[tile->getTilePos() + Point{ 0, -1 }][tile->getLayer()]);
			}
			if (tile->getLayer() + 1 < m_grid[tile->getTilePos()].size())
			{
				m_scheduledUpdateTiles.insert(&m_grid[tile->getTilePos()][tile->getLayer() + 1]);
			}
			if (tile->getLayer() > 0)
			{
				m_scheduledUpdateTiles.insert(&m_grid[tile->getTilePos()][tile->getLayer() - 1]);
			}
		}
	}
}

void TileMap::draw(const size_t layer) const
{
	for (const auto& cell : m_grid)
	{
		cell[layer].draw();
	}
}
