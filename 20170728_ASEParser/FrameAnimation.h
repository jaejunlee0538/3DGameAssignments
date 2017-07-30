#pragma once

namespace SGA {
	typedef std::map<std::string, D3DXMATRIX> KeyFrameSnapshot;
	

	struct KeyFrame {
		float time;
		D3DXVECTOR3 xyz;
		D3DXQUATERNION quat;
		D3DXVECTOR3 scale;

		KeyFrame(float t, float px, float py, float pz, float rotX, float rotY, float rotZ) {
			time = t;
			xyz.x = px; xyz.y = py; xyz.z = pz;
			D3DXQuaternionRotationYawPitchRoll(&quat, rotY, rotX, rotZ);
			scale.x = scale.y = scale.z = 1;
		}
		KeyFrame(float t, const D3DXVECTOR3& pos, const D3DXQUATERNION& quat) 
			:time(t), xyz(pos), quat(quat)
		{
			scale.x = scale.y = scale.z = 1;
		}
	};

	typedef std::vector<KeyFrame> KeyFrames;

	struct BoneAnimation
	{
		float getStartTime() const {
			assert(keyFrames.empty() == false);
			return keyFrames.front().time;
		}

		float getEndTime() const {
			assert(keyFrames.empty() == false);
			return keyFrames.back().time;
		}

		void interpolate(float t, D3DXMATRIX& mat) const;
		
		KeyFrames keyFrames;
		std::string boneName;
	};

	struct AnimationClip {
		float getClipStartTime() const;
		float getClipEndtime() const;
		float calculatePhase(float t) const;
		void interpolate(float t, KeyFrameSnapshot& bonesTransform) const;

		std::vector<BoneAnimation> boneAnimations;
	};
}