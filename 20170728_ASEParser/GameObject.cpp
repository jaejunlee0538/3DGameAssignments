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

	void GameObject::render() const
	{
		//모든 자식을 그린다.
		for each(auto pChild in _childs) {
			pChild->render();
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

	void GameObject::calculateWorldMatrix()
	{
		D3DXMATRIX matRot;
		D3DXMATRIX matTranslation;
		D3DXMatrixTranslation(&matTranslation, _position.x, _position.y, _position.z);
		D3DXMatrixRotationYawPitchRoll(&matRot, _angles.y, _angles.x, _angles.z);
		_matWorld = matRot * _matAnim * matTranslation;
		
		if (_parent) {
			//부모가 있는 경우 부모의 world행렬을 곱한다.
			_matWorld = _matWorld * _parent->getWorldMatrix();
		}
		else {
			_matWorld = _matScale * _matWorld;
		}
	}

	void GameObject::setKeyFrameAnimation(const KeyFrameSnapshot & animMatrices)
	{
		auto it = animMatrices.find(_tag);
		if (it != animMatrices.end()) {
			setAnimationMatrix(it->second);
		}
		for each(auto pChild in _childs) {
			pChild->setKeyFrameAnimation(animMatrices);
		}
	}

	void GameObject::setAnimationMatrix(const D3DXMATRIX & matAnim)
	{
		_matAnim = matAnim;
		_isDirty = true;
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

	void GameObject::setRotation(float angleX, float angleY, float angleZ)
	{
		_angles = D3DXVECTOR3(angleX, angleY, angleZ);
		_isDirty = true;
	}

	void GameObject::setRotation(const D3DXVECTOR3 & axis, float angle)
	{
		D3DXMATRIX rotMat;
		D3DXMatrixRotationAxis(&rotMat, &axis, angle);
		assert(false);
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
		D3DXMatrixRotationYawPitchRoll(&rot, _angles.y, _angles.x, _angles.z);
		D3DXVec3TransformNormal(&displace, &displace, &rot);
		_position = _position + displace;
		_isDirty = true;
	}

	void GameObject::rotate(float rotAngleX, float rotAngleY, float rotAngleZ)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, rotAngleY, rotAngleX, rotAngleZ);
		D3DXVec3TransformNormal(&_position, &_position, &rot);
		_angles.x += rotAngleX;
		_angles.y += rotAngleY;
		_angles.z += rotAngleZ;
		_isDirty = true;
	}

	void GameObject::rotateLocal(float rotAngleX, float rotAngleY, float rotAngleZ)
	{
		_angles.x += rotAngleX;
		_angles.y += rotAngleY;
		_angles.z += rotAngleZ;
		_isDirty = true;
	}
}
