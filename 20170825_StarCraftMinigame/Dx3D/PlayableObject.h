#pragma once
#include "GameObject.h"
#include "AnimCallbackKey.h"
class cSkinnedMesh;
class PlayableObject :
	public GameObject
{

	enum AnimCallbackIDs {
		ANIM_CB_ATTACK,
		ANIM_CB_ATTACK_FINISHED
	};
protected:
	enum States {
		STATE_IDLE,
		STATE_WALK,
		STATE_ATTACK
	};
public:
	PlayableObject(int maxHP, cSkinnedMesh * mesh);
	virtual ~PlayableObject();

	virtual void setup() = 0;
	virtual void render() const override;
	virtual void update(bool isDirty) override;

	void attack();

	void reupdateTransform();
	
	void takeDamage(int damage) {
		m_currentHP -= damage;
		if (m_currentHP < 0)
			m_currentHP = 0;
	}

	int currentStateID()const {
		return m_currentStateID;
	}
	int getCurrentHP() const {
		return m_currentHP;
	}
	int getMaxHP() const {
		return m_maxHP;
	}

	void setState(States state);
protected:
	virtual void attackImpl() = 0;
	void cbAnimation(int data);
	void setAnimations(std::string idle, std::string attack, std::string walk) {
		m_animIdle = idle;
		m_animAttack = attack;
		m_animWalk = walk;
	}
private:
	States			 m_state;
	int32_t			 m_attackDamage;
	cSkinnedMesh*	 m_pSkinnedMesh;
	int				 m_currentStateID;
	int				 m_maxHP;
	int				 m_currentHP;

	std::string		 m_currentAnim;
	std::string		 m_animIdle;
	std::string		 m_animAttack;
	std::string		 m_animWalk;
};

