#pragma once
#include "Grid.h"
#include "Box.h"
#include "cCamera.h"
#include "cPyramid.h"
#include "MinecraftCharacter.h"
#include "TextureManager.h"
#include "cBoxTextured.h"
#include "PlayerController.h"
#include "cObstacle.h"
namespace SGA {
	class cMainGame
	{
	private:
		LPDIRECT3D9			m_pD3D;
		LPDIRECT3DDEVICE9	m_pD3DDevice;
		Grid m_grid;
		cCamera m_camera;
		Pyramid m_piramid[3];
		SGA::MinecraftCharacter m_character;
		SGA::cBoxTextured m_floor;

		D3DXVECTOR3 m_camPosition;

		D3DXMATRIX m_matView;
		D3DXMATRIX m_matProj;

		D3DLIGHT9 m_lightSun;
		D3DXVECTOR3 m_sunDir;

		D3DMATERIAL9 _materialFloor;
		PlayerController _playerController;
		SGA::TextureManager m_textureManager;

		std::list<std::shared_ptr<cObstacle>> _boxObstacles;

		int _currentTime;
	public:
		cMainGame();
		~cMainGame();

		void Setup();
		void Update();
		void Render();

		void GenerateCube();
	};
}

