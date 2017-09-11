#include "stdafx.h"
#include "GameWorld.h"
#include <fstream>
#include "BoxObject.h"
#include "cSkinnedMesh.h"
#include "cUnitZealot.h"
#include "cCamera.h"
#include "NavigationGraph.h"
#include "HPBar.h"
#include "PlayableObject.h"
#include "cKeyManager.h"
#include "CountDown.h"
#include "cActionMoveVelocity.h"
#include "cActionSeq.h"
GameWorld::GameWorld()
	:m_cellSize(1.0f), m_pNavGraph(NULL)
{
}

GameWorld::~GameWorld()
{
	for (auto it = m_pObject.begin(); it != m_pObject.end(); ++it) {
		delete *it;
	}
	m_pObject.clear();
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pGroundMesh);
	SAFE_DELETE(m_pCountDown);
	SAFE_DELETE(m_pHPBar[0]);
	SAFE_DELETE(m_pHPBar[1]);

	SAFE_DELETE(m_pNavGraph);
}

bool GameWorld::Init(const char * levelFile)
{
	std::ifstream file(levelFile);
	if (file.good() == false)
		return false;
	std::string playerClass, enemyClass;
	int NRows, NCols;
	int playerX, playerY;
	int enemyX, enemyY;
	int NBoxes;
	file >> NRows >> NCols;
	file >> playerClass >> playerX >> playerY;
	file >> enemyClass >> enemyX >> enemyY;
	file >> NBoxes;
	std::vector<std::vector<bool>> gridMap(NRows, std::vector<bool>(NCols, false));
	for (int i = 0; i < NBoxes; ++i) {
		int ix, iy;
		file >> ix >> iy;
		gridMap[iy][ix] = true;
		BoxObject* box = new BoxObject();
		box->init(D3DXVECTOR3(m_cellSize, 0.5f, m_cellSize));
		box->setPosition(GetGridCenter(ix, iy) + D3DXVECTOR3(0, 0.25f, 0));
		m_pObject.push_back(box);
	}

	cSkinnedMesh* zealotMesh = new cSkinnedMesh("Zealot/", "zealot.X");

	cUnitZealot* pZealot = new cUnitZealot(10, zealotMesh);
	pZealot->setGameWorld(this);
	pZealot->setPosition(GetGridCenter(playerX, playerY));
	pZealot->setup();
	m_pObject.push_back(pZealot);
	m_pPlayer = pZealot;

	cSkinnedMesh* zealotMesh2 = new cSkinnedMesh("Zealot/", "zealot.X");
	cUnitZealot* pZealotEnemy = new cUnitZealot(10, zealotMesh2);
	pZealotEnemy->setPosition(GetGridCenter(enemyX, enemyY));
	pZealotEnemy->setGameWorld(this);
	pZealotEnemy->setup();
	m_pObject.push_back(pZealotEnemy);
	m_pEnemy = pZealotEnemy;

	m_pCamera = new cCamera;
	m_pCamera->Setup(D3DXVECTOR3(0,0,0));

	m_mtlGround.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1);
	m_mtlGround.Diffuse = D3DXCOLOR(1, 1, 1, 1);
	m_mtlGround.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1);
	m_mtlGround.Emissive = D3DXCOLOR(0, 0, 0, 1);
	m_mtlGround.Power = 20.0f;
	D3DXCreateBox(g_pD3DDevice, NCols*m_cellSize, 0.1, NRows*m_cellSize, &m_pGroundMesh, NULL);

	BYTE* pos = NULL;
	m_pGroundMesh->LockVertexBuffer(0, (LPVOID*)&pos);
	for (int i = 0; i < m_pGroundMesh->GetNumVertices(); ++i) {
		D3DXVECTOR3* pPos = (D3DXVECTOR3*)pos;
		*pPos = *pPos + D3DXVECTOR3(0.5f*NCols*m_cellSize, -0.05, 0.5f*NRows*m_cellSize);
		pos += m_pGroundMesh->GetNumBytesPerVertex();
	}
	m_pGroundMesh->UnlockVertexBuffer();
	std::vector<DWORD> adjacency(m_pGroundMesh->GetNumFaces() * 3);
	m_pGroundMesh->GenerateAdjacency(0.001f, adjacency.data());
	m_pGroundMesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		adjacency.data(),
		0, 0, 0);

	InitNavGraph(gridMap);

	m_pHPBar[0] = new HPBar();
	m_pHPBar[0]->Init(2.0f, 0.2f, m_pPlayer->getMaxHP());
	m_pHPBar[0]->UpdatePosition(D3DXVECTOR3(0, 3, 0));
	
	m_pHPBar[1] = new HPBar();
	m_pHPBar[1]->Init(2.0f, 0.2f, m_pEnemy->getMaxHP());
	m_pHPBar[1]->UpdatePosition(D3DXVECTOR3(0, 3, 0));

	m_bGameStarted = false;

	m_pCountDown = new CountDown();
	m_pCountDown->Init(5, 100, 100, std::bind(&GameWorld::cbOnCountDownFinished, this));
	m_pCountDown->Restart();
	return true;
}

void GameWorld::Update()
{
	m_pCountDown->Update();

	for (auto it = m_pObject.begin(); it != m_pObject.end(); ++it) {
		(*it)->update(false);
	}
	m_pCamera->Update(m_pPlayer->getWorldPosition());

	m_pHPBar[0]->UpdateCurrentHP(m_pPlayer->getCurrentHP());
	m_pHPBar[0]->UpdateCameraPosition(m_pCamera->GetPosition());
	m_pHPBar[0]->UpdatePosition(m_pPlayer->getWorldPosition() + D3DXVECTOR3(0,2.0,0));

	m_pHPBar[1]->UpdateCurrentHP(m_pEnemy->getCurrentHP());
	m_pHPBar[1]->UpdateCameraPosition(m_pCamera->GetPosition());
	m_pHPBar[1]->UpdatePosition(m_pEnemy->getWorldPosition() + D3DXVECTOR3(0, 2.0, 0));
}

