#pragma once

#include <alligator/util/math.h>

#include <allegro5/allegro_color.h>
#include <memory>
#include <vector>

enum Stat {
	HP, ATK, DEF, AGI
};

struct Stats
{
	Stats(float hp_, float atk_, float def_, float agi_) : hp(hp_), atk(atk_), def(def_), agi(agi_) {}
	float hp, atk, def, agi;
};

class Entity
{
public:
	typedef std::shared_ptr<Entity> SharedPtr;

	Entity( Stats stats, Stats modstats )
		: m_currentHP(stats.hp), m_stats(stats), m_modstats(modstats)
	{

	}

	float lifePercentage()
	{
		return m_currentHP / m_stats.hp;
	}

	float getStat( Stat stat )
	{
		switch(stat)
		{
		case HP: return m_currentHP;
		case ATK: return m_stats.atk * m_modstats.atk; break;
		case DEF: return m_stats.def * m_modstats.def; break;
		case AGI: return m_stats.agi * m_modstats.agi; break;
		}
	}

	void decreaseHP( float qtt )
	{
		std::cout << "qtt" << qtt << std::endl;
		m_currentHP -= qtt;
	}

	Vec2i position;


protected:

	float m_currentHP;
	Stats m_stats;
	Stats m_modstats;

};

enum ShapeType : int {
	Water = 0,
	Gaia,
	Fire
};


enum PickType : int
{
	PickSingle = 0,
	PickAll,
	Pick2x2Block,
	PickSelf
};

class Ability;

class AbilityApplicator
{
public:
	virtual ~AbilityApplicator() {}
	virtual void operator()( Ability* ability, Entity* source, std::vector<Entity*> targets ) = 0 ;
};

class Ability
{
public:
	typedef std::shared_ptr<Ability> SharedPtr;

	Ability( ShapeType stype, std::string txt, PickType pt = PickSingle, AbilityApplicator* applic = nullptr, float effectiveness = 1.f, int cooldown = 1.f )
		: m_shapeType(stype),
		  m_text(txt),
		  m_pickType(pt),
		  applicator(applic),
		  m_effectiveness(effectiveness),
		  cooldown(cooldown),
		  currentCooldown(0)
	{}

	void apply( Entity* source, std::vector<Entity*> targets )
	{
		if( applicator != nullptr )
		{
			(*applicator)(this, source, targets);
			currentCooldown = cooldown + 1;
		}
	}

	bool canUse()
	{
		return currentCooldown <= 0;
	}

	const char* text()
	{
		return m_text.c_str();
	}

	float effectiveness()
	{
		return m_effectiveness;
	}

	ShapeType shapeType()
	{
		return m_shapeType;
	}

	PickType pickType()
	{
		return m_pickType;
	}

	int currentCooldown;

private:

	ShapeType m_shapeType;
	std::string m_text;
	int cooldown;
	float m_effectiveness;
	PickType m_pickType;

	AbilityApplicator* applicator;


};

#define DMG_FACTOR 0.5
#define ATK_FACTOR 0.5
#define DEF_FACTOR 0.5

class DealDamageAbilityApplicator : public AbilityApplicator
{
public:
	void operator()( Ability* ability, Entity* source, std::vector<Entity*> targets ) override
	{
		float source_atk = ATK_FACTOR * source->getStat(ATK);
		std::vector<Entity*> already_targeted;
		for( Entity* target : targets )
		{
			if( target != nullptr )
			{
				already_targeted.push_back(target);
				float target_def = DEF_FACTOR * target->getStat(DEF);
				float dmg = std::max(0.f, source_atk * ability->effectiveness() - target_def) * DMG_FACTOR;
				std::cout << "effec: " << ability->effectiveness() << ", srcatk: " << source_atk << ", tgtdef: " << target_def << ", dmg: " << dmg << std::endl;
				target->decreaseHP(std::max(1.f, dmg));
			}
		}
	}
};


enum StatusEffectType : int {
	Stun,
};

struct StatusEffect {
	StatusEffectType type;
	int turnsLeft;
};


