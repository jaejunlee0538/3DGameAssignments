#include "stdafx.h"
#include "GameObject.h"
#include <algorithm>
#include "cAction.h"
#include "ColliderBase.h"
GameObject::GameObject()
	:m_pAction(NULL),
	m_pCollider(NULL),
	m_pWorld(NULL)
{
	D3DXMatrixIdentity(&m_matAnim);
	setScale(1, 1, 1);
	setPosition(0, 0, 0);
	setRotation(0, 0, 0);
	m_linearSpeed =m_angleSpeed = D3DXVECTOR3(0, 0, 0);
}

GameObject::~GameObject()
{
	SAFE_DELETE(m_pAction);
	SAFE_DELETE(m_pCollider);
	clearChilds();
}

void GameObject::render() const
{
	//모든 자식을 그린다.
	for each(auto pChild in m_childs) {
		pChild->render();
	}
}

void GameObject::update(bool isDirty)
{
	UpdatePositionWithSpeed(g_pTimeManager->GetDeltaTime());
	isDirty |= m_isDirty;

	if (m_pAction) {
		m_pAction->Update();
	}

	if (isDirty) {
		calculateWorldMatrix();
		m_isDirty = false;
	}

	//모든 자식을 업데이트한다.
	for each(auto pChild in m_childs) {
		pChild->update(isDirty);
	}
}

void GameObject::setTag(const char * tag)
{
	m_tag = tag;
}

const std::string & GameObject::getTag() const
{
	return m_tag;
}

bool GameObject::isTagSame(const char * tag) const
{
	return m_tag.compare(tag) == 0;
}

void GameObject::setScale(float sx, float sy, float sz)
{
	D3DXMatrixScaling(&m_matScale, sx, sy, sz);
}

void GameObject::setScale(const D3DXVECTOR3 & scale)
{
	setScale(scale.x, scale.y, scale.z);
}

void GameObject::calculateWorldMatrix()
{
	D3DXMATRIX matRot;
	D3DXMATRIX matTranslation;
	D3DXMatrixTranslation(&matTranslation, m_position.x, m_position.y, m_position.z);
	D3DXMatrixRotationQuaternion(&matRot, &m_quaternion);
	m_matWorld = matRot *  matTranslation;

	if (m_parent) {
		//부모가 있는 경우 부모의 world행렬을 곱한다.
		m_matWorld = m_matWorld * m_parent->getWorldMatrix();
	}
	else {
		m_matWorld = m_matScale * m_matWorld;
	}
}

size_t GameObject::getNObjects() const
{
	size_t count = 1;
	for each(auto pChild in m_childs) {
		count += pChild->getNObjects();
	}
	return count;
}

bool GameObject::hasParent() const
{
	return m_parent != nullptr;
}

bool GameObject::hasChild() const
{
	return m_childs.empty() == false;
}

void GameObject::insertChild(GameObject * child)
{
	assert(child != nullptr);
	//head에 새로운 자식을 삽입한다.
	m_childs.push_front(child);
	child->m_isDirty = true;
	child->m_parent = this;
}

void GameObject::insertChild(const char * parentTag, GameObject * child)
{
	GameObject* parent = findObject(parentTag);
	if (parent) {
		parent->insertChild(child);
	}
	else {
		delete child;
	}
}

GameObject * GameObject::findObject(const char * tag)
{
	if (m_tag.compare(tag) == 0) {
		return this;
	}
	for each(auto pChild in m_childs) {
		GameObject* obj = pChild->findObject(tag);
		if (obj) return obj;
	}
	return nullptr;
}

GameObject * GameObject::removeObject(const char * tag)
{
	auto it = std::find_if(m_childs.begin(), m_childs.end(), [&tag](GameObject* obj) {return obj->isTagSame(tag); });
	if (it != m_childs.end()) {
		m_childs.erase(it);
		return *it;
	}
	for each(auto pChild in m_childs) {
		GameObject* obj = pChild->removeObject(tag);
		if (obj) return obj;
	}
	return nullptr;
}

bool GameObject::deleteObject(const char * tag)
{
	auto it = std::find_if(m_childs.begin(), m_childs.end(), [&tag](GameObject* obj) {return obj->isTagSame(tag); });
	if (it != m_childs.end()) {
		m_childs.erase(it);
		delete *it;
		return true;
	}
	for each(auto pChild in m_childs) {
		if (pChild->deleteObject(tag)) return true;
	}
	return false;
}

void GameObject::clearChilds()
{
	for each(auto pChild in m_childs) {
		pChild->clearChilds();
		delete pChild;
	}
	m_childs.clear();
}

