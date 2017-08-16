#pragma once
#include "cGameObject.h"
class cFrame;
class cPlayer : public cGameObject
{
private:
	cFrame* m_pRunning;
	cFrame* m_pIdle;
	cFrame* m_currentAnimation;

	D3DXMATRIX		m_playerOffset;

	//bool			m_bRunning;
public:
	cPlayer();
	~cPlayer();

	void Init();
	void Update(DWORD time) override;
	void Render();
};

