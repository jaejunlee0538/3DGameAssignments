#pragma once
#include "cUIButton.h"

class cGrid;
class cCamera;
class cCrtCtrl;
class iMap;
class cSkinnedMesh;
class cFrustum;
class cAstar;
class cMapLoader;
class cActionMove;
class CountDown;
class HPBar;

class cMainGame : public iButtonDelegate
{
private:
	cGrid*			m_pGrid;
	cCamera*		m_pCamera;
	cCrtCtrl*		m_pCrtCtrl;
	LPD3DXFONT		m_pFont;
	iMap*			m_pMap;
	cFrustum*		m_pFrustum;
	LPD3DXSPRITE	m_pSprite;
	cAstar*			m_pAstar;

	vector<cSkinnedMesh*>	m_vecSkinnedMesh;
	LPDIRECT3DTEXTURE9	m_pTexture;
	D3DXIMAGE_INFO		m_stImageInfo;
	cMapLoader*			m_pNodeMap;
	cGameObject*		m_RootObject;
	list<ST_NODE*>		lstNode;
	

	vector<ST_PC_VERTEX>	m_vecSquareVertex;

	cGameObject*		m_EnemyObject;
	vector<cSkinnedMesh*>	m_vecSkinnedMeshMarin;
	list<ST_NODE*>		lstNodeMarin;
	cAstar*			m_pAstarMarin;
	bool			m_bNearCrt;
	bool			m_bRender;

	CountDown*			m_pCount;
	HPBar*				Zealothp;
	HPBar*				Marinhp;
	int					totalZealotHp;
	int					currentZealotHp;
	int					Zealotdamage;
	int					totalMarinHp;
	int					currentMarinHp;
	int					Marindamage;
	bool				attackting;
	bool				m_bStageClear;
	bool				m_bStageOver;
	int					m_ClearNum;


public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetLight();
	cActionMove* CreateActionMove(D3DXVECTOR3 from, D3DXVECTOR3 to, cGameObject* target);
	void setRender();

	// iButtonDelegate override
	virtual void OnClick(cUIButton* pSender) override;

};

