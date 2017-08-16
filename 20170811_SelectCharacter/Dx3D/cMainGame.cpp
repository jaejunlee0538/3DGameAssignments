#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cCrtCtrl.h"
#include "cObjLoader.h"
#include "cAseLoader.h"
#include "cFrame.h"
#include "cMapLoader.h"
#include "cButton.h"
#include "cPlayer.h"
#include "TickCounter.h"
#include "cActionSeq.h"
#include "cActionMove.h"
#include "NavigationGraph.h"
#include <sstream>
#include <fstream>
#include "SphereCollider.h"
#define PLAYER_MOVE_SPEED	10.0f
namespace {
	RECT createRECT(int left, int top, int width, int height) {
		return { left, top, left + width, top + height };
	}
}

cMainGame::cMainGame(void)
	: m_pGrid(NULL)
	, m_pCamera(NULL)
	,m_connectingEdge(false),
	m_navGraph(NULL),
	m_pMeshSelectionCone(NULL)
{

}

cMainGame::~cMainGame(void)
{
	for (size_t i = 0; i < m_pPlayers.size(); ++i) {
		SAFE_RELEASE(m_pPlayers[i]);
	}
	SAFE_RELEASE(m_pMeshSelectionCone);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_navGraph);
	if (m_pFloor) {
		m_pFloor->Destroy();
		m_pFloor = nullptr;
	}
	for each(auto pObs in m_vecObstacle) {
		pObs->Destroy();
	}
	m_vecObstacle.clear();
	//전역 객체 삭제
	g_pObjectPool->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	InitPlayer();
	InitCamera();
	InitLight();
	InitGrid();
	InitFloor();
	InitObstacles();
	InitGraph();

	///////////////////////////////////////////////////////////
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	g_pD3DDevice->LightEnable(0, true);
}

void cMainGame::Update()
{
	DWORD time = g_pTimeManager->GetElapsedTime()*1000;
	g_pTimeManager->Update();
	for (size_t i = 0; i < m_vecObstacle.size(); ++i) {
		m_vecObstacle[i]->Update(time, NULL);
	}
	for each(auto player in m_pPlayers) {
		player->Update(time);
	}
	m_pCamera->Update({ 0,0,0 });

	///////////////////////////////////////////////////////////
	if (g_pKeyManager->isOnceKeyDown(VK_F6)) {
		//현재 그래프를 파일로 저장한다.
		if (m_navGraph) {
			m_navGraph->Save("NavGraph.txt");
		}
	}
	///////////////////////////////////////////////////////////
	g_pAutoReleasePool->Drain();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	for (size_t i = 0; i < m_vecObstacle.size(); ++i) {
		m_vecObstacle[i]->RenderMesh();
	}
	for each(auto player in m_pPlayers) {
		player->Render();
	}
	m_pFloor->Render();
	m_navGraph->Render();
	DrawSelectCones();
	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


bool cMainGame::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message) {
	case WM_LBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		if (m_pFloor) {
		
			D3DXVECTOR3 targetPos;
			if (GetPickingPoint(pt.x, pt.y, m_pFloor->GetVertex(), targetPos)) {
				//플레이어를 선택!
				D3DXVECTOR3 rayDir, rayPos;
				rayPos = m_pCamera->GetPosition();
				rayDir = targetPos - rayPos;
				TryPickPlayer(rayPos, rayDir);
				return true;
			}
		}
		return false;
	}
	break;
	case WM_RBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		if (m_pFloor) {
			D3DXVECTOR3 targetPos;
			if (GetPickingPoint(pt.x, pt.y, m_pFloor->GetVertex(), targetPos)) {
				//선택된 모든 플레이어에게 이동 명령을 내린다.
				moveSelectedPlayerToHere(targetPos);
				return true;
			}
		}
		return false;
	}
	break;
	default:
		break;
	}
	if (m_pCamera && m_pCamera->MsgProc(hWnd, message, wParam, lParam)) {
		return true;
	}
	return false;
}

void cMainGame::InitGraph()
{
	std::ifstream fileIn("NavGraph.txt");
	m_navGraph = new NavigationGraph();
	if (m_navGraph->Load("NavGraph.txt")) {
		m_navGraphLoaded = true;
	}
	else {
		m_navGraphLoaded = false;
	}
}

