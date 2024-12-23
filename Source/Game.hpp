﻿# pragma once

# include "TileMap.hpp"
# include "UtFInput.hpp"

# include <Siv3D.hpp>

// object

enum class ObjectType : uint8
{
	Error = 0,
	Player = 1,
};

StringView ToId(ObjectType type);

Optional<ObjectType> ToObjectType(StringView id);

void Formatter(FormatData& formatData, ObjectType value);

class UtFObject	// NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	std::weak_ptr<class World> world;

	explicit UtFObject(const std::weak_ptr<World>& world = {});

	virtual ~UtFObject() = default;

	[[nodiscard]] virtual ObjectType type() const = 0;

	/// @remark lower value is higher priority
	[[nodiscard]] virtual double getUpdatePriority() const;

	virtual void update(const UtFInput& input);

	/// @remark lower value is drawn first
	[[nodiscard]] virtual double getDrawZ() const;

	virtual void draw(double accumulatorStep) const;

	friend void Formatter(FormatData& formatData, const UtFObject& value);
};

class Entity : public UtFObject
{
public:
	Vec3 pos; // position
	Vec3 vel; // velocity
	Vec3 acc; // acceleration

	explicit Entity(const std::weak_ptr<World>& world = {}, const Vec3& pos = Vec3::Zero());

	[[nodiscard]] double getDrawZ() const override;

protected:
	/// @brief Move the entity.
	/// @param resetAcc whether to reset acceleration after moving
	void move(bool resetAcc = true);

	/// @brief Apply gravity to the entity.
	void gravitate();

	/// @brief Decelerate the entity.
	/// @param coefficient the deceleration coefficient
	void decelerate(const Vec3& coefficient);

	/// @brief Decelerate the entity.
	/// @param coefficient the deceleration coefficient
	void decelerate(double coefficient);
};

class LivingEntity : public Entity
{
public:
	int64 hp;
	int64 maxHp;

	explicit LivingEntity(const std::weak_ptr<World>& world = {}, const Vec3& pos = Vec3::Zero(), int64 maxHp = 1);

	bool damage(int64 amount, const String& reason = U"unknown");
};

// world

class World final
{
public:
	TileMap tileMap;
	Array<std::shared_ptr<UtFObject>> objects;

	double gravity;

	explicit World(const String& worldName);

	void update(const UtFInput& input);
	void draw(double accumulatorStep) const;
};
