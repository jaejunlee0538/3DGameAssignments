#pragma once
class cMtlTex;
class cMeshCreateHelper
{
public:
	cMeshCreateHelper();
	~cMeshCreateHelper();

	//mtlName 매터리얼을 사용하는 face를 메시에 추가한다.
	void AddFaces(const std::vector<ST_PNT_VERTEX>& faces, std::string mtlName);
	//매터리얼이 없는 face를 메시에 추가한다.
	void AddFaces(const std::vector<ST_PNT_VERTEX>& faces);
	//매터리얼을 추가
	void AddMaterial(std::string mtlName, cMtlTex* material);
	//매시를 생성하고 매터리얼 목록을 반환한다.
	//매터리얼이 없는 서브셋은 가장 마지막 ID를 할당받고 그 ID의 매터리얼의 포인터는 NULL로 되어있다.
	void CreateMesh(ID3DXMesh** pMesh, std::vector<cMtlTex*>& materials);
private:
	std::map<std::string, cMtlTex*>		m_materials;
	std::map<std::string, int>			m_materialToSubsetID;
	std::vector<DWORD>					m_subset;
	std::vector<ST_PNT_VERTEX>			m_vertices;
};

