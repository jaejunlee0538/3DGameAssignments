#pragma once
#include "cCamera.h"
#include "Grid.h"
#include "FreqEstimater.h"
#include "TickCounter.h"
namespace SGA {
	class cMainGame
	{
	private:
		LPDIRECT3D9			m_pD3D;
		LPDIRECT3DDEVICE9	m_pD3DDevice;
		cCamera m_camera;
		LPD3DXFONT	m_pFont;
		Grid _grid;
		FreqEstimater m_freq;
		TickCounter m_counter;
	public:
		cMainGame();
		~cMainGame();

		void Setup();
		void Update();
		void Render();
	};
}