void cMainGame::InitFloor()
{
	cObjLoader loader;
	std::vector<cFrame*> objs;
	loader.Load("dijkstraResource/floor.obj", objs);
	m_pFloor = objs.front();
}

void cMainGame::InitObstacles()
{
	D3DXVECTOR3 obsPos[] = {
		{ -5.0f ,0.48f ,4.0f },
		{ 2.0f  ,0.48f ,2.0f },
		{ -1.0f ,0.48f ,-2.0f },
		{ 1.5f , 0.48f , -1.5f },
		{ 3.3f   , 0.48f , -3.9f },
		{ 5.0f  , 0.48f , -0.3f },
		{ -2.0f  , 0.48f ,  4.0f }
	};
	for (size_t i = 0; i < 7; ++i) {
		cFrame* obstacleBox;
		cObjLoader loader;
		loader.LoadMesh("dijkstraResource/ObjectBox.obj", NULL, &obstacleBox);
		obstacleBox->SetPosition(obsPos[i].x, obsPos[i].y, obsPos[i].z);
		m_vecObstacle.push_back(obstacleBox);
	}
}

void cMainGame::InitPlayer()
{
	const size_t NPlayers = 4;
	float left = -9.0f;
	float top = 9.0f;
	for(size_t i=0;i<NPlayers;++i){
		cPlayer* player = new cPlayer();
		player->Init();
		player->SetPosition({ left+i*2,0,top });
		//충돌 구체를 플레이어에게 붙인다.
		player->SetCollider(new SphereCollider(1.0, D3DXVECTOR3(0, 0.25, 0)));
		m_pPlayers.push_back(player);
	}

	//SelectionCone의 메시
	SAFE_RELEASE(m_pMeshSelectionCone);
	const float coneLength = 1.0f;
	D3DXCreateCylinder(g_pD3DDevice, 0, 0.25, coneLength, 10, 10, &m_pMeshSelectionCone, NULL);
	size_t nVertices = m_pMeshSelectionCone->GetNumVertices();
	ST_PN_VERTEX* vertex;
	D3DXMATRIX matRot;
	D3DXMatrixRotationX(&matRot, D3DXToRadian(-90));
	matRot._42 = 1.5 + 0.5*coneLength;//플레이어 머리 위에 얼마나 높이 떠있을지.
	m_pMeshSelectionCone->LockVertexBuffer(0, (LPVOID*)&vertex);
	for (size_t i = 0; i < nVertices; ++i) {
		D3DXVec3TransformCoord(&vertex[i].p, &vertex[i].p, &matRot);
		D3DXVec3TransformNormal(&vertex[i].n, &vertex[i].n, &matRot);
	}
	m_pMeshSelectionCone->UnlockVertexBuffer();

	//SelectionCone의 매터리얼
	m_mtlSelectionCone.Ambient = D3DXCOLOR(1,1,1,1);
	m_mtlSelectionCone.Diffuse = D3DXCOLOR(1, 1, 1, 1);
	m_mtlSelectionCone.Specular = D3DXCOLOR(0,0,0, 1);
	m_mtlSelectionCone.Power = 20.0f;
	m_mtlSelectionCone.Emissive = D3DXCOLOR(0, 0, 0, 1);
}

void cMainGame::InitCamera()
{
	m_pCamera = new cCamera;
	m_pCamera->Setup(D3DXVECTOR3(0,0,0));
}

void cMainGame::InitGrid()
{
	m_pGrid = new cGrid();
	m_pGrid->Setup();
	m_pGrid->SetOrigin(0, -0.2, 0);
}

void cMainGame::InitLight()
{
	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Direction = m_pCamera->GetLookDir();
	stLight.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	stLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	stLight.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	g_pD3DDevice->SetLight(0, &stLight);
}

void cMainGame::DrawSelectCones()
{
	g_pD3DDevice->SetMaterial(&m_mtlSelectionCone);
	for each(auto player in m_selectedPlayers) {
		D3DXMATRIX worldMatrix = player->GetWorldMatrix();
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &worldMatrix);
		m_pMeshSelectionCone->DrawSubset(0);
	}
}

