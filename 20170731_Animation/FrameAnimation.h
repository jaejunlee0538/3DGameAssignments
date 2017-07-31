#pragma once

namespace SGA {
	typedef std::map<std::string, D3DXMATRIX> KeyFrameSnapshot;
	
	struct KeyFrameRotation {
		float time;
		D3DXQUATERNION quat;

		KeyFrameRotation(){}
		KeyFrameRotation(float t, float rotX, float rotY, float rotZ) {
			time = t;
			D3DXQuaternionRotationYawPitchRoll(&quat, rotY, rotX, rotZ);
		}
		KeyFrameRotation(float t, const D3DXQUATERNION& quat)
			:time(t), quat(quat)
		{
		}
	};

	struct KeyFrameTranslation {
		float time;
		D3DXVECTOR3 xyz;

		KeyFrameTranslation(){}
		KeyFrameTranslation(float t, const D3DXVECTOR3& xyz)
		:time(t), xyz(xyz){

		}
		KeyFrameTranslation(float t, float x, float y, float z) {
			time = t;
			xyz.x = x;
			xyz.y = y;
			xyz.z = z;
		}
	};

	struct KeyFrameScale {
		float time;
		D3DXVECTOR3 scale;

		KeyFrameScale(){}
		KeyFrameScale(float t, float sx, float sy, float sz) {
			time = t;
			scale.x = sx;
			scale.y = sy;
			scale.z = sz;
		}
	};

	class BoneAnimation
	{
	public:
		void setBoneName(const std::string& boneName) { _boneName = boneName; }
		const std::string& getBoneName()const { return _boneName; }

		float getStartTime() const;
		float getEndTime() const;

		std::vector<KeyFrameRotation>& getFramesRotation() { return framesRotation; }
		std::vector<KeyFrameTranslation>& getFramesTranslation() { return framesTranslation; }
		std::vector<KeyFrameScale>& getFramesScale() { return framesScale; }

		void insertRotationQuaternion(float t, const D3DXQUATERNION& rotation);
		void insertRotationAxisAngle(float t, const D3DXVECTOR3& axis, float rotAngle);
		void insertTranslation(float t, const D3DXVECTOR3& translation);

		void interpolate(float t, D3DXQUATERNION& rotation, D3DXVECTOR3& translation, D3DXVECTOR3& scale) const;
		void interpolate(float t, D3DXMATRIX& transform) const;
	protected:
		void interpolateRotation(float t, D3DXQUATERNION& rotation) const;
		void interpolateTranslation(float t, D3DXVECTOR3& translation) const;
		void interpolateScale(float t, D3DXVECTOR3& scale) const;

		//Keyframes ��� ������ �ð� t�� �����ϴ� �� ���� Ű�������� ã�� �ε����� ���� Ű�������� �ε����� ��ȯ�Ѵ�.
		template <typename FramesT>
		int searchFrameTime(float t, const std::vector<FramesT>& frames) const{
			//������ ���� �ִϸ��̼� ��ȯ ����� ����Ѵ�.
			//t�� ã������ ����Ž��
			size_t idxL = 0, idxR = frames.size() - 1;
			size_t idx = frames.size() / 2;
			while ((idxR - idxL) > 1) {
				if (t > frames[idx].time) {
					idxL = idx;
				}
				else if (t < frames[idx].time) {
					idxR = idx;
				}
				else {
					break;
				}
				idx = (idxL + idxR) / 2;
			}

			//
			if (t < frames[idx].time) {
				idxL = idx - 1;
				idxR = idx;
			}
			else if (t > frames[idx].time) {
				idxL = idx;
				idxR = idx + 1;
			}
			return idxL;
		}
	private:
		std::vector<KeyFrameRotation> framesRotation;
		std::vector<KeyFrameTranslation> framesTranslation;
		std::vector<KeyFrameScale> framesScale;
		std::string _boneName;
	};

	struct AnimationClip {
		float getClipStartTime() const;
		float getClipEndtime() const;
		float calculatePhase(float t) const;
		void interpolate(float t, KeyFrameSnapshot& bonesTransform) const;

		std::vector<BoneAnimation> boneAnimations;
	};
}