void GameWorld::Render()
{
	for (auto it = m_pObject.begin(); it != m_pObject.end(); ++it) {
		(*it)->render();
	}
	if (m_pGroundMesh) {
		D3DXMATRIX identity;
		D3DXMatrixIdentity(&identity);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &identity);
		g_pD3DDevice->SetMaterial(&m_mtlGround);
		m_pGroundMesh->DrawSubset(0);
	}
	if (m_pNavGraph) {
		m_pNavGraph->Render();
	}
	m_pHPBar[0]->Render();
	m_pHPBar[1]->Render();

	m_pCountDown->Render();
}

bool GameWorld::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera) {
		if (m_pCamera->MsgProc(hWnd, message, wParam, lParam)) {
			return true;
		}
	}
	switch (message) {
	case WM_LBUTTONDOWN:
	{
		if (!m_bGameStarted) {
			return false;
		}
		int x, y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		D3DXVECTOR3 rayDir, rayPos;
		BOOL hit;
		float dist, u, v;
		DWORD face, numHits;
		m_pCamera->GetPickingRay(x, y, rayDir, rayPos);
		LPD3DXBUFFER buffer = NULL;
		D3DXIntersect(m_pGroundMesh, &rayPos, &rayDir, &hit, &face, &u, &v, &dist, &buffer, &numHits);
		SAFE_RELEASE(buffer);
		if (hit) {
			D3DXVECTOR3 pickingPos = rayPos + rayDir * dist;
			auto * nodeEnd = m_pNavGraph->GetNearestNode(pickingPos);
			auto* nodeStart = m_pNavGraph->GetNearestNode(m_pPlayer->getWorldPosition());
			AStarPath path;
			m_pNavGraph->AStar(nodeStart->id, nodeEnd->id, path);
			m_pNavGraph->ClearPickedNode();
			cActionSeq* actionSeq = new cActionSeq();

			D3DXVECTOR3 prevPos = m_pPlayer->getWorldPosition();
			for (auto it = path.begin(); it != path.end(); ++it) {
				m_pNavGraph->SetNodePicked((*it)->id);
				actionSeq->AddAction(CreateVelocityAction(prevPos, (*it)->GetPosition(), 10, m_pPlayer));
				prevPos = (*it)->GetPosition();
			}
			actionSeq->AddAction(CreateVelocityAction(prevPos, pickingPos, 10, m_pPlayer));
			m_pPlayer->setAction(actionSeq);
		}
	}
		return true;
	}
	return false;
}

D3DXVECTOR3 GameWorld::GetGridCenter(int ix, int iz) const
{
	return D3DXVECTOR3(m_cellSize*(ix + 0.5f), 0.0f, m_cellSize*(iz + 0.5f));
}

void GameWorld::QueryObjectsInRadius(D3DXVECTOR3 pos, float r, std::vector<GameObject*>& objects)
{
	for each(auto obj in m_pObject) {
		float dist = D3DXVec3Length(&(obj->getWorldPosition() - pos));
		if (dist <= r) {
			objects.push_back(obj);
		}
	}
}

void GameWorld::BroadcastMessage(int msgType, std::vector<cVariant>& params)
{
	for each(auto obj in m_pObject) {
		obj->handleMessage(msgType, params);
	}
}

void GameWorld::InitNavGraph(const std::vector<std::vector<bool>>& gridMap)
{
	SAFE_DELETE(m_pNavGraph);
	std::vector<std::vector<int>> nodeIDs(gridMap.size(), std::vector<int>(gridMap[0].size(), -1));
	m_pNavGraph = new NavigationGraph();
	for (int iRow = 0; iRow < gridMap.size(); ++iRow) {
		for (int iCol = 0; iCol < gridMap[0].size(); ++iCol) {
			if (gridMap[iRow][iCol]==false) {
				D3DXVECTOR3 pos = GetGridCenter(iCol, iRow);
				int currentID = m_pNavGraph->AddNode(pos.x, pos.y, pos.z);
				int left=-1, top=-1, topLeft=-1;
				
				if (iCol >= 1) left = nodeIDs[iRow][iCol - 1];
				if (iRow >= 1) top = nodeIDs[iRow - 1][iCol];
				if (iCol >= 1 && iRow >= 1) topLeft = nodeIDs[iRow - 1][iCol - 1];
				if (left >= 0) m_pNavGraph->AddEdge(left, currentID);
				if (top >= 0) m_pNavGraph->AddEdge(top, currentID);
				if (topLeft >= 0 && left>=0 && top>=0)m_pNavGraph->AddEdge(topLeft, currentID);
				if (left >= 0 && top >= 0 && topLeft>=0) m_pNavGraph->AddEdge(left, top);
				nodeIDs[iRow][iCol] = currentID;
			}
		}
	}
	m_pNavGraph->UpdateEdgeVertexBuffer();
}

void GameWorld::cbOnCountDownFinished()
{
	m_bGameStarted = true;
}

cActionMoveVelocity * GameWorld::CreateVelocityAction(D3DXVECTOR3 from, D3DXVECTOR3 to, float speed, GameObject * target)
{
	cActionMoveVelocity * actionVelocity = new cActionMoveVelocity(from, to, speed);
	actionVelocity->SetTarget(target);
	return actionVelocity;
}
