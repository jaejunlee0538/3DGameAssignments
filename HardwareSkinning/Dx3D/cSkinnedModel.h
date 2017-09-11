#pragma once
struct Bone;
class cSkinnedModel
{
public:
	cSkinnedModel();
	~cSkinnedModel();

	bool Init(const char* modelFileX);
	void Update();
	void Render(Bone* bone = NULL);

	size_t GetNumAnimations() const;
	bool GetAnimationName(int idx, std::string& name) const;
	bool SetAnimation(int idx);
private:
	bool LoadEffect(const char* effectFile);
	//
	void SetupBoneMatixPtrs(Bone* bone);
	void SetupAllAnimationNames();
private:
	Bone*						m_rootBone;
	ID3DXAnimationController *	m_pAnimControl;
	std::vector <std::string>	m_animationNames;
	LPD3DXEFFECT				m_pEffect;
};

