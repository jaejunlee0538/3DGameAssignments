#pragma once
#include "EffectStructTypes.h"
class cGrid;
class cCamera;
class cSkinnedModel;

class cMainGame
{
private:
	cGrid*			m_pGrid;
	cCamera*		m_pCamera;
	cSkinnedModel*	m_pZealot;
	LPD3DXFONT		m_pFont;

	LPD3DXMESH		m_pBall;
	EffectTypes::Material m_mtlBall;

	LPD3DXMESH		m_pGroundBox;
	EffectTypes::Material m_mtlGround;
	EffectTypes::DirectionalLight m_dirLights[3];
	int m_numDirLights;

	ID3DXEffect*	m_pEffectDrawShadowMap;
	ID3DXEffect*	m_pEffectDrawScene;
public:
	cMainGame(void);
	~cMainGame(void);

	bool Setup();
	void Update();
	void Render();
	
	//메시지를 받아서 처리했으면 true를 반환하고 그렇지 않으면 false를 반환한다.
	bool MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	void InitBall();
	void InitGround();
	void InitFont();
	void InitZealot();
	void InitCamera();
	void InitGrid();
	void InitLight();

	void DrawTextAnimationList();
};

