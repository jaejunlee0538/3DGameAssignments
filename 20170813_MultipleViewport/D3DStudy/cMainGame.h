#pragma once
#include "cCamera.h"
#include "TextureManager.h"
#include "MeshModel.h"
#include "Grid.h"
#include "BezierCurve.h"
#include "ASEParser.h"
namespace SGA {
	class cMainGame
	{
	private:
		LPDIRECT3D9			m_pD3D;
		LPDIRECT3DDEVICE9	m_pD3DDevice;
		LPDIRECT3DDEVICE9	m_pD3DDevice2;
		LPDIRECT3DSWAPCHAIN9	m_pSwapChain;
		cCamera m_camera;
		cCamera m_camera2;

		//ÅÂ¾ç±¤¿ø
		D3DLIGHT9 m_lightSun;
		D3DXVECTOR3 m_sunDir;

	

		GameObject* loadedObject;
		AnimationClip *_animationClip;

		Grid _grid;

		float _time;

		int _startFrame, _endFrame;
		double _timeScale;
	public:
		cMainGame();
		~cMainGame();

		void Setup();
		void Update();
		void Render();
	};
}

