#include "stdafx.h"
#include "GameObject.h"

namespace SGA {
	GameObject::GameObject()
	{
		D3DXMatrixIdentity(&_matAnim);
	}

	GameObject::~GameObject()
	{
		clearChilds();
	}

	void GameObject::render() const
	{
		GameObject* child = _headChilds;
		while (child) {
			child->render();
			child = child->_nextSibling;
		}
	}

	void GameObject::update(bool isDirty)
	{
		isDirty |= _isDirty;
		if (isDirty) {
			calculateWorldMatrix();
			_isDirty = false;
		}

		GameObject* child = _headChilds;
		while (child) {
			child->update(isDirty);
			child = child->_nextSibling;
		}
	}

	void GameObject::setTag(const TCHAR * tag)
	{
		_tag = tag;
	}

	const tstring & GameObject::getTag() const
	{
		return _tag.c_str();
	}

	bool GameObject::isTagSame(const TCHAR * tag) const
	{
		return _tag.compare(tag) == 0;
	}

	void GameObject::calculateWorldMatrix()
	{
		D3DXMATRIX matRot;
		D3DXMATRIX matTranslation;
		D3DXMatrixTranslation(&matTranslation, _position.x, _position.y, _position.z);
		D3DXMatrixRotationYawPitchRoll(&matRot, _angles.y, _angles.x, _angles.z);
		_matWorld = matRot * _matAnim * matTranslation;
		if (_parent) {
			//�θ� �ִ� ��� �θ��� world����� ���Ѵ�.
			_matWorld = _matWorld * _parent->getWorldMatrix();
		}
	}

	void GameObject::setKeyFrameAnimation(const KeyFrameSnapshot & animMatrices)
	{
		auto it = animMatrices.find(_tag);
		if (it != animMatrices.end()) {
			setAnimationMatrix(it->second);
		}
		GameObject* child = _headChilds;
		while (child) {
			child->setKeyFrameAnimation(animMatrices);
			child = child->_nextSibling;
		}
	}

	void GameObject::setAnimationMatrix(const D3DXMATRIX & matAnim)
	{
		_matAnim = matAnim;
		_isDirty = true;
	}

	void GameObject::insertChild(GameObject * child)
	{
		assert(child != nullptr);
		//head�� ���ο� �ڽ��� �����Ѵ�.
		GameObject* tmp = _headChilds;
		_headChilds = child;
		_headChilds->_nextSibling = tmp;
		child->_isDirty = true;
		child->setParent(this);
	}

	void GameObject::setParent(GameObject * parent)
	{
		_parent = parent;
	}

	GameObject * GameObject::findObject(const TCHAR * tag)
	{
		if (_tag.compare(tag) == 0) {
			return this;
		}
		GameObject* child = _headChilds;
		while (child) {
			GameObject* obj = child->findObject(tag);
			if (obj) return obj;
			child = child->_nextSibling; //
		}
		return nullptr;
	}

	GameObject * GameObject::removeObject(const TCHAR * tag)
	{
		GameObject* child = _headChilds;
		GameObject* prevChild = nullptr;
		while (child) {
			if (child->isTagSame(tag)) {
				//���� �ڽ��� tag�� ���� ������Ʈ�� ���� �ڽ��� ���Ḯ��Ʈ���� �����Ѵ�.
				if (child == _headChilds) {
					_headChilds = child->_nextSibling;
				}
				else {
					prevChild->_nextSibling = child->_nextSibling;
				}
				child->_parent = nullptr;
				child->_nextSibling = nullptr;
				return child;
			}

			//���� �ڽ��� �ڽĵ� �߿� tag�� ���� ������Ʈ�� ã�Ƽ� �����Ѵ�.
			GameObject* obj = child->removeObject(tag);
			if (obj) return obj;

			//���� �ڽ�
			child = child->_nextSibling;
		}
		return nullptr;
	}

	bool GameObject::deleteObject(const TCHAR * tag)
	{
		GameObject* child = _headChilds;
		GameObject* prevChild = nullptr;
		while (child) {
			if (child->isTagSame(tag)) {
				//���� �ڽ��� tag�� ���� ������Ʈ�� ���� �ڽ��� ���Ḯ��Ʈ���� �����Ѵ�.
				if (child == _headChilds) {
					_headChilds = child->_nextSibling;
				}
				else {
					prevChild->_nextSibling = child->_nextSibling;
				}
				//���� �ڽ��� �޸𸮿��� �����Ѵ�.
				delete child;
				return true;
			}

			//���� �ڽ��� �ڽĵ� �߿� tag�� ���� ������Ʈ�� ã�Ƽ� �����Ѵ�.
			if (child->removeObject(tag)) return true;

			//���� �ڽ�
			child = child->_nextSibling;
		}
		return false;
	}

	void GameObject::clearChilds()
	{
		if (_headChilds) {
			GameObject* child = _headChilds;
			while (child) {
				child->clearChilds();
				GameObject* tmp = child->_nextSibling;
				delete child;
				child = tmp;
			}
			_headChilds = nullptr;
		}
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
