#pragma once
#include "Grid.h"
#include "Box.h"
#include "cCamera.h"
#include "cPyramid.h"

class cMainGame
{
private:
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pD3DDevice;
	Grid m_grid;
	Box m_box;
	cCamera m_camera;
	Pyramid m_piramid[3];

	D3DXVECTOR3 m_camPosition;

	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
public:
	cMainGame();
	~cMainGame();

	void Setup();
	void Update();
	void Render();
};

