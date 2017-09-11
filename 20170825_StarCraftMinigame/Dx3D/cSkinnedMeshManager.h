#pragma once

#define g_pSkinnedMeshManager cSkinnedMeshManager::GetInstance()

class cSkinnedMesh;

class cSkinnedMeshManager
{
private:
	std::map<std::string, cSkinnedMesh*> m_mapSkinnedMesh;

public:
	SINGLETON(cSkinnedMeshManager);

	cSkinnedMesh* GetSkinnedMesh(const char* szFolder, const char* szFilename);
	
	void Destroy();
};

