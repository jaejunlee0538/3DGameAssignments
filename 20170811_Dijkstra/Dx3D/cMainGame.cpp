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
	, m_pFont(NULL)
	,m_connectingEdge(false)
{

}

cMainGame::~cMainGame(void)
{
	SAFE_RELEASE(m_pPlayer);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_RELEASE(m_pFont);
	//전역 객체 삭제
	g_pObjectPool->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
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

	{
		std::ifstream fileIn("NavGraph.txt");
		if (fileIn.good()) {
			fileIn >> m_navGraph;
			m_navGraphLoaded = true;
		}
		else {
			m_navGraphLoaded = false;
		}
		m_graphRender.SetGraph(&m_navGraph);
	}
	{
		cObjLoader loader;
		std::vector<cFrame*> objs;
		loader.Load("dijkstraResource/floor.obj", objs);
		m_pFloor = objs.front();
	}

	{
		D3DXVECTOR3 obsPos[] = {
			{ -5.0f ,0.48f ,4.0f },
			{ 2.0f  ,0.48f ,2.0f },
			{ -1.0f ,0.48f ,-2.0f },
			{ 1.5f , 0.48f , -1.5f},
			{ 3.3f   , 0.48f , -3.9f },
			{ 5.0f  , 0.48f , -0.3f },
			{ -2.0f  , 0.48f ,  4.0f}
		};
		for (size_t i = 0; i < 7; ++i) {
			cFrame* obstacleBox;
			cObjLoader loader;
			loader.LoadMesh("dijkstraResource/ObjectBox.obj", NULL, &obstacleBox);
			obstacleBox->SetPosition(obsPos[i].x, obsPos[i].y, obsPos[i].z);
			m_vecObstacle.push_back(obstacleBox);
		}
	}

	//////////////////////글꼴 생성////////////////////////////
	D3DXFONT_DESC fd;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
	fd.Height = 30;
	fd.Width = 15;
	fd.Weight = FW_NORMAL;
	fd.Italic = false;
	fd.CharSet = DEFAULT_CHARSET;
	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily = FF_DONTCARE;
	//AddFontResource("umberto.ttf");
	strcpy_s(fd.FaceName, "고딕");	//글꼴 스타일
	D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);
	
	//////////////////////플레이어 생성////////////////////////////
	m_pPlayer = new cPlayer();
	m_pPlayer->Init();
	m_pPlayer->SetPosition({ -5,0,0 });
	//////////////////////그리드 셋업////////////////////////////
	m_pGrid = new cGrid();
	m_pGrid->Setup();
	m_pGrid->SetOrigin(0,-0.2,0);

	//////////////////////카메라 생성////////////////////////////
	m_pCamera = new cCamera;
	m_pCamera->Setup(m_pPlayer->GetPosition());

	///////////////////////////////////////////////////////////
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	g_pD3DDevice->LightEnable(0, true);
	UpdateLight();
}

