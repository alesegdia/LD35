#pragma once

#include <alligator/util/math.h>

#include <allegro5/allegro_color.h>
#include <memory>
#include <vector>
#include <stack>

enum Stat {
	HP, ATK, DEF, AGI, NONE
};

struct Stats
{
	Stats(float hp_, float atk_, float def_, float agi_) : hp(hp_), atk(atk_), def(def_), agi(agi_) {}
	float hp, atk, def, agi;
};

enum StatusEffectType : int {
	Stun,
	Healing,
	Burnt,
	Demolec,
	Shell,
	Fist,
	None
};

struct StatusEffect {

	typedef std::shared_ptr<StatusEffect> SharedPtr;

	StatusEffectType type;
	int turnsLeft;
	float value = 1.f;

};

class Entity
{
public:
	typedef std::shared_ptr<Entity> SharedPtr;

	Entity( Stats stats, Stats shape_mod_stats )
		: m_currentHP(stats.hp), m_stats(stats), m_shapeModStats(shape_mod_stats), m_statusModStats(0, 0, 0, 0)
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
		case HP: return m_stats.hp;
		case ATK: return m_stats.atk * m_shapeModStats.atk + m_stats.atk * m_statusModStats.atk; break;
		case DEF: return m_stats.def * m_shapeModStats.def + m_stats.def * m_statusModStats.def; break;
		case AGI: return m_stats.agi * m_shapeModStats.agi + m_stats.agi * m_statusModStats.agi; break;
		}
		return 0;
	}

	void decreaseHP( float qtt )
	{
		m_currentHP -= qtt;
		m_currentHP = std::max(0.f, m_currentHP);
	}

	void healHP( float qtt )
	{
		m_currentHP += qtt;
		if( m_currentHP > m_stats.hp )
		{
			m_currentHP = m_stats.hp;
		}
	}

	Vec2i position;


	void applyStatusEffect( StatusEffect::SharedPtr seff, std::stack<std::string>& notifstack )
	{

		switch( seff->type )
		{
		case Healing:
			healHP(seff->value);
			notifstack.push("Yo healin dawg!");
			break;
		case Burnt:
			m_currentHP -= seff->value;
			notifstack.push("Enemee'z' berning!");
			break;
		case Demolec:
			m_statusModStats.agi = 2;
			notifstack.push("You feel light!");
			break;
		case Shell:
			m_statusModStats.def = 2;
			notifstack.push("Rocking this!");
			break;
		case Fist:
			m_statusModStats.atk = 2;
			notifstack.push("Rolling that!");
			break;
		case Stun:
			m_stunned = true;
			notifstack.push("Stunned yo!");
			break;
		}
	}

	void handleStatusRemoval(StatusEffect::SharedPtr se)
	{
		switch(se->type)
		{
		case Demolec:
			m_statusModStats.agi = 0;
			break;
		case Shell:
			m_statusModStats.def = 0;
			break;
		case Fist:
			m_statusModStats.atk = 0;
			break;
		case Stun:
			m_stunned = false;
			break;
		}
	}

	void stepStatusEffects(std::stack<std::string>& notifstack)
	{
		std::vector<StatusEffect::SharedPtr> to_remove;
		for( StatusEffect::SharedPtr se : status )
		{
			if( se->turnsLeft > 0 )
			{
				applyStatusEffect(se, notifstack);
				se->turnsLeft--;
			}
			else
			{
				handleStatusRemoval(se);
				to_remove.push_back(se);
			}
		}
		for( StatusEffect::SharedPtr se : to_remove )
		{
			status.erase(std::remove(status.begin(), status.end(), se), status.end());
		}
	}

	float currentHP()
	{
		return m_currentHP;
	}

	bool isStunned()
	{
		return m_stunned;
	}

	std::vector<StatusEffect::SharedPtr> status;