void GameObject::setAction(cAction * action)
{
	if (m_pAction) {
		delete m_pAction;
	}
	m_pAction = action;
}

bool GameObject::checkRayCollision(D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir)
{
	return m_pCollider!=NULL && m_pCollider->CheckRayCollision(rayPos, rayDir, NULL);
}

void GameObject::setPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR3(x, y, z);
	m_isDirty = true;
}

void GameObject::setPosition(const D3DXVECTOR3 & pos)
{
	setPosition(pos.x, pos.y, pos.z);
}

void GameObject::setPositionY(float y)
{
	setPosition(m_position.x, y, m_position.z);
}

void GameObject::setRotation(float angleX, float angleY, float angleZ)
{
	D3DXQuaternionRotationYawPitchRoll(&m_quaternion, angleY, angleX, angleZ);
	m_isDirty = true;
}

void GameObject::setRotation(const D3DXVECTOR3 & axis, float angle)
{
	D3DXQuaternionRotationAxis(&m_quaternion, &axis, angle);
	m_isDirty = true;
}

void GameObject::setRotation(const D3DXQUATERNION & quat)
{
	m_quaternion = quat;
	m_isDirty = true;
}

void GameObject::setSpeed(float vx, float vy, float vz)
{
	m_linearSpeed = D3DXVECTOR3(vx, vy, vz);
}

void GameObject::setLocalSpeed(float vx, float vy, float vz)
{
	m_linearSpeed = D3DXVECTOR3(vx, vy, vz);
	D3DXVec3TransformNormal(&m_linearSpeed, &m_linearSpeed, &m_matWorld);
}

D3DXVECTOR3 GameObject::getSpeed() const
{
	return m_linearSpeed;
}

void GameObject::setAngularSpeed(float vx, float vy, float vz)
{
	m_angleSpeed = D3DXVECTOR3(vx, vy, vz);
}

void GameObject::UpdatePositionWithSpeed(float dt)
{
	if (isMoving()==false) {
		return;
	}
	D3DXVECTOR3 deltaAngle = m_angleSpeed * dt;
	D3DXVECTOR3 deltaPos = m_linearSpeed * dt;
	rotate(deltaAngle.x, deltaAngle.y, deltaAngle.z);
	translate(deltaPos.x, deltaPos.y, deltaPos.z);
	m_isDirty = true;
}

bool GameObject::isMoving() const
{
	return D3DXVec3LengthSq(&m_angleSpeed) > 0.0001f || D3DXVec3LengthSq(&m_linearSpeed) > 0.0001f;
}

void GameObject::setTransform(const D3DXMATRIX * transform)
{
	m_position.x = (*transform)(3, 0);
	m_position.y = (*transform)(3, 1);
	m_position.z = (*transform)(3, 2);
	D3DXQuaternionRotationMatrix(&m_quaternion, transform);
	m_isDirty = true;
}

D3DXVECTOR3 GameObject::getWorldPosition() const
{
	return D3DXVECTOR3(m_matWorld(3, 0), m_matWorld(3, 1), m_matWorld(3, 2));
}

D3DXQUATERNION GameObject::getWorldRotation() const
{
	D3DXQUATERNION quat;
	D3DXQuaternionRotationMatrix(&quat, &m_matWorld);
	return quat;
}

void GameObject::translate(float dx, float dy, float dz)
{
	m_position = m_position + D3DXVECTOR3(dx, dy, dz);
	m_isDirty = true;
}

void GameObject::translateLocal(float dx, float dy, float dz)
{
	D3DXVECTOR3 displace(dx, dy, dz);
	D3DXMATRIX rot;
	D3DXMatrixRotationQuaternion(&rot, &m_quaternion);
	D3DXVec3TransformNormal(&displace, &displace, &rot);
	m_position = m_position + displace;
	m_isDirty = true;
}

void GameObject::rotate(float rotAngleX, float rotAngleY, float rotAngleZ)
{
	D3DXMATRIX rot;
	D3DXQUATERNION quatRot;
	D3DXQuaternionRotationYawPitchRoll(&quatRot, rotAngleY, rotAngleX, rotAngleZ);
	D3DXMatrixRotationQuaternion(&rot, &quatRot);
	D3DXVec3TransformNormal(&m_position, &m_position, &rot);
	m_quaternion = m_quaternion * quatRot;
	m_isDirty = true;
}

void GameObject::rotateLocal(float rotAngleX, float rotAngleY, float rotAngleZ)
{
	D3DXQUATERNION quatRot;
	D3DXQuaternionRotationYawPitchRoll(&quatRot, rotAngleY, rotAngleX, rotAngleZ);
	m_quaternion = m_quaternion * quatRot;
	m_isDirty = true;
}