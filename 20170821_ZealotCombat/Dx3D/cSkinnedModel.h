#pragma once
struct Bone;

class cSkinnedModel : public ID3DXAnimationCallbackHandler
{
private:
	enum CharacterStates {
		STATE_IDLE,
		STATE_ATTACK,
		STATE_RUNNING
	};
	enum {
		CURRENT_ANIM_TRACK=0,
		PREV_ANIM_TRACK=1
	};

	struct BoneWrapper :public cObject{
		BoneWrapper(Bone* bone) {
			rootBone = bone;
		}
		~BoneWrapper();
		Bone * rootBone;
	};
public:
	cSkinnedModel();
	~cSkinnedModel();

	cSkinnedModel* Clone();

	void EnableControl(bool enable) {
		m_enableControl = enable;
	}

	bool LoadFromX(const char* fileName);
	void Update();
	void Render(Bone* bone = NULL);

	size_t GetNumAnimations() const;
	void SetState(CharacterStates state);
	bool GetAnimationName(int idx, std::string& name) const;
	bool SetAnimation(int idx, float transitionTime);
	bool SetAnimation(const std::string& animName, float transitionTime);
	void SetPosition(float x, float y, float z) {
		m_position = D3DXVECTOR3(x, y, z);
	}
	void SetPosition(D3DXVECTOR3 pos) {
		m_position = pos;
	}
	void SetRotation(D3DXQUATERNION quat) {
		m_rotation = quat;
	}

	void TranslateLocal(float dx, float dy, float dz);
	void Translate(float dx, float dy, float dz);
	void RotateLocal(D3DXQUATERNION quat);

	STDMETHOD(HandleCallback)(THIS_ UINT Track, LPVOID pCallbackData) override;
private:
	void CBAnimation();
	void CBAnimationOneLoop();
	void SetupBoneMatixPtrs(Bone* bone);
	void SetupAllAnimationNames();
private:
	bool						m_enableControl;
	double						m_periodCurrentAnim;
	double						m_nextLoopCallbackTime;
	bool						m_callbackShouldBeCalled;
	double						m_callbackTime;
	CharacterStates				m_state;
	D3DXVECTOR3					m_position;

	D3DXVECTOR3					m_prevPosition;
	D3DXQUATERNION				m_rotation;

	BoneWrapper*				m_rootBone;
	ID3DXAnimationController *	m_pAnimControl;
	std::vector <std::string>	m_animationNames;
	int m_currentAnimationIdx;
};

