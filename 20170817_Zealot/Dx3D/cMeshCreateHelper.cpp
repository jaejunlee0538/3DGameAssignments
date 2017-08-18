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
	assert(faces.size() % 3 == 0 && "���ؽ��� ������ 3�� ����̾�� �մϴ�.");
	auto it = m_materialToSubsetID.find(mtlName);
	assert(it != m_materialToSubsetID.end() && "��ϵ��� ���� Material�Դϴ�.");
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

	//���ؽ� ���۸� ä���.
	ST_PNT_VERTEX* v;
	mesh->LockVertexBuffer(0, (void**)&v);
	memcpy(v, m_vertices.data(), sizeof(ST_PNT_VERTEX)*m_vertices.size());
	mesh->UnlockVertexBuffer();

	//�ε��� ���۸� ä���.
	int32_t *idx;
	mesh->LockIndexBuffer(0, (void**)&idx);
	for (int32_t i = 0; i < m_vertices.size(); ++i) {
		idx[i] = i;
	}
	mesh->UnlockIndexBuffer();

	//Attribute���۸� ä���.
	int maxSubsetID = m_materialToSubsetID.size()-1;
	bool thereIsNoneMaterialSubset = false;
	if (std::find(m_subset.begin(), m_subset.end(), NONE_MATERIAL_ID) != m_subset.end()) {
		//���͸����� ������� �ʴ� ������� �ִ� ��� 
		maxSubsetID += 1;
		thereIsNoneMaterialSubset = true;
	}
	if (thereIsNoneMaterialSubset) {
		//���͸����� ������� �ʴ� ������� �ִ� ��� �ش� ������� ID�� ���� ���� ID�� �ο��Ѵ�.
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

	//����ȭ ����
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