class AbilityApplicators
{
public:
	DealDamageAbilityApplicator damageAP;
};


class Enemy : public Entity
{
public:

	AbilityApplicators aps;

	typedef std::shared_ptr<Enemy> SharedPtr;

	Enemy( Stats stats, float attack_effectiveness = 1.f ) : Entity(stats, Stats(1, 1, 1, 1))
	{
		abilities().push_back(Ability::SharedPtr(new Ability(Water, "", PickSingle, &(aps.damageAP), attack_effectiveness)));
	}

	std::vector<StatusEffect> status;

	std::vector<Ability::SharedPtr>& abilities()
	{
		return m_abilities;
	}

private:
	std::vector<Ability::SharedPtr> m_abilities;

};

class AbilitiesStore
{
public:
	AbilitiesStore()
	{

	}
};

class EntityFactory
{
public:
	static Enemy::SharedPtr makeDummyEnemy()
	{
		Enemy::SharedPtr enemy(new Enemy(Stats(10, 5, 5, 5)));
		return enemy;
	}
};

class Player : public Entity
{
private:
	AbilityApplicators aps;
public:
	typedef std::shared_ptr<Player> SharedPtr;

	Player() : Entity(Stats(10, 5, 5, 5), Stats(1,1,1,1))
	{
		abilities().push_back(Ability::SharedPtr(new Ability(Water, "Headbutt", PickSingle, &(aps.damageAP), 1.f)));

		// WATER
		abilities().push_back(Ability::SharedPtr(new Ability(Water, "PressureShot", PickSingle, &(aps.damageAP), 2.f, 3)));
		abilities().push_back(Ability::SharedPtr(new Ability(Water, "Tsunami", PickAll, &(aps.damageAP), 3.f)));
		abilities().push_back(Ability::SharedPtr(new Ability(Water, "Demoleculize", PickSelf)));

		// GAIA
		abilities().push_back(Ability::SharedPtr(new Ability(Gaia, "Regen", PickSelf)));
		abilities().push_back(Ability::SharedPtr(new Ability(Gaia, "Spike Shield", PickSelf)));
		abilities().push_back(Ability::SharedPtr(new Ability(Gaia, "Earthquake", PickAll, &(aps.damageAP), 3.f)));

		// FIRE
		abilities().push_back(Ability::SharedPtr(new Ability(Fire, "Blaze", Pick2x2Block, &(aps.damageAP), 2)));
		abilities().push_back(Ability::SharedPtr(new Ability(Fire, "Fiery Beast", PickAll)));
		abilities().push_back(Ability::SharedPtr(new Ability(Fire, "Magma", PickAll, &(aps.damageAP), 3)));
	}

	void turn()
	{
		for( Ability::SharedPtr ability : abilities() )
		{
			if( ability->currentCooldown > 0 )
			{
				ability->currentCooldown--;
			}
			else
			{
				ability->currentCooldown = 0;
			}
		}
	}

	void setShape( ShapeType st )
	{
		currentShape = st;
		switch(currentShape)
		{
		case Water:
			m_modstats.agi = 1.5f;
			m_modstats.atk = 1.f;
			m_modstats.def = 0.5f;
			break;
		case Fire:
			m_modstats.agi = 0.5f;
			m_modstats.atk = 1.5f;
			m_modstats.def = 1.0f;
			break;
		case Gaia:
			m_modstats.agi = 1.0f;
			m_modstats.atk = 0.5f;
			m_modstats.def = 1.0f;
			break;
		}
	}

	ALLEGRO_COLOR shapeColor()
	{
		switch( currentShape )
		{
		case Gaia: return al_map_rgb(55, 148, 110);
		case Water: return al_map_rgb(91, 110, 225);
		case Fire: return al_map_rgb(172, 50, 50);
		default: return al_map_rgb(255, 0, 255);
		}
	}


	ShapeType shapeType()
	{
		return currentShape;
	}

	std::vector<Ability::SharedPtr>& abilities()
	{
		return m_abilities;
	}

private:
	std::vector<Ability::SharedPtr> m_abilities;
	ShapeType currentShape = Water;

};
