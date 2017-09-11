#pragma once
class GameWorld;
class cAction;
class ColliderBase;
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();
	virtual void render() const;
	virtual void update(bool isDirty);

	void setGameWorld(GameWorld* world) {
		m_pWorld = world;
	}
	void setTag(const char* tag);
	const std::string& getTag() const;
	bool isTagSame(const char* tag) const;

	void setScale(float sx, float sy, float sz);
	void setScale(const D3DXVECTOR3& scale);
	void setPosition(float x, float y, float z);
	void setPosition(const D3DXVECTOR3& pos);
	void setPositionY(float y);
	void setRotation(float angleX, float angleY, float angleZ);
	void setRotation(const D3DXVECTOR3& axis, float angle);
	void setRotation(const D3DXQUATERNION& quat);

	void setSpeed(float vx, float vy, float vz);
	void setLocalSpeed(float vx, float vy, float vz);
	D3DXVECTOR3 getSpeed() const;
	void setAngularSpeed(float vx, float vy, float vz);
	void UpdatePositionWithSpeed(float dt);
	bool isMoving() const;
	void setTransform(const D3DXMATRIX* transform);

	const D3DXVECTOR3& getLocalPosition() const { return m_position; }
	const D3DXQUATERNION& getLocalRotation() const { return m_quaternion; }

	D3DXVECTOR3 getWorldPosition() const;
	D3DXQUATERNION getWorldRotation() const;

	//�θ� ��ǥ�踦 �������� ������Ʈ�� �̵���Ų��.
	void translate(float dx, float dy, float dz);
	//Local ��ǥ�踦 �������� ������Ʈ�� �̵���Ų��.
	void translateLocal(float dx, float dy, float dz);
	//�θ� ��ǥ���� ���� �������� ȸ���Ѵ�.
	void rotate(float rotAngleX, float rotAngleY, float rotAngleZ);
	//Local ��ǥ���� ���� �������� ȸ���Ѵ�.
	void rotateLocal(float rotAngleX, float rotAngleY, float rotAngleZ);

	void calculateWorldMatrix();
	const D3DXMATRIX& getWorldMatrix() const { return m_matWorld; }

	size_t getNObjects() const;

	GameObject* getParent() { return m_parent; }
	bool hasParent() const;
	bool hasChild() const;
	void insertChild(GameObject* child);

	void insertChild(const char* parentTag, GameObject* child);
	GameObject* findObject(const char* tag);
	GameObject* removeObject(const char* tag);
	bool deleteObject(const char* tag);
	void clearChilds();

	void setAction(cAction* action);

	bool checkRayCollision(D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir);

	virtual void handleMessage(int msgType, const std::vector<cVariant>& params){}
protected:
	GameWorld* m_pWorld;
private:
	GameObject* m_parent = nullptr;
	cAction* m_pAction;
	ColliderBase* m_pCollider;

	std::list<GameObject*> m_childs;

	std::string m_tag;

	//m_parent�� null�� �ƴ� ��쿡�� �θ� ���� ��� ��ġ
	//m_parent�� null�� ��쿡�� world�󿡼��� ��ġ
	D3DXVECTOR3 m_position;
	D3DXQUATERNION m_quaternion;

	D3DXVECTOR3 m_linearSpeed;
	D3DXVECTOR3 m_angleSpeed;

	//world���
	bool m_isDirty = true;
	D3DXMATRIX m_matAnim;
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matScale;
};