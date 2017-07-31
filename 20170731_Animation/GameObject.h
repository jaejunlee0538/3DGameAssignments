#pragma once
namespace SGA {
	class GameObject
	{
	public:
		GameObject();
		virtual ~GameObject();
		virtual void render() const;
		virtual void update(bool isDirty);
		
		void setTag(const char* tag);
		const std::string& getTag() const;
		bool isTagSame(const char* tag) const;

		void setScale(float sx, float sy, float sz);
		void setSpeed(float vx, float vy, float vz);
		void setPosition(float x, float y, float z);
		void setRotation(float angleX, float angleY, float angleZ);
		void setRotation(const D3DXVECTOR3& axis, float angle);

		void setTransform(const D3DXMATRIX* transform);

		const D3DXVECTOR3& getPosition() const { return _position; }
		const D3DXQUATERNION& getQuaternion() const { return _quaternion; }

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
	
		size_t getNObjects() const;

		GameObject* getParent() { return _parent; }
		bool hasParent() const;
		bool hasChild() const;
		void insertChild(GameObject* child);

		void insertChild(const char* parentTag, GameObject* child);
		GameObject* findObject(const char* tag);
		GameObject* removeObject(const char* tag);
		bool deleteObject(const char* tag);
		void clearChilds();
	private:
		GameObject* _parent = nullptr;
		//GameObject* _headChilds = nullptr;
		//GameObject* _nextSibling = nullptr;
		std::list<GameObject*> _childs;
		
		std::string _tag;

		//_parent�� null�� �ƴ� ��쿡�� �θ� ���� ��� ��ġ
		//_parent�� null�� ��쿡�� world�󿡼��� ��ġ
		D3DXVECTOR3 _position;
		D3DXQUATERNION _quaternion;
		
		//world���
		bool _isDirty = true;
		D3DXMATRIX _matAnim;
		D3DXMATRIX _matWorld;
		D3DXMATRIX _matScale;
	};
}

