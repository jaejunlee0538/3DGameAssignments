#pragma once
#include "GameObject.h"
namespace SGA {
	class cObstacle : public GameObject
	{
	public:
		cObstacle();
		~cObstacle();
		void init(float xLen, float yLen, float zLen, D3DXVECTOR3 originPos, std::vector<D3DTexturePtr> textures);
		void update(bool isDirty=false) override;
		void render() const override;
		void setCurrentHP(int hp);
		int getCurrentHP() const;
	private:
		static int _count;
		int _id;
		int _hp;
		D3DMATERIAL9 _material;
		//D3DLIGHT9 _pointLight;
		VertexPosNormTex _vertices[36];
		std::vector<D3DTexturePtr> _textures;
		D3DTexturePtr _currentTexture;
	};
}