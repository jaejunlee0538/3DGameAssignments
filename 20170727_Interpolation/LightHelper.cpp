#include "stdafx.h"
#include "LightHelper.h"

namespace SGA {
	namespace Colors {
		const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
		const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
		const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
		const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
		const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);
	}

	namespace Lights {
		void initDirectionalLight(D3DLIGHT9 & light, const D3DXCOLOR & ambient, const D3DXCOLOR & diffuse, const D3DXCOLOR & specular, const D3DXVECTOR3 & direction)
		{
			ZeroMemory(&light, sizeof(light));
			light.Type = D3DLIGHT_DIRECTIONAL;
			light.Ambient = ambient;
			light.Diffuse = diffuse;
			light.Specular = specular;
			light.Direction = direction;
		}

		void initPointLight(D3DLIGHT9 & light, const D3DXCOLOR & ambient, const D3DXCOLOR & diffuse, const D3DXCOLOR & specular, float range)
		{
			ZeroMemory(&light, sizeof(light));
			light.Type = D3DLIGHT_POINT;
			light.Ambient = ambient;
			light.Diffuse = diffuse;
			light.Specular = specular;
			light.Range = range;
		}

		void initSpotLight(D3DLIGHT9 & light, const D3DXCOLOR & ambient, const D3DXCOLOR & diffuse, const D3DXCOLOR & specular, const D3DXVECTOR3 & direction, float range, float inLightAngle, float outLightAngle)
		{
			ZeroMemory(&light, sizeof(light));
			light.Type = D3DLIGHT_SPOT;
			light.Ambient = ambient;
			light.Diffuse = diffuse;
			light.Specular = specular;
			light.Direction = direction;
			light.Range = range;
			light.Falloff = 1.0f;
			light.Theta = inLightAngle;
			light.Phi = outLightAngle;
		}
	}

	namespace Materials {
		void initMaterial(D3DMATERIAL9& material, const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular, const D3DXCOLOR& emissive, float power) {
			material.Ambient = ambient;
			material.Diffuse = diffuse;
			material.Specular = specular;
			material.Emissive = emissive;
			material.Power = power;
		}
	}
}

