#pragma once

#include "cMtlTex.h"

struct KeyFrameTranslation {
	int time;
	D3DXVECTOR3 translation;
};

struct KeyFrameRotation {
	int time;
	D3DXQUATERNION quat;
};

class cFrame
{
private:
	vector<cFrame*> m_vecChild;
	SYNTHESIZE(string, m_sFrameName, FrameName);

private:
	SYNTHESIZE_PASS_BY_REF(vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);
	SYNTHESIZE_PASS_BY_REF(std::vector<KeyFrameTranslation>, m_keyFramesTranslation, keyFramesTranslation);
	SYNTHESIZE_PASS_BY_REF(std::vector<KeyFrameRotation>, m_keyFramesRotation, keyFramesRotation);
	SYNTHESIZE(DWORD, m_animTimeOffset, AnimTimeOffset);
	SYNTHESIZE(DWORD, m_animTimeFrameLength, AnimTimeFrameLength);
	SYNTHESIZE(double, m_animTimeScale, AnimTimeScale);
	D3DXMATRIX					localmtl;
	D3DXMATRIX					mfinal;
	D3DXMATRIX*					parents;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;

	std::vector<cMtlTex*>		m_vecMaterials;
	ID3DXMesh*					m_pMesh;
public:
	cFrame(void);
	~cFrame(void);

	void RenderUP();
	void Render();
	void RenderMesh();
	void UpdateVertexBuffer();
	void SetMesh(ID3DXMesh* pMesh, std::vector<cMtlTex*> materials);
	void SetMesh(ID3DXMesh* pMesh);
	void AddChild(cFrame* pChild);
	void Destroy();
	void SetTransForm(D3DXMATRIX currentmtl);
	void SetPosition(float x, float y, float z);
	void Update(DWORD currentTime,D3DXMATRIX* parent);
	void roateLocal(D3DXVECTOR3 deltaAngle);
	void TransformVertices(D3DXMATRIX m);
private:
	void interpolateTranslation(IN DWORD time, OUT D3DXVECTOR3& xyz);
	void interpolateRotation(IN DWORD time, OUT D3DXQUATERNION& quat);
};

