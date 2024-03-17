#pragma once

#include <alligator/util/math.h>
#include <alligator/util/rng.h>

#include <allegro5/allegro_color.h>
#include <memory>
#include <vector>
#include <stack>

#include <string>

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
		m_currentHP = max(0.f, m_currentHP);
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
			std::cout << "removing stun" << std::endl;
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
				if( se->type == Stun )
				{
					m_stunned = false;
				}
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

	bool isBurnt()
	{
		return hasEffect(StatusEffectType::Burnt);
	}

	bool hasEffect(StatusEffectType effect)
	{
		return TurnsLeftForStatus(effect);
	}

	int TurnsLeftForStatus(StatusEffectType effect)
	{
		for (auto item : status)
		{
			if (item->type == effect)
			{
				return item->turnsLeft;
			}
		}
		return -1;
	}

	const std::vector<StatusEffect::SharedPtr> GetStatuses()
	{
		return status;
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
	virtual int operator()( Ability* ability, Entity* source, std::vector<Entity*> targets ) = 0 ;
};

class Ability
{
public:
	typedef std::shared_ptr<Ability> SharedPtr;

	Ability( ShapeType stype, std::string txt, PickType pt = PickSingle, AbilityApplicator* applic = nullptr, float effectiveness = 1.f, int cooldown = 1.f, std::string desc1="", std::string desc2="", std::string desc3="", std::string desc4=""  )
		: m_shapeType(stype),
		  m_text(txt),
		  m_pickType(pt),
		  applicator(applic),
		  m_effectiveness(effectiveness),
		  cooldown(cooldown),
		  currentCooldown(0)
	{
		this->desc1 = desc1;
		this->desc2 = desc2;
		this->desc3 = desc3;
		this->desc4 = desc4;
	}