protected:

	float m_currentHP;
	Stats m_stats;
	Stats m_shapeModStats;
	Stats m_statusModStats;
	bool m_stunned = false;

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

	void setModel( StatusEffect* model )
	{
		m_model = model;
	}

	void operator()( Ability* ability, Entity* source, std::vector<Entity*> targets ) override
	{
		float source_atk = ATK_FACTOR * source->getStat(ATK);
		for( Entity* target : targets )
		{
			if( target != nullptr )
			{
				float target_def = DEF_FACTOR * target->getStat(DEF);
				float dmg = std::max(0.f, source_atk * ability->effectiveness() - target_def) * DMG_FACTOR;
				std::cout << "effec: " << ability->effectiveness() << ", srcatk: " << source_atk << ", tgtdef: " << target_def << ", dmg: " << dmg << std::endl;
				target->decreaseHP(std::max(0.f, dmg));
				if( m_model != nullptr )
				{
					StatusEffect::SharedPtr se(new StatusEffect);
					se->value = source->getStat(ATK) / 2.f;
					se->turnsLeft = m_model->turnsLeft;
					se->type = m_model->type;
					target->status.push_back(se);
				}
			}
		}
	}

private:
	StatusEffect* m_model = nullptr;

};


class OnlyStatusEffectAbilityApplicator : public AbilityApplicator
{
public:
	void setup( bool self, StatusEffect* model, Stat influence_stat = NONE, float factor = 1.f )
	{
		m_self = self;
		m_model = model;
		m_influenceStat = influence_stat;
		m_factor = factor;
	}

	void operator()( Ability* ability, Entity* source, std::vector<Entity*> targets ) override
	{
		if( m_self )
		{
			source->status.push_back(makeFromModel(source));
		}
		else
		{
			for( Entity* target : targets )
			{
				if( target != nullptr )
				{
					target->status.push_back(makeFromModel(source));
				}
			}

		}
		std::cout << source << std::endl;
	}

private:

	StatusEffect::SharedPtr makeFromModel(Entity* source)
	{
		StatusEffect::SharedPtr se(new StatusEffect);
		se->turnsLeft = m_model->turnsLeft;
		se->type = m_model->type;
		se->value = source->getStat(m_influenceStat) * m_factor;
		return se;
	}

	bool m_self;
	StatusEffect* m_model;
	Stat m_influenceStat;
	float m_factor;

};

class HealAbilityApplicator : public AbilityApplicator
{
public:
	void operator()( Ability* ability, Entity* source, std::vector<Entity*> targets )
	{
		std::cout << "heal!" << std::endl;
		StatusEffect::SharedPtr se(new StatusEffect);
		se->turnsLeft = 3;
		se->type = Healing;
		source->status.push_back(se);
		std::cout << source << std::endl;
		//e->healHP( source->getStat(HP) * 0.10 );
	}
};



class DemolecAbilityApplicator : public AbilityApplicator
{
public:
	void operator()( Ability* ability, Entity* source, std::vector<Entity*> targets )
	{
		std::cout << "demolec!" << std::endl;
		StatusEffect::SharedPtr se(new StatusEffect);
		se->turnsLeft = 5;
		se->type = Demolec;
		source->status.push_back(se);
	}
};




class AbilityApplicators
{
public:
	DealDamageAbilityApplicator damageAP;
	//HealAbilityApplicator healAP;
	DemolecAbilityApplicator demolecAP;

	StatusEffect burning;
	OnlyStatusEffectAbilityApplicator burnAP;

	StatusEffect healing;
	OnlyStatusEffectAbilityApplicator healAP;

	StatusEffect shell;
	OnlyStatusEffectAbilityApplicator shellAP;

	StatusEffect fist;
	OnlyStatusEffectAbilityApplicator fistAP;

	StatusEffect stun;
	DealDamageAbilityApplicator stunAttackAP;