void cMainGame::TryPickPlayer(D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir)
{
	D3DXVec3Normalize(&rayDir, &rayDir);
	for each(auto player in m_pPlayers) {
		//광선을 가지고 플레이어와 충돌체크를 수행한다.
		if (player->CheckRayCollision(rayPos, rayDir, NULL)) {
			auto it = m_selectedPlayers.find(player);
			if (it != m_selectedPlayers.end()) {
				//플레이어가 이미 선택되어 있으면 선택을 해제 시킨다.
				m_selectedPlayers.erase(it);
			}
			else {
				//플레이어가 선택되어 있지 않으면 선택한다.
				m_selectedPlayers.insert(player);
			}
			//플레이어가 겹쳐 있는 경우 하나만 선택되도록 하기 위해 break를 건다.
			break;
		}
	}
}

bool cMainGame::GetPickingPoint(int px, int py, const std::vector<ST_PNT_VERTEX>& triangles, D3DXVECTOR3 & pickingPoint)
{
	D3DXVECTOR3 rayDir;
	D3DXVECTOR3 rayPos;
	m_pCamera->GetPickingRay(px, py, rayDir, rayPos);
	float dist;
	auto& vertices = m_pFloor->GetVertex();
	for (size_t i = 0; i < vertices.size(); i += 3) {
		if (D3DXIntersectTri(&vertices[i].p, &vertices[i + 1].p, &vertices[i + 2].p, &rayPos, &rayDir, 0, 0, &dist)) {
			pickingPoint = rayPos + rayDir * dist;
			return true;
		}
	}
	return false;
}

void cMainGame::movePlayerToHere(cPlayer* pPlayer, D3DXVECTOR3 destPos)
{
	if (CheckRayCollisionWithObstaclesFromTo(pPlayer->GetPosition(), destPos) == false) {
		//현재 플레이어 위치에서 목적지가 바로 보이는 경우.
		cActionSeq* pActionMoveSeq = new cActionSeq;
		pActionMoveSeq->AddAction(CreateActionMove(pPlayer, pPlayer->GetPosition(), destPos));
		pActionMoveSeq->Start();
		pPlayer->SetAction(pActionMoveSeq);
		pActionMoveSeq->AutoRelease();
		return;
	}

	if (m_navGraph->IsEmpty())
		return;

	DijkstraPath dijkstra;
	std::vector<int> pathNodes;
	int smartStartNode = -1;
	int smartDestinationNode = -1;

	{
		//목적지에서 가장 가까운 노드를 찾는다.
		tagNavNode* destinationNode = GetNearestVisibleNavNode(destPos);
		if (destinationNode == NULL) {
			//장애물 내부를 찍은 경우!
			destinationNode = GetNearestNavNode(destPos);
			destPos = destinationNode->GetPosition();
		}

		//다익스트라 알고리즘 수행
		m_navGraph->Dijkstra(destinationNode->id, dijkstra);

		//플레이어에서 가장 가까운 노드를 찾는다.
		tagNavNode* startNode = GetNearestVisibleNavNode(pPlayer->GetPosition());
		assert(startNode);

		//플레이어가 멍청하게 움직이지 않게 하기 위해 다익스트라 결과에서 시작/끝 노드를 추려낸다.
		int currentNodeID = startNode->id;
		while (currentNodeID >= 0) {
			pathNodes.push_back(currentNodeID);
			currentNodeID = dijkstra[currentNodeID].parentID;
		}
	}

	//스마트한 시작 노드를 찾는다.
	//시작 노드에서부터 경로를 타고 올라가면서 장애물들과 충돌체크를 수행한다.
	for (int i = 1; i < pathNodes.size(); ++i) {
		if (CheckRayCollisionWithObstaclesFromTo(pPlayer->GetPosition(), dijkstra[pathNodes[i]].node->GetPosition())) {
			//충돌이 발생하면 충돌이 없었던 바로 직전 노드를 시작노드로 선택한다.
			smartStartNode = i - 1;
			break;
		}
	}
	if (smartStartNode < 0) {
		smartStartNode = pathNodes.size() - 1;
	}

	//스마트한 도착 노드를 찾는다.
	//도착 노드에서부터 경로를 거꾸로 타고 내려오면서 장애물들과 충돌체크를 수행한다.
	for (int i = pathNodes.size() - 2; i >= smartStartNode; --i) {
		if (CheckRayCollisionWithObstaclesFromTo(destPos, dijkstra[pathNodes[i]].node->GetPosition())) {
			//충돌이 발생하면 충돌이 없었던 바로 직전 노드를 도착 노드로 선택한다.
			smartDestinationNode = i + 1;
			break;
		}
	}
	if (smartDestinationNode < 0) {
		smartDestinationNode = smartStartNode;
	}

	assert(smartStartNode <= smartDestinationNode);
	//경로로 선택된 노드들을 Picked상태로 바꾼다.
	m_navGraph->ClearPickedNode();
	for (int i = smartStartNode; i <= smartDestinationNode; ++i) {
		m_navGraph->SetNodePicked(pathNodes[i]);
	}

	//다익스트라 경로를 따라서 플레이어 액션을 추가한다.
	cActionSeq* pActionMoveSeq = new cActionSeq;
	//플레이어 위치에서 시작 노드까지의 ActionMove
	pActionMoveSeq->AddAction(CreateActionMove(pPlayer, pPlayer->GetPosition(), dijkstra[pathNodes[smartStartNode]].node->GetPosition()));
	//다익스트라 경로를 따라가기 위한 ActionMove
	for (int i = smartStartNode + 1; i <= smartDestinationNode; ++i) {
		pActionMoveSeq->AddAction(CreateActionMove(pPlayer, dijkstra[pathNodes[i - 1]].node->GetPosition(), dijkstra[pathNodes[i]].node->GetPosition()));
	}
	//마지막 노드에서 목적지까지의 ActionMove
	pActionMoveSeq->AddAction(CreateActionMove(pPlayer, dijkstra[pathNodes[smartDestinationNode]].node->GetPosition(), destPos));
	//액션 시작
	pActionMoveSeq->Start();
	pPlayer->SetAction(pActionMoveSeq);
	//오토 릴리즈
	pActionMoveSeq->AutoRelease();
}

