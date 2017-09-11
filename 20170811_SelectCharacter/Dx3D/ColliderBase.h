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
		//GameObject�� ColliderBase�� ������ �ְ� GameObject�� �ı��ڿ��� Collider�� �����ϱ� ������
		//Collider�� GameObject�� AddRef�� ȣ���ϸ� �ȵȴ�.
	}

	virtual bool CheckRayCollision(D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir, float* distance) const = 0;
	virtual void Render() const{}
protected:
	cGameObject* m_parentObject;
};