	AbilityApplicators()
	{
		burning.turnsLeft = 3;
		burning.type = Burnt;
		burning.value = 0; // IGNORED BECAUSE IT'S A MODEL
		//burnAP.setModel(&burning);
		burnAP.setup(false, &burning, ATK, 0.5f);

		healing.turnsLeft = 4;
		healing.type = Healing;
		healing.value = 0 ;
		healAP.setup(true, &healing, HP, 0.20f);

		shell.turnsLeft = 4;
		shell.type = Shell;
		shell.value = 0;
		shellAP.setup(true, &shell);

		fist.turnsLeft = 4;
		fist.type = Fist;
		fist.value = 0;
		fistAP.setup(true, &fist);

		stun.turnsLeft = 2;
		stun.type = Stun;
		stun.value = 0;

		stunAttackAP.setModel(&stun);
	}

};


class Enemy : public Entity
{
public:

	AbilityApplicators aps;

	int type = 0;

	typedef std::shared_ptr<Enemy> SharedPtr;

	Enemy( Stats stats, int type = 0, float attack_effectiveness = 4.f ) : Entity(stats, Stats(1, 1, 1, 1))
	{
		this->type = type;
		abilities().push_back(Ability::SharedPtr(new Ability(Water, "", PickSingle, &(aps.damageAP), attack_effectiveness)));
	}


	std::vector<Ability::SharedPtr>& abilities()
	{
		return m_abilities;
	}

	int computeExp()
	{
		return m_stats.agi + m_stats.atk + m_stats.def + m_stats.hp / 2;
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
	static Enemy::SharedPtr makeDummyEnemy(int type)
	{
		Enemy::SharedPtr enemy(new Enemy(Stats(10, 5, 5, 5), type));
		return enemy;
	}

	static Enemy::SharedPtr makeEnemyStats( int type, int points )
	{
		Stats stats(points * 2, points, points, points);
		Enemy::SharedPtr enemy(new Enemy(stats, type));
		return enemy;
	}
};

struct LevelingNotification
{
	bool isPresent = false;
	std::string text1;
	std::string text2;
	std::string text3;
	std::string text4;
};

class Player : public Entity
{
private:
	AbilityApplicators aps;
public:
	typedef std::shared_ptr<Player> SharedPtr;

	Player() : Entity(Stats(50, 5, 5, 5), Stats(1,1,1,1))
	{
		abilities().push_back(Ability::SharedPtr(new Ability(Water, "Headbutt", PickSingle, &(aps.damageAP), 2.f, -2.f)));

		/*
		unlock(Gaia);
		unlock(Gaia);
		unlock(Gaia);

		unlock(Fire);
		unlock(Fire);
		unlock(Fire);

		unlock(Water);
		unlock(Water);
		unlock(Water);
		*/

		// WATER

		// GAIA

		// FIRE
	}

