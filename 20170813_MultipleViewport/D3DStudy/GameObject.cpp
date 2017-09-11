#include "stdafx.h"
#include "GameObject.h"
#include <algorithm>
namespace SGA {
	GameObject::GameObject()
	{
		D3DXMatrixIdentity(&_matAnim);
		setScale(1, 1, 1);
		setPosition(0, 0, 0);
		setRotation(0, 0, 0);
	}

	GameObject::~GameObject()
	{
		clearChilds();
	}

	void GameObject::render(LPDIRECT3DDEVICE9 device) const
	{
		//모든 자식을 그린다.
		for each(auto pChild in _childs) {
			pChild->render(device);
		}
	}

	void GameObject::update(bool isDirty)
	{
		isDirty |= _isDirty;
		if (isDirty) {
			calculateWorldMatrix();
			_isDirty = false;
		}

		//모든 자식을 업데이트한다.
		for each(auto pChild in _childs) {
			pChild->update(isDirty);
		}
	}

	void GameObject::setTag(const char * tag)
	{
		_tag = tag;
	}

	const std::string & GameObject::getTag() const
	{
		return _tag;
	}

	bool GameObject::isTagSame(const char * tag) const
	{
		return _tag.compare(tag) == 0;
	}

	void GameObject::setScale(float sx, float sy, float sz)
	{
		D3DXMatrixScaling(&_matScale, sx, sy, sz);
	}

	void GameObject::setScale(const D3DXVECTOR3 & scale)
	{
		setScale(scale.x, scale.y, scale.z);
	}

	void GameObject::calculateWorldMatrix()
	{
		D3DXMATRIX matRot;
		D3DXMATRIX matTranslation;
		D3DXMatrixTranslation(&matTranslation, _position.x, _position.y, _position.z);
		D3DXMatrixRotationQuaternion(&matRot, &_quaternion);
		_matWorld = matRot *  matTranslation;
		
		if (_parent) {
			//부모가 있는 경우 부모의 world행렬을 곱한다.
			_matWorld = _matWorld * _parent->getWorldMatrix();
		}
		else {
			_matWorld = _matScale * _matWorld;
		}
	}

	void GameObject::setKeyFrameAnimation(const AnimationSnapshots & animMatrices)
	{
		auto it = animMatrices.find(_tag);
		if (it != animMatrices.end()) {
			if(it->second.hasScale())  
				setScale(it->second.scale);
			if(it->second.hasPosition()) 
				setPosition(it->second.pos);
			if(it->second.hasRotation()) 
				setRotation(it->second.rot);
		}
		for each(auto pChild in _childs) {
			pChild->setKeyFrameAnimation(animMatrices);
		}
	}

	size_t GameObject::getNObjects() const
	{
		size_t count = 1;
		for each(auto pChild in _childs) {
			count += pChild->getNObjects();
		}
		return count;
	}

	bool GameObject::hasParent() const
	{
		return _parent != nullptr;
	}

	bool GameObject::hasChild() const
	{
		return _childs.empty() == false;
	}

	void GameObject::insertChild(GameObject * child)
	{
		assert(child != nullptr);
		//head에 새로운 자식을 삽입한다.
		_childs.push_front(child);
		child->_isDirty = true;
		child->_parent = this;
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
		if (_tag.compare(tag) == 0) {
			return this;
		}
		for each(auto pChild in _childs) {
			GameObject* obj = pChild->findObject(tag);
			if (obj) return obj;
		}
		return nullptr;
	}

	GameObject * GameObject::removeObject(const char * tag)
	{
		auto it = std::find_if(_childs.begin(), _childs.end(), [&tag](GameObject* obj) {return obj->isTagSame(tag); });
		if (it != _childs.end()) {
			_childs.erase(it);
			return *it;
		}
		for each(auto pChild in _childs) {
			GameObject* obj = pChild->removeObject(tag);
			if (obj) return obj;
		}
		return nullptr;
	}

	bool GameObject::deleteObject(const char * tag)
	{
		auto it = std::find_if(_childs.begin(), _childs.end(), [&tag](GameObject* obj) {return obj->isTagSame(tag); });
		if (it != _childs.end()) {
			_childs.erase(it);
			delete *it;
			return true;
		}
		for each(auto pChild in _childs) {
			if (pChild->deleteObject(tag)) return true;
		}
		return false;
	}

	void GameObject::clearChilds()
	{
		for each(auto pChild in _childs) {
			pChild->clearChilds();
			delete pChild;
		}
		_childs.clear();
	}

	void GameObject::setPosition(float x, float y, float z)
	{
		_position = D3DXVECTOR3(x, y, z);
		_isDirty = true;
	}

	void GameObject::setPosition(const D3DXVECTOR3 & pos)
	{
		setPosition(pos.x, pos.y, pos.z);
	}

	void GameObject::setRotation(float angleX, float angleY, float angleZ)
	{
		D3DXQuaternionRotationYawPitchRoll(&_quaternion, angleY, angleX, angleZ);
		_isDirty = true;
	}

	void GameObject::setRotation(const D3DXVECTOR3 & axis, float angle)
	{
		D3DXQuaternionRotationAxis(&_quaternion, &axis, angle);
		_isDirty = true;
	}

	void GameObject::setRotation(const D3DXQUATERNION & quat)
	{
		_quaternion = quat;
		_isDirty = true;
	}

	void GameObject::setTransform(const D3DXMATRIX * transform)
	{
		_position.x = (*transform)(3,0);
		_position.y = (*transform)(3,1);
		_position.z = (*transform)(3,2);
		D3DXQuaternionRotationMatrix(&_quaternion, transform);
		_isDirty = true;
	}

	D3DXVECTOR3 GameObject::getWorldPosition() const
	{
		return D3DXVECTOR3(_matWorld(3,0),_matWorld(3,1),_matWorld(3,2));
	}

	D3DXQUATERNION GameObject::getWorldRotation() const
	{
		D3DXQUATERNION quat;
		D3DXQuaternionRotationMatrix(&quat, &_matWorld);
		return quat;
	}

	void GameObject::translate(float dx, float dy, float dz)
	{
		_position = _position + D3DXVECTOR3(dx, dy, dz);
		_isDirty = true;
	}

	void GameObject::translateLocal(float dx, float dy, float dz)
	{
		D3DXVECTOR3 displace(dx, dy, dz);
		D3DXMATRIX rot;
		D3DXMatrixRotationQuaternion(&rot, &_quaternion);
		D3DXVec3TransformNormal(&displace, &displace, &rot);
		_position = _position + displace;
		_isDirty = true;
	}

	void GameObject::rotate(float rotAngleX, float rotAngleY, float rotAngleZ)
	{
		D3DXMATRIX rot;
		D3DXQUATERNION quatRot;
		D3DXQuaternionRotationYawPitchRoll(&quatRot, rotAngleY, rotAngleX, rotAngleZ);
		D3DXMatrixRotationQuaternion(&rot, &quatRot);
		D3DXVec3TransformNormal(&_position, &_position, &rot);
		_quaternion = _quaternion * quatRot;
		_isDirty = true;
	}

	void GameObject::rotateLocal(float rotAngleX, float rotAngleY, float rotAngleZ)
	{
		D3DXQUATERNION quatRot;
		D3DXQuaternionRotationYawPitchRoll(&quatRot, rotAngleY, rotAngleX, rotAngleZ);
		_quaternion = _quaternion * quatRot;

		_isDirty = true;
	}
}
