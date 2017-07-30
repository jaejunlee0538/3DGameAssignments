#pragma once
#include "cCamera.h"
#include "TextureManager.h"
#include "MeshModel.h"
#include "Grid.h"

namespace SGA {
	class cMainGame
	{
	private:
		LPDIRECT3D9			m_pD3D;
		LPDIRECT3DDEVICE9	m_pD3DDevice;
		cCamera m_camera;

		//ÅÂ¾ç±¤¿ø
		D3DLIGHT9 m_lightSun;
		D3DXVECTOR3 m_sunDir;

		MeshModel::Ptr _meshModel;
		MeshModel::Ptr _meshBox;
		MeshModel::Ptr _meshRustyBox;

		Grid _grid;
	public:
		cMainGame();
		~cMainGame();

		void Setup();
		void Update();
		void Render();
	};
}

