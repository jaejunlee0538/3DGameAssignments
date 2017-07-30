#include "stdafx.h"
#include "cBoxTextured.h"
namespace SGA {


	cBoxTextured::cBoxTextured()
	{

	}

	cBoxTextured::~cBoxTextured()
	{

	}

	void cBoxTextured::render() const
	{
		g_pD3DDevice->SetTexture(0, _texture.get());
		g_pD3DDevice->SetFVF(VertexPosNormTex::FVF);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &getWorldMatrix());

		g_pD3DDevice->DrawPrimitiveUP(
			D3DPT_TRIANGLELIST, 12, _vertices, sizeof(VertexPosNormTex));

		GameObject::render();
	}

	void cBoxTextured::update(bool isDirty)
	{
		GameObject::update(isDirty);
	}

	void cBoxTextured::init(float xLen, float yLen, float zLen, D3DXVECTOR3 originPos, D3DTexturePtr texture, const std::vector<float>& uv)
	{
		const static int indices[36] = {  7,6,5,7,5,4/**/,0,1,2,0,2,3,/**/4,5,1,4,1,0/**/,3,2,6,3,6,7/**/,1,5,6,1,6,2/**/,4,0,3,4,3,7 };
		float x = xLen / 2;
		float y = yLen / 2;
		float z = zLen / 2;

		_texture = texture;

		D3DXVECTOR3 vertices[8] = { D3DXVECTOR3(-x,-y,-z),D3DXVECTOR3(-x,y,-z),D3DXVECTOR3(x,y,-z),D3DXVECTOR3(x,-y,-z),D3DXVECTOR3(-x,-y,z),D3DXVECTOR3(-x,y,z),D3DXVECTOR3(x,y,z),D3DXVECTOR3(x,-y,z) };
		D3DXVECTOR3 normals[6] = { D3DXVECTOR3(0,0,-1),D3DXVECTOR3(0,0,1) ,D3DXVECTOR3(-1,0,0) ,D3DXVECTOR3(1,0,0) ,D3DXVECTOR3(0,1,0) ,D3DXVECTOR3(0,-1,0) };
		for (int i = 0; i < 8; ++i) {
			vertices[i] -= originPos;
		}
		int iFace;
		//전면, 후면, 왼쪽 측면, 오른쪽 측면, 상단, 하단
		for (int iFace = 0; iFace < 6; ++iFace) {
			float top, left, bottom, right;
			left = uv[iFace * 4 + 0];
			top = uv[iFace * 4 + 1];
			right= uv[iFace * 4 + 2];
			bottom = uv[iFace * 4 + 3];

			_vertices[iFace * 6 + 0].p = vertices[indices[iFace * 6 + 0]];
			_vertices[iFace * 6 + 1].p = vertices[indices[iFace * 6 + 1]];
			_vertices[iFace * 6 + 2].p = vertices[indices[iFace * 6 + 2]];
			_vertices[iFace * 6 + 3].p = vertices[indices[iFace * 6 + 3]];
			_vertices[iFace * 6 + 4].p = vertices[indices[iFace * 6 + 4]];
			_vertices[iFace * 6 + 5].p = vertices[indices[iFace * 6 + 5]];
			_vertices[iFace * 6 + 0].uv = D3DXVECTOR2(left, bottom);
			_vertices[iFace * 6 + 1].uv = D3DXVECTOR2(left, top);
			_vertices[iFace * 6 + 2].uv = D3DXVECTOR2(right, top);
			_vertices[iFace * 6 + 3].uv = D3DXVECTOR2(left, bottom);
			_vertices[iFace * 6 + 4].uv = D3DXVECTOR2(right, top);
			_vertices[iFace * 6 + 5].uv = D3DXVECTOR2(right, bottom);
			_vertices[iFace * 6 + 0].n = normals[iFace];
			_vertices[iFace * 6 + 1].n = normals[iFace];
			_vertices[iFace * 6 + 2].n = normals[iFace];
			_vertices[iFace * 6 + 3].n = normals[iFace];
			_vertices[iFace * 6 + 4].n = normals[iFace];
			_vertices[iFace * 6 + 5].n = normals[iFace];
		}
	}
}
