#pragma once
class ColliderBase
{
public:
	ColliderBase():m_parentObject(NULL){}
	virtual ~ColliderBase() {
		//SAFE_RELEASE(m_parentObject);
	}

	void SetParent(cGameObject* parentObject) {
		//SAFE_RELEASE(m_parentObject);
		m_parentObject = parentObject;
		//m_parentObject->AddRef();
		//GameObject가 ColliderBase를 가지고 있고 GameObject의 파괴자에서 Collider를 삭제하기 때문에
		//Collider는 GameObject에 AddRef를 호출하면 안된다.
	}

	virtual bool CheckRayCollision(D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir, float* distance) const = 0;
	virtual void Render() const{}
protected:
	cGameObject* m_parentObject;
};