	void turn()
	{
		std::cout << "leagi: " << getStat(AGI) << std::endl;
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
			m_shapeModStats.agi = 1.5f;
			m_shapeModStats.atk = 1.f;
			m_shapeModStats.def = 0.5f;
			break;
		case Fire:
			m_shapeModStats.agi = 0.5f;
			m_shapeModStats.atk = 1.5f;
			m_shapeModStats.def = 1.0f;
			break;
		case Gaia:
			m_shapeModStats.agi = 1.0f;
			m_shapeModStats.atk = 0.5f;
			m_shapeModStats.def = 1.0f;
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

	void unlock(ShapeType st)
	{

		switch( st )
		{
		case Water:
			m_shapeLevel[0]++;
			switch(m_shapeLevel[0])
			{
			case 1:
				abilities().push_back(Ability::SharedPtr(new Ability(Water, "PressureShot", PickSingle, &(aps.stunAttackAP), 2.f, 3)));
				break;
			case 2:
				abilities().push_back(Ability::SharedPtr(new Ability(Water, "Tsunami", PickAll, &(aps.damageAP), 3.f)));
				break;
			case 3:
				abilities().push_back(Ability::SharedPtr(new Ability(Water, "Demoleculize", PickSelf, &(aps.demolecAP), 1.f, 6)));
				break;
			}
			break;
		case Gaia:




			m_shapeLevel[1]++;
			switch(m_shapeLevel[1])
			{
			case 1:
				abilities().push_back(Ability::SharedPtr(new Ability(Gaia, "Regen", PickSelf, &(aps.healAP), 1.f, 6)));
				break;
			case 2:
				abilities().push_back(Ability::SharedPtr(new Ability(Gaia, "Mega Shell", PickSelf, &(aps.shellAP), 1.f, 6.f)));
				break;
			case 3:
				abilities().push_back(Ability::SharedPtr(new Ability(Gaia, "Earthquake", PickAll, &(aps.damageAP), 3.f)));
				break;
			}
			break;
		case Fire:



			m_shapeLevel[2]++;
			switch(m_shapeLevel[2])
			{
			case 1:
				abilities().push_back(Ability::SharedPtr(new Ability(Fire, "Blaze", Pick2x2Block, &(aps.burnAP), 2)));
				break;
			case 2:
				abilities().push_back(Ability::SharedPtr(new Ability(Fire, "Flame Fists", PickSelf, &(aps.fistAP), 1, 6)));
				break;
			case 3:
				abilities().push_back(Ability::SharedPtr(new Ability(Fire, "Magma", PickAll, &(aps.damageAP), 3)));
				break;
			}
			break;
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

	static constexpr int LevelExp = 100;

	LevelingNotification levelingNotification;

	void addExp( int exp )
	{
		m_exp += exp;
		if( m_exp >= m_nextLevelNeededExp )
		{
			m_level++;
			levelingNotification.isPresent = true;
			levelingNotification.text1 = "Level up to " + std::to_string(m_level) + "!";
			m_exp -= m_nextLevelNeededExp;
			m_nextLevelNeededExp = (m_level + 1) * LevelExp;
			switch( currentShape )
			{
			case Water:
				levelingNotification.text4 = std::to_string((int)m_stats.agi) + " + " + std::to_string(3) + " agi!";
				m_stats.agi += 3;
				levelingNotification.text3 = std::to_string((int)m_stats.def) + " + " + std::to_string(1) + " def!";
				m_stats.def += 1;
				levelingNotification.text2 = std::to_string((int)m_stats.atk) + " + " + std::to_string(2) + " atk!";
				m_stats.atk += 2;
				break;
			case Gaia:
				levelingNotification.text4 = std::to_string((int)m_stats.agi) + " + " + std::to_string(3) + " agi!";
				m_stats.agi += 3;
				levelingNotification.text3 = std::to_string((int)m_stats.def) + " + " + std::to_string(4) + " def!";
				m_stats.def += 4;
				levelingNotification.text2 = std::to_string((int)m_stats.atk) + " + " + std::to_string(1) + " def!";
				m_stats.atk += 1;
				break;
			case Fire:
				levelingNotification.text4 = std::to_string((int)m_stats.agi) + " + " + std::to_string(2) + " agi!";
				m_stats.agi += 2;
				levelingNotification.text3 = std::to_string((int)m_stats.def) + " + " + std::to_string(1) + " def!";
				m_stats.def += 1;
				levelingNotification.text2 = std::to_string((int)m_stats.atk) + " + " + std::to_string(3) + " def!";
				m_stats.atk += 3;
				break;
			}
		}
	}

	float expPercentage()
	{
		return (float)m_exp / (float)m_nextLevelNeededExp;
	}

	int level()
	{
		return m_level;
	}

private:
	std::vector<Ability::SharedPtr> m_abilities;
	ShapeType currentShape = Water;

	int m_shapeLevel[3] = {0, 0, 0};
	int m_exp = 0;
	int m_nextLevelNeededExp = LevelExp;
	int m_level = 1;

};