	bool apply( Entity* source, std::vector<Entity*> targets )
	{
		bool ret = false;
		if( applicator != nullptr )
		{
			ret = (*applicator)(this, source, targets);
			currentCooldown = cooldown + 1;
		}
		return ret;
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

	std::string desc1;
	std::string desc2;
	std::string desc3;
	std::string desc4;

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

	bool rollMiss( Entity* src, Entity* target )
	{
		float srcagi = src->getStat(AGI);
		float tgtagi = target->getStat(AGI);
		if( srcagi > tgtagi )
		{
			return false;
		}
		else
		{
			float thresh = tgtagi - srcagi;
			int r = RNG::rng->nextInt(100);
			return thresh > r;
		}
	}

	bool rollCritical( Entity* src, Entity* target )
	{
		float srcagi = src->getStat(AGI);
		float tgtagi = target->getStat(AGI);
		if( srcagi > tgtagi )
		{
			float thresh = srcagi - tgtagi;
			int r = RNG::rng->nextInt(30);
			return thresh > r;
		}
		else
		{
			return false;
		}
	}

	int operator()( Ability* ability, Entity* source, std::vector<Entity*> targets ) override
	{
		int ret = 0;
		float source_atk = ATK_FACTOR * source->getStat(ATK);
		for( Entity* target : targets )
		{
			if( target != nullptr )
			{
				bool decideMiss = rollMiss(source, target);
				if( !decideMiss )
				{
					float target_def = DEF_FACTOR * target->getStat(DEF);
					float dmg = max(0.f, source_atk * ability->effectiveness() - target_def) * DMG_FACTOR;
					// std::cout << "effec: " << ability->effectiveness() << ", srcatk: " << source_atk << ", tgtdef: " << target_def << ", dmg: " << dmg << std::endl;

					bool decideCrit = rollCritical(source, target);
					if( decideCrit )
					{
						dmg *= 1.5f;
						ret = 2;
					}

					target->decreaseHP(max(0.f, dmg));
					if( m_model != nullptr )
					{
						StatusEffect::SharedPtr se(new StatusEffect);
						se->value = source->getStat(ATK) / 2.f;
						se->turnsLeft = m_model->turnsLeft;
						se->type = m_model->type;
						target->status.push_back(se);
					}
					ret = 1;
				}
			}
		}
		return ret;
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

	int operator()( Ability* ability, Entity* source, std::vector<Entity*> targets ) override
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
		return 1;
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
	int operator()( Ability* ability, Entity* source, std::vector<Entity*> targets ) override
	{
		StatusEffect::SharedPtr se(new StatusEffect);
		se->turnsLeft = 3;
		se->type = Healing;
		source->status.push_back(se);
		//e->healHP( source->getStat(HP) * 0.10 );
		return 1;
	}
};



class DemolecAbilityApplicator : public AbilityApplicator
{
public:
	int operator()( Ability* ability, Entity* source, std::vector<Entity*> targets ) override
	{
		StatusEffect::SharedPtr se(new StatusEffect);
		se->turnsLeft = 5;
		se->type = Demolec;
		source->status.push_back(se);
		return 1;
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

	StatusEffect burning2;
	OnlyStatusEffectAbilityApplicator burnAP2;

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
		burning.turnsLeft = 4;
		burning.type = Burnt;
		burning.value = 0; // IGNORED BECAUSE IT'S A MODEL
		//burnAP.setModel(&burning);
		burnAP.setup(false, &burning, ATK, 0.5f);

		burning2.turnsLeft = 3;
		burning2.type = Burnt;
		burning2.value = 0; // IGNORED BECAUSE IT'S A MODEL
		//burnAP.setModel(&burning);
		burnAP2.setup(false, &burning2, ATK, 0.2f);

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
		return 1.2f * m_stats.agi + 1.5f * m_stats.atk + 1.4f * m_stats.def + m_stats.hp * 1.1f;
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
		Stats stats(points * 1.3f, points, points, points);
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

	Player() : Entity(Stats(100, 5, 5, 5), Stats(1,1,1,1))
	{
		abilities().push_back(Ability::SharedPtr(new Ability(Water, "Gob", PickSingle, &(aps.damageAP), 2.f, -2.f,
															 "Low damage.", "Get saliva", "and SHOT!!")));
		abilities().push_back(Ability::SharedPtr(new Ability(Fire, "Chaos Fart", Pick2x2Block, &(aps.damageAP), 1.f, -2.f,
															 "Low damage,", "all enemies.", "Nothing like", "good ol' farts.")));
		abilities().push_back(Ability::SharedPtr(new Ability(Gaia, "Shard", Pick2x2Block, &(aps.stunAttackAP), 0.5f, 2.f,
															 "Stun 2x2 block", "of enemies.", "Throw one rock,", "enjoy all day.")));

		// unlockAll();

		// WATER

		// GAIA

		// FIRE
	}

	void unlockAll()
	{
		unlock(Gaia);
		unlock(Gaia);
		unlock(Gaia);
		unlock(Gaia);

		unlock(Fire);
		unlock(Fire);
		unlock(Fire);
		unlock(Fire);

		unlock(Water);
		unlock(Water);
		unlock(Water);
		unlock(Water);
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
			m_shapeModStats.def = 1.5f;
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
				abilities().push_back(Ability::SharedPtr(new Ability(Water, "PressureShot", PickSingle, &(aps.stunAttackAP), 2.f, 3,
																	 "Stunts,", "single target,", "high damage.", "-GroggyF")));
				break;
			case 2:
				abilities().push_back(Ability::SharedPtr(new Ability(Water, "Tsunami", Pick2x2Block, &(aps.damageAP), 2.f, 3.f,
																	 "All enemies.", "Mid damage.", "Watersome,", "Rekt.")));
				break;
			case 3:
				abilities().push_back(Ability::SharedPtr(new Ability(Water, "Demolecularize", PickSelf, &(aps.demolecAP), 1.f, 6,
																	 "Agi +200%!!", "You fade", "through your", "oponents, dawg")));
				break;
			case 4:
				abilities().push_back(Ability::SharedPtr(new Ability(Water, "Mare Nostrum", PickAll, &(aps.damageAP), 5.f, 6,
																	 "All enemies.", "High damage.", "Watersome,", "Rekt.")));
				break;
			}
			break;
		case Gaia:




			m_shapeLevel[1]++;
			switch(m_shapeLevel[1])
			{
			case 1:
				abilities().push_back(Ability::SharedPtr(new Ability(Gaia, "Regen", PickSelf, &(aps.healAP), 1.f, 6,
																	 "HP regen?", "C'mon and", "go play", "some COD.")));
				break;
			case 2:
				abilities().push_back(Ability::SharedPtr(new Ability(Gaia, "Mega Shell", PickSelf, &(aps.shellAP), 1.f, 6.f,
																	 "Def +200%!!", "Defend like", "a boss, dawg.", "#NO_DWG_WO_SHLL")));
				break;
			case 3:
				abilities().push_back(Ability::SharedPtr(new Ability(Gaia, "Earthquake", Pick2x2Block, &(aps.damageAP), 3.f, 4.f,
																	 "All enemies.", "High damage.", "Did someone fart?")));
				break;
			case 4:
				abilities().push_back(Ability::SharedPtr(new Ability(Gaia, "Meteor", PickSingle, &(aps.damageAP), 8.f, 1.f,
																	 "High damage", "single enemy.", "Zurians meteor", "in your face.")));
				break;
			}
			break;
		case Fire:



			m_shapeLevel[2]++;
			switch(m_shapeLevel[2])
			{
			case 1:
				abilities().push_back(Ability::SharedPtr(new Ability(Fire, "Torch", PickSingle, &(aps.burnAP2), 0.5f, 1.f,
																	 "Do you have", "a light?", "Provokes burn.", "Stack it, dawg!")));
				break;
			case 2:
				abilities().push_back(Ability::SharedPtr(new Ability(Fire, "Flame Fists", PickSelf, &(aps.fistAP), 1, 6, "I deal moar", "damage yes!", "Well, during", "the effect...")));
				break;
			case 3:
				abilities().push_back(Ability::SharedPtr(new Ability(Fire, "Blaze", Pick2x2Block, &(aps.burnAP), 2, 6.f,
																	 "Affects 2x2 block", "of enemies.", "Provokes burn.", "EVEN FIRER!!")));
				break;
			case 4:
				abilities().push_back(Ability::SharedPtr(new Ability(Fire, "Magma", PickAll, &(aps.damageAP), 3, 8.f,
																	 "High damage", "all enemies", "Yes, you read", "well.")));
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
				m_stats.hp += 10;
				levelingNotification.text4 = std::to_string((int)m_stats.agi) + " + " + std::to_string(4) + " agi!";
				m_stats.agi += 4;
				levelingNotification.text3 = std::to_string((int)m_stats.def) + " + " + std::to_string(2) + " def!";
				m_stats.def += 2;
				levelingNotification.text2 = std::to_string((int)m_stats.atk) + " + " + std::to_string(3) + " atk!";
				m_stats.atk += 3;
				break;
			case Gaia:
				m_stats.hp += 15;
				levelingNotification.text4 = std::to_string((int)m_stats.agi) + " + " + std::to_string(3) + " agi!";
				m_stats.agi += 3;
				levelingNotification.text3 = std::to_string((int)m_stats.def) + " + " + std::to_string(6) + " def!";
				m_stats.def += 6;
				levelingNotification.text2 = std::to_string((int)m_stats.atk) + " + " + std::to_string(2) + " atk!";
				m_stats.atk += 2;
				break;
			case Fire:
				m_stats.hp += 5;
				levelingNotification.text4 = std::to_string((int)m_stats.agi) + " + " + std::to_string(3) + " agi!";
				m_stats.agi += 3;
				levelingNotification.text3 = std::to_string((int)m_stats.def) + " + " + std::to_string(2) + " def!";
				m_stats.def += 2;
				levelingNotification.text2 = std::to_string((int)m_stats.atk) + " + " + std::to_string(4) + " atk!";
				m_stats.atk += 4;
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

	int m_level = 1;

	int m_shapeLevel[3] = {0, 0, 0};


private:
	std::vector<Ability::SharedPtr> m_abilities;
	ShapeType currentShape = Water;

	int m_exp = 0;
	int m_nextLevelNeededExp = LevelExp;

};
