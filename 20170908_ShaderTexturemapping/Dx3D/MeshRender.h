#pragma once
class MeshRender
{
private:
	std::string		m_texturePath;
	LPD3DXMESH		m_pMesh;
	LPD3DXEFFECT	m_pEffect;

	D3DXVECTOR3		m_vecPosition;

	D3DXHANDLE m_hTime;
	D3DXHANDLE m_hMatWorld;
	D3DXHANDLE m_hMatView;
	D3DXHANDLE m_hMatProj;
	D3DXHANDLE m_hTexture;
public:
	MeshRender();
	~MeshRender();

	bool Setup(std::string xFile, std::string texturePath, std::string effectFile);
	void Update();
	void Render();
};

