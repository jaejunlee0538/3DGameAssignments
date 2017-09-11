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

	//부모 좌표계를 기준으로 오브젝트를 이동시킨다.
	void translate(float dx, float dy, float dz);
	//Local 좌표계를 기준으로 오브젝트를 이동시킨다.
	void translateLocal(float dx, float dy, float dz);
	//부모 좌표계의 축을 기준으로 회전한다.
	void rotate(float rotAngleX, float rotAngleY, float rotAngleZ);
	//Local 좌표계의 축을 기준으로 회전한다.
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

	//m_parent가 null이 아닌 경우에는 부모에 대한 상대 위치
	//m_parent가 null인 경우에는 world상에서의 위치
	D3DXVECTOR3 m_position;
	D3DXQUATERNION m_quaternion;

	D3DXVECTOR3 m_linearSpeed;
	D3DXVECTOR3 m_angleSpeed;

	//world행렬
	bool m_isDirty = true;
	D3DXMATRIX m_matAnim;
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matScale;
};