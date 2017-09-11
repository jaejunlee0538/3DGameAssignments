#pragma once

struct ST_BONE;

class cSkinnedMesh
{
	friend class cSkinnedMeshManager;

private:
	//�ϳ��� ����
	ST_BONE*					m_pRootFrame;			
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;

	// ��ü���� ����
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);

public:
	cSkinnedMesh(const char* szFolder, const char* szFilename);
	~cSkinnedMesh(void);
	
	void UpdateAndRender(const D3DXMATRIXA16* matParent);
	void SetAnimationIndex(int nIndex);

	void SetRandomTrackPosition(); // �׽�Ʈ��
private:
	cSkinnedMesh();
	void Load(const char* szFolder, const char* szFilename);
	LPD3DXEFFECT LoadEffect(char* szFilename);
	void Update(ST_BONE* pCurrent, const D3DXMATRIXA16* pmatParent);
	void Render(ST_BONE* pBone = NULL);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
	void Destroy();
};

