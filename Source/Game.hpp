# pragma once

# include <Siv3D.hpp>

class UtFObject
{
public:
	virtual ~UtFObject();
	[[nodiscard]] virtual int32 getUpdatePriority() const; // lower value is higher priority
	virtual void update();
	[[nodiscard]] virtual int32 getDrawZ() const; // lower value is drawn first
	virtual void draw() const;
};

class Entity : public UtFObject
{
public:
	Vec3 pos, vel, acc;

	explicit Entity(const Vec3& pos = Vec3::Zero());

	void move(bool resetAcc = true);
};

class LivingEntity : public Entity
{
public:
	int64 hp, maxHp;

	explicit LivingEntity(const Vec3& pos = Vec3::Zero(), int64 maxHp = 1);

	bool damage(int64 amount, const String& reason = U"unknown");
};

class World final
{
public:
	Array<std::shared_ptr<UtFObject>> entities;

	void update();
	void draw() const;
};