void cMainGame::moveSelectedPlayerToHere(D3DXVECTOR3 destPos)
{
	for each(auto player in m_selectedPlayers) {
		movePlayerToHere(player, destPos);
	}
}

cActionMove * cMainGame::CreateActionMove(cGameObject* target, D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	cActionMove* actionMove = new cActionMove(from, to);
	float distance = D3DXVec3Length(&(from - to));
	actionMove->SetActionTime(distance / PLAYER_MOVE_SPEED);
	actionMove->SetTarget(target);
	//오토 릴리즈
	actionMove->AutoRelease();
	return actionMove;
}

bool cMainGame::CheckRayCollisionWithObstaclesFromTo(D3DXVECTOR3 fromPos, D3DXVECTOR3 toPos, float* distance)
{
	D3DXVECTOR3 rayDir = toPos - fromPos;
	D3DXVECTOR3 rayPos = fromPos;
	float distFromTo = D3DXVec3Length(&rayDir);
	if (distFromTo < 0.001f) {
		return false;
	}
	rayDir /= distFromTo;

	for (size_t i = 0; i < m_vecObstacle.size(); ++i) {
		float collisionDist;
		if (m_vecObstacle[i]->IsCollideWithRay(rayPos, rayDir, collisionDist)) {
			if (collisionDist < distFromTo) {
				//노드와 pos사이에 장애물이 있다.
				if (distance) {
					*distance = collisionDist;
				}
				return true;
			}
		}
	}
	return false;
}

tagNavNode * cMainGame::GetNearestVisibleNavNode(D3DXVECTOR3 pos)
{
	std::vector<tagNodeWithDistance> sortedNodes;
	m_navGraph->GetSortedNodeListWithDistance(pos, sortedNodes);
	for (size_t i = 0; i < sortedNodes.size(); ++i) {
		//모든 장애물과 충돌 체크를 한다.
		if (CheckRayCollisionWithObstaclesFromTo(pos, sortedNodes[i].node->GetPosition(), NULL)==false) {
			//광선과 충돌하는 장애물이 하나도 없다!
			return sortedNodes[i].node;
		}
	}
	//위치 pos에서 보이는 노드가 하나도 없다!
	return nullptr;
}

tagNavNode* cMainGame::GetNearestNavNode(D3DXVECTOR3 pos)
{
	return m_navGraph->GetNearestNode(pos);
}