void cMainGame::Update()
{
	static DWORD time = 0;
	time += 13;
	g_pTimeManager->Update();

	if (g_pKeyManager->isOnceKeyDown(VK_F6)) {
		std::ofstream file("NavGraph.txt");
		file << m_navGraph;
	}

	for (size_t i = 0; i < m_vecObstacle.size(); ++i) {
		m_vecObstacle[i]->Update(time, NULL);
	}
	m_pPlayer->Update(time);
	///////////////////////////////////////////////////////////
	if (m_pCamera) 
		m_pCamera->Update({ 0,0,0 });

	UpdateLight();
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
	m_graphRender.Render();
	//m_pGrid->Render();

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
			D3DXVECTOR3 targetPos;
			if (GetPickingPoint(pt.x, pt.y, m_pFloor->GetVertex(), targetPos)) {
				if (CheckRayCollisionWithObstacles(m_pCamera->GetPosition(), targetPos)) {
					tagNavNode* node = m_navGraph.GetNearestNode(targetPos);
					//targetPos = node->GetPosition();
					float distance;
					CheckRayCollisionWithObstacles( targetPos, node->GetPosition(), &distance);
					D3DXVECTOR3 targetToNode = node->GetPosition() - targetPos;
					D3DXVec3Normalize(&targetToNode, &targetToNode);
					distance += 0.1f;
					targetPos = targetPos + targetToNode* distance;
				}
				movePlayerToHere(targetPos);
			}
		}
		return false;
	}
	case WM_RBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		if (m_pFloor) {
			if (g_pKeyManager->isStayKeyDown(VK_SHIFT)) {
				//노드 추가
				D3DXVECTOR3 targetPos;
				if (GetPickingPoint(pt.x, pt.y, m_pFloor->GetVertex(), targetPos)) {
					m_navGraph.AddNode(targetPos.x, targetPos.y+0.1f, targetPos.z);
				}
			}
			else if(g_pKeyManager->isStayKeyDown(VK_CONTROL)) {
				//
				D3DXVECTOR3 rayDir, rayPos;
				int pickedNode;
				m_pCamera->GetPickingRay(pt.x, pt.y, rayDir, rayPos);
				if (m_graphRender.PickNode(rayPos, rayDir, pickedNode)) {
					m_graphRender.SetPickedNode(pickedNode);
					m_pickedNode[0] = pickedNode;
				}
				m_connectingEdge = true;
			}
		}
	}
	break;
	case WM_RBUTTONUP:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		if (m_pFloor && m_connectingEdge) {
			D3DXVECTOR3 targetPos;
			D3DXVECTOR3 rayDir, rayPos;
			int pickedNode;
			m_pCamera->GetPickingRay(pt.x, pt.y, rayDir, rayPos);
			if (m_graphRender.PickNode(rayPos, rayDir, pickedNode)) {
				m_graphRender.SetPickedNode(pickedNode);
				m_pickedNode[1] = pickedNode;
				if (m_navGraph.AddEdge(m_pickedNode[0], m_pickedNode[1])) {
					m_graphRender.UpdateEdgeVertexBuffer();
					LogDebugMessage("AddEdge %d %d\n", m_pickedNode[0], m_pickedNode[1]);
				}
			}
			m_graphRender.ClearPickedNode();
			m_connectingEdge = false;
		}
	}
	break;
	default:
		break;
	}
	return false;
}

void cMainGame::UpdateLight()
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

void cMainGame::movePlayerToHere(D3DXVECTOR3 pickingPos)
{
	if (m_navGraph.Empty())
		return;

	if (CheckRayCollisionWithObstacles(m_pPlayer->GetPosition(), pickingPos) == false) {
		cActionSeq* pActionMoveSeq = new cActionSeq;

		cActionMove* moveAction = new cActionMove(m_pPlayer->GetPosition(), pickingPos);
		moveAction->SetTarget(m_pPlayer);
		moveAction->SetActionTime(1.0f);
		pActionMoveSeq->AddAction(moveAction);
		pActionMoveSeq->Start();
		m_pPlayer->SetAction(pActionMoveSeq);
		return;
	}

	//목적지에서 가장 가까운 노드를 찾는다.
	tagNavNode* nearestNodeFromDest = GetNearestVisibleNavNode(pickingPos);
	assert(nearestNodeFromDest);
	DijkstraPath dijkstra;
	m_navGraph.Dijkstra(nearestNodeFromDest->id, dijkstra);

	//플레이어에서 가장 가까운 노드를 찾는다.
	tagNavNode* nearestNodeFromPlayer = GetNearestVisibleNavNode(m_pPlayer->GetPosition());
	assert(nearestNodeFromPlayer);

	//플레이어가 멍청하게 움직이지 않게 하기 위해 다익스트라 결과에서 시작/끝 노드를 추려낸다.

	std::vector<int> pathNodeID;
	{
		int currentNodeID = nearestNodeFromPlayer->id;
		LogDebugMessage("시작 노드(%d) - 비용(%.3f)\n", currentNodeID, dijkstra[currentNodeID].cost);
		while (currentNodeID >= 0) {
			pathNodeID.push_back(currentNodeID);
			currentNodeID = dijkstra[currentNodeID].parentID;
		}
	}

	int playerStartNodeIdx = -1;
	{
		for (int i = 1; i < pathNodeID.size(); ++i) {
			if (CheckRayCollisionWithObstacles(
				m_pPlayer->GetPosition(),
				D3DXVECTOR3(dijkstra[pathNodeID[i]].node->x,
					dijkstra[pathNodeID[i]].node->y,
					dijkstra[pathNodeID[i]].node->z))) {
				//충돌이 일어났다.
				playerStartNodeIdx = i - 1;
				break;
			}
		}
		assert(playerStartNodeIdx >= 0);
	}
	int playerLastNodeIdx = -1;
	{
		for (int i = pathNodeID.size() - 2; i >= playerStartNodeIdx; --i) {
			if (CheckRayCollisionWithObstacles(
				pickingPos,
				D3DXVECTOR3(dijkstra[pathNodeID[i]].node->x,
					dijkstra[pathNodeID[i]].node->y,
					dijkstra[pathNodeID[i]].node->z))) {
				//충돌이 일어났다.
				playerLastNodeIdx = i + 1;
				break;
			}
		}
		if (playerLastNodeIdx < 0) {
			playerLastNodeIdx = playerStartNodeIdx;
		}
	}

	{
		assert(playerStartNodeIdx <= playerLastNodeIdx);
		//다익스트라 렌더링
		m_graphRender.ClearPickedNode();
		for (int i = playerStartNodeIdx; i <= playerLastNodeIdx; ++i) {
			m_graphRender.SetPickedNode(pathNodeID[i]);
		}
		std::ostringstream oss;
		for (int i = playerStartNodeIdx; i <= playerLastNodeIdx; ++i) {
			oss << pathNodeID[i] << " ";
		}
		oss << std::endl;
		LogDebugMessage("%s", oss.str().c_str());
	}

	//다익스트라 경로를 따라서 플레이어 액션을 추가한다.
	{
		cActionSeq* pActionMoveSeq = new cActionSeq;
		pActionMoveSeq->AddAction(CreateActionMove(m_pPlayer->GetPosition(), dijkstra[pathNodeID[playerStartNodeIdx]].node->GetPosition()));
		
		for (int i = playerStartNodeIdx + 1; i <= playerLastNodeIdx; ++i) {
			pActionMoveSeq->AddAction(CreateActionMove(dijkstra[pathNodeID[i - 1]].node->GetPosition(), dijkstra[pathNodeID[i]].node->GetPosition()));
		}
		pActionMoveSeq->AddAction(CreateActionMove(dijkstra[pathNodeID[playerLastNodeIdx]].node->GetPosition(), pickingPos));
		pActionMoveSeq->Start();
		m_pPlayer->SetAction(pActionMoveSeq);
	}

	//{
	//	std::ostringstream oss;
	//	for each(auto& id in pathNodeID) {
	//		oss << id << " ";
	//	}
	//	oss << std::endl;
	//	LogDebugMessage("%s", oss.str().c_str());
	//}
}

