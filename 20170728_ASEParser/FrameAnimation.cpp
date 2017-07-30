#include "stdafx.h"
#include "FrameAnimation.h"
#include <algorithm>


namespace SGA {
	void BoneAnimation::interpolate(float t, D3DXMATRIX & mat) const {
		assert(keyFrames.empty() == false);
		if (t < getStartTime() || keyFrames.size() == 1) {
			//t가 애니메이션 시작 시간보다 이전인 경우
			//첫번째 프레임을 사용한다.
			auto& firstFrame = keyFrames.front();
			D3DXMatrixTransformation(&mat, NULL, NULL, &firstFrame.scale, NULL, &firstFrame.quat, &firstFrame.xyz);
		}
		else if (t > getEndTime()) {
			//t가 애니메이션 종료 시간을 넘어간 경우
			//마지막 프레임을 사용한다.
			auto& endFrmae = keyFrames.back();
			D3DXMatrixTransformation(&mat, NULL, NULL, &endFrmae.scale, NULL, &endFrmae.quat, &endFrmae.xyz);
		}
		else {
			//보간을 통해 애니메이션 변환 행렬을 계산한다.
			//t를 찾기위한 이진탐색
			size_t idxL = 0, idxR = keyFrames.size() - 1;
			size_t idx = keyFrames.size() / 2;
			while ((idxR - idxL) > 1) {
				if (t > keyFrames[idx].time) {
					idxL = idx;
				}
				else if(t < keyFrames[idx].time){
					idxR = idx;
				}
				else {
					break;
				}
				idx = (idxL + idxR) / 2;
			}
			
			//
			if (t < keyFrames[idx].time) {
				idxL = idx - 1;
				idxR = idx;
			}
			else if (t > keyFrames[idx].time) {
				idxL = idx;
				idxR = idx + 1;
			}

			//Interpolation
			float phase = (t - keyFrames[idxL].time) / (keyFrames[idxR].time - keyFrames[idxL].time);
			D3DXVECTOR3 xyzLerp;
			D3DXVECTOR3 scaleLerp;
			D3DXQUATERNION quatLerp;
			D3DXVec3Lerp(&xyzLerp,&keyFrames[idxL].xyz,&keyFrames[idxR].xyz, phase);
			D3DXVec3Lerp(&scaleLerp, &keyFrames[idxL].scale, &keyFrames[idxR].scale, phase);
			D3DXQuaternionSlerp(&quatLerp, &keyFrames[idxL].quat, &keyFrames[idxR].quat, phase);
			D3DXMatrixTransformation(&mat, NULL, NULL, &scaleLerp, NULL, &quatLerp, &xyzLerp);
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

	void AnimationClip::interpolate(float t, KeyFrameSnapshot& bonesTransform) const
	{
		D3DXMATRIX mat;
		for (auto it = boneAnimations.begin(); it != boneAnimations.end(); ++it) {
			it->interpolate(t, mat);
			bonesTransform[it->boneName] = mat;
		}
	}
}
