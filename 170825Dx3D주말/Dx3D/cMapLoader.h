#pragma once

class cMtlTex;

enum GridObjectTypes {
	OBJ_NONE = -1,
	OBJ_FLOOR = 1,
	OBJ_WALL = 2
};
class cMapLoader
{
private:

	vector<vector<int>>		m_vecMap;
	LPD3DXMESH				m_pMeshFloor;
	LPD3DXMESH				m_pMeshWall;
	vector<cMtlTex*>		m_vecMtlFloor;
	vector<cMtlTex*>		m_vecMtlWall;
public:

	cMapLoader();
	~cMapLoader();


	void Setup();
	void MapLoad(IN string sPath);
	void Render();

};

