#pragma once
#include "cCamera.h"
#include "TextureManager.h"
#include "MeshModel.h"
#include "Grid.h"
#include "MinecraftCharacter.h"
#include "BezierCurve.h"
#include "ASEParser.h"
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

		std::vector<VertexPosDiff> _hexagon;
		BezierCurve _bezierCurve[3];
		BezierCurve _linearCurve;

		ASEParser _aseParser;

		MeshObject* loadedObject;

		Grid _grid;

		float _time;
	public:
		cMainGame();
		~cMainGame();

		void Setup();
		void Update();
		void Render();
	};
}

