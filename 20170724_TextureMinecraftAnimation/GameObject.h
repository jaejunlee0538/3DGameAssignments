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

		//�θ� ��ǥ�踦 �������� ������Ʈ�� �̵���Ų��.
		void translate(float dx, float dy, float dz);
		//Local ��ǥ�踦 �������� ������Ʈ�� �̵���Ų��.
		void translateLocal(float dx, float dy, float dz);
		//�θ� ��ǥ���� ���� �������� ȸ���Ѵ�.
		void rotate(float rotAngleX, float rotAngleY, float rotAngleZ);
		//Local ��ǥ���� ���� �������� ȸ���Ѵ�.
		void rotateLocal(float rotAngleX, float rotAngleY, float rotAngleZ);

		void calculateWorldMatrix();
		const D3DXMATRIX& getWorldMatrix() const { return _matWorld; }

		//this�� ������ this�� ���� ������Ʈ���� �ִϸ��̼� ����� �����Ѵ�.
		//������Ʈ�� _tag�� ��ġ�ϴ� �̸��� ���� �ִϸ��̼� ����� ���õȴ�.
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

		//_parent�� null�� �ƴ� ��쿡�� �θ� ���� ��� ��ġ
		//_parent�� null�� ��쿡�� world�󿡼��� ��ġ
		D3DXVECTOR3 _position;
		D3DXVECTOR3 _angles;
		
		//world���
		bool _isDirty = true;
		D3DXMATRIX _matAnim;
		D3DXMATRIX _matWorld;
	};
}

