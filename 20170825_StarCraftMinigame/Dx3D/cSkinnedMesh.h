#pragma once
#include "AnimCallbackKey.h"
struct ST_BONE;

class cSkinnedMesh
{
	friend class cSkinnedMeshManager;
	enum {
		CURRENT_ANIM_TRACK=0,
		PREV_ANIM_TRACK=1
	};
private:
	//하나만 생성
	ST_BONE*					m_pRootFrame;			
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;

	// 객체마다 생성
	LPD3DXANIMATIONCONTROLLER		m_pAnimController;
	std::map<std::string, int>		m_mapAnimNameToIndex;
	std::vector<AnimCallbackKey>	m_callbackKeys;
	std::function<void(int userData)> m_callbackKeyFunc;
	int									m_nextCallbackKey;
	double								m_periodCurrentAnim;

	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);

public:
	cSkinnedMesh(const char* szFolder, const char* szFilename);
	~cSkinnedMesh(void);
	
	void UpdateAndRender(const D3DXMATRIXA16* matParent);
	void SetAnimationIndex(int nIndex, const std::vector<AnimCallbackKey>& callbackKey);
	void SetAnimationByName(const std::string& name, const std::vector<AnimCallbackKey>& callbackKey);
	void SetRandomTrackPosition(); // 테스트용
	void SetAnimationCallback(std::function<void(int)> cbFunc) {
		m_callbackKeyFunc = cbFunc;
	}
private:
	void SetCurrentAnimationSet(LPD3DXANIMATIONSET animSet, const std::vector<AnimCallbackKey>& callbackKey);

	cSkinnedMesh();
	void Load(const char* szFolder, const char* szFilename);
	LPD3DXEFFECT LoadEffect(char* szFilename);
	void Update(ST_BONE* pCurrent, const D3DXMATRIXA16* pmatParent);
	void Render(ST_BONE* pBone = NULL);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
	void Destroy();
private:
	void CheckAndCallCallback(float animKeyTime);
};

