#pragma once
namespace SGA {
	namespace Colors {
		extern const D3DXCOLOR WHITE;
		extern const D3DXCOLOR BLACK;
		extern const D3DXCOLOR RED;
		extern const D3DXCOLOR GREEN;
		extern const D3DXCOLOR BLUE;
	}

	namespace Lights {
		void initDirectionalLight(D3DLIGHT9& light, const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular, const D3DXVECTOR3& direction);

		void initPointLight(D3DLIGHT9& light, const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular, float range);

		void initSpotLight(D3DLIGHT9& light, const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular, const D3DXVECTOR3& direction, float range, float inLightAngle, float outLightAngle);
	}

	namespace Materials {
		void initMaterial(D3DMATERIAL9& material, const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular, const D3DXCOLOR& emissive, float power);
	}
}