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
#define PLAYER_MOVE_SPEED	5.0f
namespace {
	RECT createRECT(int left, int top, int width, int height) {
		return { left, top, left + width, top + height };
	}
}

cMainGame::cMainGame(void)
	: m_pGrid(NULL)
	, m_pCamera(NULL)
	, m_pPlayer(NULL)
	,m_connectingEdge(false),
	m_navGraph(NULL),
	m_time(0)
{

}

cMainGame::~cMainGame(void)
{
	SAFE_RELEASE(m_pPlayer);
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
	m_time = 0;
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
	m_time += 13;
	g_pTimeManager->Update();
	for (size_t i = 0; i < m_vecObstacle.size(); ++i) {
		m_vecObstacle[i]->Update(m_time, NULL);
	}
	m_pPlayer->Update(m_time);
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
	m_pFloor->Render();
	m_pPlayer->Render();
	m_navGraph->Render();

	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


bool cMainGame::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera && m_pCamera->MsgProc(hWnd, message, wParam, lParam)){
		return true;
	}
	switch (message) {
	case WM_LBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		if (m_pFloor && g_pKeyManager->isStayKeyDown(VK_CONTROL)) {
			//플레이어에게 이동 명령을 내린다.
			D3DXVECTOR3 targetPos;
			if (GetPickingPoint(pt.x, pt.y, m_pFloor->GetVertex(), targetPos)) {
				movePlayerToHere(targetPos);
			}
		}
		return false;
	}
	case WM_RBUTTONDOWN:
	{
		//그래프 에디터를 위한 부분!
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		if (m_pFloor) {
			if (g_pKeyManager->isStayKeyDown(VK_SHIFT)) {
				//노드 추가
				D3DXVECTOR3 targetPos;
				if (GetPickingPoint(pt.x, pt.y, m_pFloor->GetVertex(), targetPos)) {
					m_navGraph->AddNode(targetPos.x, targetPos.y+0.1f, targetPos.z);
				}
			}
			else if(g_pKeyManager->isStayKeyDown(VK_CONTROL)) {
				//
				D3DXVECTOR3 rayDir, rayPos;
				int pickedNode;
				m_pCamera->GetPickingRay(pt.x, pt.y, rayDir, rayPos);
				if (m_navGraph->CheckPicking(rayPos, rayDir, pickedNode)) {
					m_navGraph->SetNodePicked(pickedNode);
					m_pickedNode[0] = pickedNode;
				}
				m_connectingEdge = true;
			}
		}
	}
	break;
	case WM_RBUTTONUP:
	{
		//그래프 에디터를 위한 부분!
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		if (m_pFloor && m_connectingEdge) {
			D3DXVECTOR3 targetPos;
			D3DXVECTOR3 rayDir, rayPos;
			int pickedNode;
			m_pCamera->GetPickingRay(pt.x, pt.y, rayDir, rayPos);
			if (m_navGraph->CheckPicking(rayPos, rayDir, pickedNode)) {
				m_navGraph->SetNodePicked(pickedNode);
				m_pickedNode[1] = pickedNode;
				if (m_navGraph->AddEdge(m_pickedNode[0], m_pickedNode[1])) {
					m_navGraph->UpdateEdgeVertexBuffer();
					LogDebugMessage("AddEdge %d %d\n", m_pickedNode[0], m_pickedNode[1]);
				}
			}
			m_navGraph->ClearPickedNode();
			m_connectingEdge = false;
		}
	}
	break;
	default:
		break;
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
	{
		////////////////////////다익스트라 테스트////////////////////////////
		//NavigationGraph navGraph;
		//for (size_t i = 0; i < 8; ++i) {
		//	navGraph.AddNode(0, 0, 0);
		//}
		//navGraph.AddEdge(0, 1, 2);
		//navGraph.AddEdge(0, 5, 3);
		//navGraph.AddEdge(1, 2, 4);
		//navGraph.AddEdge(1, 3, 1);
		//navGraph.AddEdge(2, 4, 2);
		//navGraph.AddEdge(3, 4, 3);
		//navGraph.AddEdge(3, 6, 2);
		//navGraph.AddEdge(4, 7, 4);
		//navGraph.AddEdge(5, 6, 6);
		//navGraph.AddEdge(6, 7, 4);

		//DijkstraPath path;
		//navGraph.Dijkstra(5, path);

		//for (size_t i = 0; i < path.size(); ++i) {
		//	std::ostringstream oss;
		//	oss << i;
		//	int nodeIdx = path[i].parentID;
		//	while (nodeIdx >= 0) {
		//		oss << "=>" << nodeIdx;
		//		nodeIdx = path[nodeIdx].parentID;
		//	}
		//	oss << " (Cost : " << path[i].cost << ")" << std::endl;
		//	LogDebugMessage("%s", oss.str().c_str());
		//}
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
	m_pPlayer = new cPlayer();
	m_pPlayer->Init();
	m_pPlayer->SetPosition({ -5,0,0 });
}

void cMainGame::InitCamera()
{
	m_pCamera = new cCamera;
	m_pCamera->Setup(m_pPlayer->GetPosition());
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

void cMainGame::movePlayerToHere(D3DXVECTOR3 destPos)
{
	if (CheckRayCollisionWithObstaclesFromTo(m_pPlayer->GetPosition(), destPos) == false) {
		//현재 플레이어 위치에서 목적지가 바로 보이는 경우.
		cActionSeq* pActionMoveSeq = new cActionSeq;
		pActionMoveSeq->AddAction(CreateActionMove(m_pPlayer->GetPosition(), destPos));
		pActionMoveSeq->Start();
		m_pPlayer->SetAction(pActionMoveSeq);
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
		tagNavNode* startNode = GetNearestVisibleNavNode(m_pPlayer->GetPosition());
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
		if (CheckRayCollisionWithObstaclesFromTo(m_pPlayer->GetPosition(), dijkstra[pathNodes[i]].node->GetPosition())) {
			//충돌이 발생하면 충돌이 없었던 바로 직전 노드를 시작노드로 선택한다.
			smartStartNode = i - 1;
			break;
		}
	}
	if (smartStartNode < 0) {
		smartStartNode = pathNodes.back();
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
	pActionMoveSeq->AddAction(CreateActionMove(m_pPlayer->GetPosition(), dijkstra[pathNodes[smartStartNode]].node->GetPosition()));
	//다익스트라 경로를 따라가기 위한 ActionMove
	for (int i = smartStartNode + 1; i <= smartDestinationNode; ++i) {
		pActionMoveSeq->AddAction(CreateActionMove(dijkstra[pathNodes[i - 1]].node->GetPosition(), dijkstra[pathNodes[i]].node->GetPosition()));
	}
	//마지막 노드에서 목적지까지의 ActionMove
	pActionMoveSeq->AddAction(CreateActionMove(dijkstra[pathNodes[smartDestinationNode]].node->GetPosition(), destPos));
	//액션 시작
	pActionMoveSeq->Start();
	m_pPlayer->SetAction(pActionMoveSeq);
	//오토 릴리즈
	pActionMoveSeq->AutoRelease();
}

cActionMove * cMainGame::CreateActionMove(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	cActionMove* actionMove = new cActionMove(from, to);
	float distance = D3DXVec3Length(&(from - to));
	actionMove->SetActionTime(distance / PLAYER_MOVE_SPEED);
	actionMove->SetTarget(m_pPlayer);
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
