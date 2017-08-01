#include "stdafx.h"
#include "FrameAnimation.h"
#include <algorithm>


namespace SGA {
	float BoneAnimation::getStartTime() const {
		float startTime = -1.0f;
		if (framesRotation.empty() == false) {
			startTime = framesRotation.front().time;
		}
		if (framesTranslation.empty() == false) {
			if (framesTranslation.front().time < startTime) {
				startTime = framesTranslation.front().time;
			}
		}
		if (framesScale.empty() == false) {
			if (framesScale.front().time < startTime) {
				startTime = framesScale.front().time;
			}
		}
		return startTime;
	}
	float BoneAnimation::getEndTime() const {
		float endTime = -1.0f;
		if (framesRotation.empty() == false) {
			endTime = framesRotation.back().time;
		}
		if (framesTranslation.empty() == false) {
			if (framesTranslation.back().time > endTime) {
				endTime = framesTranslation.back().time;
			}
		}
		if (framesScale.empty() == false) {
			if (framesScale.back().time > endTime) {
				endTime = framesScale.back().time;
			}
		}
		return endTime;
	}

	size_t BoneAnimation::getNFramesRotation() const
	{
		return framesRotation.size();
	}

	size_t BoneAnimation::getNFramesTranslation() const
	{
		return framesTranslation.size();
	}

	size_t BoneAnimation::getNFramesScale() const
	{
		return framesScale.size();
	}

	void BoneAnimation::insertRotationQuaternion(float t, const D3DXQUATERNION & rotation)
	{
		framesRotation.emplace_back(t, rotation);
	}

	void BoneAnimation::insertRotationAxisAngle(float t, const D3DXVECTOR3 & axis, float rotAngle)
	{
		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &axis, rotAngle);
		framesRotation.emplace_back(t, quat);
	}

	void BoneAnimation::insertTranslation(float t, const D3DXVECTOR3 & translation)
	{
		framesTranslation.emplace_back(t, translation);
	}

	void BoneAnimation::interpolate(float t, D3DXQUATERNION & rotation, D3DXVECTOR3 & translation, D3DXVECTOR3 & scale) const
	{
		interpolateRotation(t, rotation);
		interpolateTranslation(t, translation);
		interpolateScale(t, scale);
	}

	void BoneAnimation::interpolate(float t, D3DXMATRIX& transform) const {
		D3DXVECTOR3 translation;
		D3DXVECTOR3 scale;
		D3DXQUATERNION quat;
		interpolate(t, quat, translation, scale);
		
		D3DXMATRIX matRot, matTrans, matScale;
		D3DXMatrixRotationQuaternion(&matRot, &quat);
		D3DXMatrixTranslation(&matTrans, translation.x, translation.y, translation.z);
		D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);
		transform = matScale * matRot * matTrans;
	}

	void BoneAnimation::interpolate(float t, KeyFrame & keyFrame) const
	{
		keyFrame.isValidRot = getNFramesRotation() > 0;
		keyFrame.isValidPos = getNFramesTranslation() > 0;
		keyFrame.isValidScale = getNFramesScale() > 0;
		interpolate(t, keyFrame.rot, keyFrame.pos, keyFrame.scale);
		keyFrame.time = t;
	}

	void BoneAnimation::interpolateRotation(float t, D3DXQUATERNION & rotation) const
	{
		if (framesRotation.empty()) {
			//회전 키프레임이 없으면 Identity를 사용한다.
			D3DXQuaternionIdentity(&rotation);
			return;
		}
		if (t < framesRotation.front().time || framesRotation.size() == 1) {
			//t가 애니메이션 시작 시간보다 이전인 경우
			//첫번째 프레임을 사용한다.
			rotation = framesRotation.front().quat;
		}
		else if (t > getEndTime()) {
			//t가 애니메이션 종료 시간을 넘어간 경우
			//마지막 프레임을 사용한다.
			rotation = framesRotation.back().quat;
		}
		else {
			int idxL = searchFrameTime(t, framesRotation);
			float phase = (t - framesRotation[idxL].time) / (framesRotation[idxL+1].time - framesRotation[idxL].time);
			D3DXQuaternionSlerp(&rotation, &(framesRotation[idxL].quat), &(framesRotation[idxL + 1].quat), phase);
		}
	}

	void BoneAnimation::interpolateTranslation(float t, D3DXVECTOR3 & translation) const
	{
		if (framesTranslation.empty()) {
			//회전 키프레임이 없으면 (0,0,0)을 사용한다.
			translation = D3DXVECTOR3(0, 0, 0);
			return;
		}
		if (t < framesTranslation.front().time || framesTranslation.size() == 1) {
			//t가 애니메이션 시작 시간보다 이전인 경우
			//첫번째 프레임을 사용한다.
			translation = framesTranslation.front().xyz;
		}
		else if (t > getEndTime()) {
			//t가 애니메이션 종료 시간을 넘어간 경우
			//마지막 프레임을 사용한다.
			translation = framesTranslation.back().xyz;
		}
		else {
			int idxL = searchFrameTime(t, framesTranslation);
			float phase = (t - framesTranslation[idxL].time) / (framesTranslation[idxL + 1].time - framesTranslation[idxL].time);
			D3DXVec3Lerp(&translation,&(framesTranslation[idxL].xyz), &(framesTranslation[idxL + 1].xyz), phase);
		}
	}

	void BoneAnimation::interpolateScale(float t, D3DXVECTOR3 & scale) const
	{
		if (framesScale.empty()) {
			//회전 키프레임이 없으면 (1, 1, 1)을 사용한다.
			scale = D3DXVECTOR3(1, 1, 1);
			return;
		}
		if (t < framesScale.front().time || framesScale.size() == 1) {
			//t가 애니메이션 시작 시간보다 이전인 경우
			//첫번째 프레임을 사용한다.
			scale = framesScale.front().scale;
		}
		else if (t > getEndTime()) {
			//t가 애니메이션 종료 시간을 넘어간 경우
			//마지막 프레임을 사용한다.
			scale = framesScale.back().scale;
		}
		else {
			int idxL = searchFrameTime(t, framesScale);
			float phase = (t - framesScale[idxL].time) / (framesScale[idxL + 1].time - framesScale[idxL].time);
			D3DXVec3Lerp(&scale, &(framesScale[idxL].scale), &(framesScale[idxL + 1].scale), phase);
		}
	}

	float AnimationClip::getClipStartTime() const
	{
		float t = FLT_MAX;
		for (auto it = boneAnimations.begin(); it != boneAnimations.end(); ++it) {
			t = std::min(t, it->getStartTime());
		}
		return t;
	}

	float AnimationClip::getClipEndtime() const
	{
		float t = FLT_MIN;
		for (auto it = boneAnimations.begin(); it != boneAnimations.end(); ++it) {
			t = std::max(t, it->getEndTime());
		}
		return t;
	}

	float AnimationClip::calculatePhase(float t) const
	{
		float tEnd = getClipEndtime();
		float tBegin = getClipStartTime();

		float phase = (t - tBegin) / (tEnd - tBegin);
		if (phase < 0)
			phase = 0;
		if (phase > 1.0f)
			phase = 1.0f;
		return phase;
	}

	void AnimationClip::interpolate(float t, AnimationSnapshots& bonesTransform) const
	{
		KeyFrame keyFrame;
		for (auto it = boneAnimations.begin(); it != boneAnimations.end(); ++it) {
			it->interpolate(t, keyFrame);
			bonesTransform[it->getBoneName()] = keyFrame;
		}
	}
}
