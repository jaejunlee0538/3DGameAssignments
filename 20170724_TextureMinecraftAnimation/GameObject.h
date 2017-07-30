#pragma once
namespace SGA {
	class GameObject
	{
	public:
		GameObject();
		virtual ~GameObject();
		virtual void render() const;
		virtual void update(bool isDirty);
		
		void setTag(const TCHAR* tag);
		const tstring& getTag() const;
		bool isTagSame(const TCHAR* tag) const;

		void setSpeed(float vx, float vy, float vz);
		void setPosition(float x, float y, float z);
		void setRotation(float angleX, float angleY, float angleZ);

		const D3DXVECTOR3& getPosition() const { return _position; }
		const D3DXVECTOR3& getAngles() const { return _angles; }

		//부모 좌표계를 기준으로 오브젝트를 이동시킨다.
		void translate(float dx, float dy, float dz);
		//Local 좌표계를 기준으로 오브젝트를 이동시킨다.
		void translateLocal(float dx, float dy, float dz);
		//부모 좌표계의 축을 기준으로 회전한다.
		void rotate(float rotAngleX, float rotAngleY, float rotAngleZ);
		//Local 좌표계의 축을 기준으로 회전한다.
		void rotateLocal(float rotAngleX, float rotAngleY, float rotAngleZ);

		void calculateWorldMatrix();
		const D3DXMATRIX& getWorldMatrix() const { return _matWorld; }

		//this를 포함한 this의 하위 오브젝트들의 애니메이션 행렬을 설정한다.
		//오브젝트의 _tag와 일치하는 이름을 가진 애니메이션 행렬이 선택된다.
		void setKeyFrameAnimation(const KeyFrameSnapshot& animMatrices);
		void setAnimationMatrix(const D3DXMATRIX& matAnim);

		void insertChild(GameObject* child);
		void setParent(GameObject* parent);
		GameObject* findObject(const TCHAR* tag);
		GameObject* removeObject(const TCHAR* tag);
		bool deleteObject(const TCHAR* tag);
		void clearChilds();
	private:
		GameObject* _parent = nullptr;
		GameObject* _headChilds = nullptr;
		GameObject* _nextSibling = nullptr;
		
		tstring _tag;

		//_parent가 null이 아닌 경우에는 부모에 대한 상대 위치
		//_parent가 null인 경우에는 world상에서의 위치
		D3DXVECTOR3 _position;
		D3DXVECTOR3 _angles;
		
		//world행렬
		bool _isDirty = true;
		D3DXMATRIX _matAnim;
		D3DXMATRIX _matWorld;
	};
}

