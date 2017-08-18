#include "stdafx.h"
#include "cMeshCreateHelper.h"
#include "cMtlTex.h"
#define NONE_MATERIAL_ID	-1
cMeshCreateHelper::cMeshCreateHelper()
{
}

cMeshCreateHelper::~cMeshCreateHelper()
{
	for (auto&mtl : m_materials) {
		SAFE_RELEASE(mtl.second);
	}
}

void cMeshCreateHelper::AddMaterial(std::string mtlName, cMtlTex * material)
{
	auto it = m_materials.find(mtlName);
	if (it != m_materials.end()) {
		if (material != it->second) {
			it->second->Release();
			it->second = material;
		}
	}
	else {
		m_materials[mtlName] = material;
		m_materialToSubsetID[mtlName] = m_materialToSubsetID.size();
	}
	material->AddRef();
}

void cMeshCreateHelper::AddFaces(const std::vector<ST_PNT_VERTEX>& faces, std::string mtlName)
{
	assert(faces.size() % 3 == 0 && "버텍스의 개수는 3의 배수이어야 합니다.");
	auto it = m_materialToSubsetID.find(mtlName);
	assert(it != m_materialToSubsetID.end() && "등록되지 않은 Material입니다.");
	m_vertices.insert(m_vertices.end(), faces.begin(), faces.end());
	m_subset.insert(m_subset.end(), faces.size() / 3, it->second);
}

void cMeshCreateHelper::AddFaces(const std::vector<ST_PNT_VERTEX>& faces)
{
	m_vertices.insert(m_vertices.end(), faces.begin(), faces.end());
	m_subset.insert(m_subset.end(), faces.size() / 3, NONE_MATERIAL_ID);
}

void cMeshCreateHelper::CreateMesh(ID3DXMesh ** pMesh, std::vector<cMtlTex*>& materials)
{
	assert(m_vertices.empty() == false);

	DWORD nFaces = m_vertices.size() / 3;
	ID3DXMesh* mesh;
	HRESULT result = D3DXCreateMeshFVF(
		nFaces,
		m_vertices.size(),
		D3DXMESH_MANAGED|D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&mesh);

	//버텍스 버퍼를 채운다.
	ST_PNT_VERTEX* v;
	mesh->LockVertexBuffer(0, (void**)&v);
	memcpy(v, m_vertices.data(), sizeof(ST_PNT_VERTEX)*m_vertices.size());
	mesh->UnlockVertexBuffer();

	//인덱스 버퍼를 채운다.
	int32_t *idx;
	mesh->LockIndexBuffer(0, (void**)&idx);
	for (int32_t i = 0; i < m_vertices.size(); ++i) {
		idx[i] = i;
	}
	mesh->UnlockIndexBuffer();

	//Attribute버퍼를 채운다.
	int maxSubsetID = m_materialToSubsetID.size()-1;
	bool thereIsNoneMaterialSubset = false;
	if (std::find(m_subset.begin(), m_subset.end(), NONE_MATERIAL_ID) != m_subset.end()) {
		//매터리얼을 사용하지 않는 서브셋이 있는 경우 
		maxSubsetID += 1;
		thereIsNoneMaterialSubset = true;
	}
	if (thereIsNoneMaterialSubset) {
		//매터리얼을 사용하지 않는 서브셋이 있는 경우 해당 서브셋의 ID에 가장 높은 ID를 부여한다.
		for (size_t i = 0; i < m_subset.size(); ++i) {
			if (m_subset[i] == NONE_MATERIAL_ID) {
				m_subset[i] = maxSubsetID;
			}
		}
	}

	DWORD * attr;
	mesh->LockAttributeBuffer(0, &attr);
	memcpy(attr, m_subset.data(), m_subset.size());
	mesh->UnlockAttributeBuffer();

	//최적화 수행
	std::vector<DWORD> adjacency(nFaces * 3);
	mesh->GenerateAdjacency(0.001f, adjacency.data());
	mesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		adjacency.data(),
		0, 0, 0);

	*pMesh = mesh;

	materials.resize(maxSubsetID + 1, NULL);
	for (auto& mtl : m_materials) {
		materials[m_materialToSubsetID[mtl.first]] = mtl.second;
	}
}