cActionMove * cMainGame::CreateActionMove(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	cActionMove* actionMove = new cActionMove(from, to);
	float distance = D3DXVec3Length(&(from - to));
	actionMove->SetActionTime(distance / PLAYER_MOVE_SPEED);
	actionMove->SetTarget(m_pPlayer);
	return actionMove;
}

bool cMainGame::CheckRayCollisionWithObstacles(D3DXVECTOR3 fromPos, D3DXVECTOR3 toPos, float* distance)
{
	D3DXVECTOR3 rayDir = toPos - fromPos;
	D3DXVECTOR3 rayPos = fromPos;
	float distFromTo = D3DXVec3Length(&rayDir);
	D3DXVec3Normalize(&rayDir, &rayDir);

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
	SortedNodeWithDistance sortedNodes;
	m_navGraph.GenerateNodeListWithDistance(pos, sortedNodes);
	while (sortedNodes.empty() == false) {
		const tagNodeWithDistance& nodeInfo = sortedNodes.top();
		tagNavNode* node = sortedNodes.top().node;
		D3DXVECTOR3 rayDir(node->x - pos.x, node->y - pos.y, node->z - pos.z);
		D3DXVECTOR3 rayPos(pos.x, pos.y, pos.z);
		D3DXVec3Normalize(&rayDir, &rayDir);

		for (size_t i = 0; i < m_vecObstacle.size(); ++i) {
			float collisionDist;
			if (m_vecObstacle[i]->IsCollideWithRay(rayPos, rayDir, collisionDist)) {
				if (collisionDist < nodeInfo.distance) {
					//노드와 pos사이에 장애물이 있다.
					goto CheckNextNode;
				}
			}
		}
		//포문이 끝났다는 말은 pos와 노드 사이에 장애물이 없다는 뜻
		return node;
	CheckNextNode:
		//pos와 노드 사이에 장애물이 있는 경우 다음으로 가까운 노드를 체크한다.
		sortedNodes.pop();
	}
	//와일문이 끝났다는 말은 pos와 노드 사이에 장애물이 없는 경우가 없다는 뜻이다.
	return nullptr;
}
