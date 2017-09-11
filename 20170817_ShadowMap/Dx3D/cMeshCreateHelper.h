#pragma once
class cMtlTex;
class cMeshCreateHelper
{
public:
	cMeshCreateHelper();
	~cMeshCreateHelper();

	//mtlName ���͸����� ����ϴ� face�� �޽ÿ� �߰��Ѵ�.
	void AddFaces(const std::vector<ST_PNT_VERTEX>& faces, std::string mtlName);
	//���͸����� ���� face�� �޽ÿ� �߰��Ѵ�.
	void AddFaces(const std::vector<ST_PNT_VERTEX>& faces);
	//���͸����� �߰�
	void AddMaterial(std::string mtlName, cMtlTex* material);
	//�Žø� �����ϰ� ���͸��� ����� ��ȯ�Ѵ�.
	//���͸����� ���� ������� ���� ������ ID�� �Ҵ�ް� �� ID�� ���͸����� �����ʹ� NULL�� �Ǿ��ִ�.
	void CreateMesh(ID3DXMesh** pMesh, std::vector<cMtlTex*>& materials);
private:
	std::map<std::string, cMtlTex*>		m_materials;
	std::map<std::string, int>			m_materialToSubsetID;
	std::vector<DWORD>					m_subset;
	std::vector<ST_PNT_VERTEX>			m_vertices;
};

