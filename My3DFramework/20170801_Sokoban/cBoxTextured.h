#pragma once
#include "GameObject.h"
namespace SGA {

	class cBoxTextured : public GameObject
	{
	public:
		cBoxTextured();
		~cBoxTextured();

		void render() const override;
		void update(bool isDirty = false) override;
		//전면, 후면,왼쪽 측면, 오른쪽 측면, 상단, 하단
		void init(float xLen, float yLen, float zLen, D3DXVECTOR3 originPos, D3DTexturePtr texture, const std::vector<float>& uv);
	private:
		VertexPosNormTex _vertices[36];
		mutable D3DTexturePtr _texture;
	};
}
