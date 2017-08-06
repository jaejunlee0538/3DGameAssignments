#include "stdafx.h"
#include "Axis.h"

namespace SGA {

	Axis::Axis(float scale)
	{
		init(scale);
	}


	Axis::~Axis()
	{
	}

	void Axis::init(float scale)
	{
		if (scale <= 0.0f) {
			scale = 1.0f;
		}
		_pts[0].color = D3DCOLOR_XRGB(255, 0, 0);
		_pts[1].color = D3DCOLOR_XRGB(255, 0, 0);
		_pts[2].color = D3DCOLOR_XRGB(0, 255, 0);
		_pts[3].color = D3DCOLOR_XRGB(0, 255, 0);
		_pts[4].color = D3DCOLOR_XRGB(0, 0, 255);
		_pts[5].color = D3DCOLOR_XRGB(0, 0, 255);

		_pts[0].p = D3DXVECTOR3(0, 0, 0);
		_pts[2].p = D3DXVECTOR3(0, 0, 0);
		_pts[4].p = D3DXVECTOR3(0, 0, 0);

		_pts[1].p = D3DXVECTOR3(scale, 0, 0);
		_pts[3].p = D3DXVECTOR3(0, scale, 0);
		_pts[5].p = D3DXVECTOR3(0, 0, scale);
	}

	void Axis::render() const
	{
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
		g_pD3DDevice->SetTexture(0, 0);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &getWorldMatrix());
		g_pD3DDevice->SetFVF(_pts[0].FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 3, &_pts[0], sizeof(_pts[0]));
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
		GameObject::render();
	}

}