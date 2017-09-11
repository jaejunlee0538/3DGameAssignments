#pragma once
#include "GameObject.h"
class cSkinnedMesh;
class ZealotPlayer :
	public GameObject
{
	enum ZealotState {
		STATE_WALK,
		STATE_ATTACK,
		STATE_IDLE
	};
public:
	ZealotPlayer();
	~ZealotPlayer();

	void setup(cSkinnedMesh* mesh);
	virtual void render() const override;
	virtual void update(bool isDirty) override;
	void reupdateTransform();
	void setState(ZealotState state);
private:
	cSkinnedMesh*	m_pSkinnedMesh;
	ZealotState		m_state;
};

