#pragma once

namespace EffectTypes {
	struct DirectionalLight {
		D3DXVECTOR4 Ambient;
		D3DXVECTOR4 Diffuse;
		D3DXVECTOR4 Specular;
		D3DXVECTOR3 Direction;
		float pad;

		DirectionalLight() {
			Ambient = D3DXVECTOR4(0, 0, 0, 0);
			Diffuse = D3DXVECTOR4(0, 0, 0, 0);
			Specular = D3DXVECTOR4(0, 0, 0, 0);
			Direction = D3DXVECTOR3(0, -1, 0);
			pad = 0;
		}
	};

	struct Material
	{
		D3DXVECTOR4 Ambient;
		D3DXVECTOR4 Diffuse;
		D3DXVECTOR4 Specular;//w = SpecPower
		D3DXVECTOR4 Reflect;

		void InitFromD3DMaterial(const D3DMATERIAL9& mtl) {
			Ambient = D3DXVECTOR4(mtl.Ambient.r, mtl.Ambient.g, mtl.Ambient.b, mtl.Ambient.a);
			Diffuse = D3DXVECTOR4(mtl.Diffuse.r, mtl.Diffuse.g, mtl.Diffuse.b, mtl.Diffuse.a);
			Specular = D3DXVECTOR4(mtl.Specular.r, mtl.Specular.g, mtl.Specular.b, mtl.Power);
			Reflect = D3DXVECTOR4(0, 0, 0, 0);
		}
	};
}