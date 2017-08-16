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
	//���� ��ü ����
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
		//���� �׷����� ���Ϸ� �����Ѵ�.
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
			//�÷��̾�� �̵� ����� ������.
			D3DXVECTOR3 targetPos;
			if (GetPickingPoint(pt.x, pt.y, m_pFloor->GetVertex(), targetPos)) {
				movePlayerToHere(targetPos);
			}
		}
		return false;
	}
	case WM_RBUTTONDOWN:
	{
		//�׷��� �����͸� ���� �κ�!
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		if (m_pFloor) {
			if (g_pKeyManager->isStayKeyDown(VK_SHIFT)) {
				//��� �߰�
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
		//�׷��� �����͸� ���� �κ�!
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
		////////////////////////���ͽ�Ʈ�� �׽�Ʈ////////////////////////////
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
		//���� �÷��̾� ��ġ���� �������� �ٷ� ���̴� ���.
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
		//���������� ���� ����� ��带 ã�´�.
		tagNavNode* destinationNode = GetNearestVisibleNavNode(destPos);
		if (destinationNode == NULL) {
			//��ֹ� ���θ� ���� ���!
			destinationNode = GetNearestNavNode(destPos);
			destPos = destinationNode->GetPosition();
		}

		//���ͽ�Ʈ�� �˰��� ����
		m_navGraph->Dijkstra(destinationNode->id, dijkstra);

		//�÷��̾�� ���� ����� ��带 ã�´�.
		tagNavNode* startNode = GetNearestVisibleNavNode(m_pPlayer->GetPosition());
		assert(startNode);

		//�÷��̾ ��û�ϰ� �������� �ʰ� �ϱ� ���� ���ͽ�Ʈ�� ������� ����/�� ��带 �߷�����.
		int currentNodeID = startNode->id;
		while (currentNodeID >= 0) {
			pathNodes.push_back(currentNodeID);
			currentNodeID = dijkstra[currentNodeID].parentID;
		}
	}

	//����Ʈ�� ���� ��带 ã�´�.
	//���� ��忡������ ��θ� Ÿ�� �ö󰡸鼭 ��ֹ���� �浹üũ�� �����Ѵ�.
	for (int i = 1; i < pathNodes.size(); ++i) {
		if (CheckRayCollisionWithObstaclesFromTo(m_pPlayer->GetPosition(), dijkstra[pathNodes[i]].node->GetPosition())) {
			//�浹�� �߻��ϸ� �浹�� ������ �ٷ� ���� ��带 ���۳��� �����Ѵ�.
			smartStartNode = i - 1;
			break;
		}
	}
	if (smartStartNode < 0) {
		smartStartNode = pathNodes.back();
	}

	//����Ʈ�� ���� ��带 ã�´�.
	//���� ��忡������ ��θ� �Ųٷ� Ÿ�� �������鼭 ��ֹ���� �浹üũ�� �����Ѵ�.
	for (int i = pathNodes.size() - 2; i >= smartStartNode; --i) {
		if (CheckRayCollisionWithObstaclesFromTo(destPos, dijkstra[pathNodes[i]].node->GetPosition())) {
			//�浹�� �߻��ϸ� �浹�� ������ �ٷ� ���� ��带 ���� ���� �����Ѵ�.
			smartDestinationNode = i + 1;
			break;
		}
	}
	if (smartDestinationNode < 0) {
		smartDestinationNode = smartStartNode;
	}

	assert(smartStartNode <= smartDestinationNode);
	//��η� ���õ� ������ Picked���·� �ٲ۴�.
	m_navGraph->ClearPickedNode();
	for (int i = smartStartNode; i <= smartDestinationNode; ++i) {
		m_navGraph->SetNodePicked(pathNodes[i]);
	}

	//���ͽ�Ʈ�� ��θ� ���� �÷��̾� �׼��� �߰��Ѵ�.
	cActionSeq* pActionMoveSeq = new cActionSeq;
	//�÷��̾� ��ġ���� ���� �������� ActionMove
	pActionMoveSeq->AddAction(CreateActionMove(m_pPlayer->GetPosition(), dijkstra[pathNodes[smartStartNode]].node->GetPosition()));
	//���ͽ�Ʈ�� ��θ� ���󰡱� ���� ActionMove
	for (int i = smartStartNode + 1; i <= smartDestinationNode; ++i) {
		pActionMoveSeq->AddAction(CreateActionMove(dijkstra[pathNodes[i - 1]].node->GetPosition(), dijkstra[pathNodes[i]].node->GetPosition()));
	}
	//������ ��忡�� ������������ ActionMove
	pActionMoveSeq->AddAction(CreateActionMove(dijkstra[pathNodes[smartDestinationNode]].node->GetPosition(), destPos));
	//�׼� ����
	pActionMoveSeq->Start();
	m_pPlayer->SetAction(pActionMoveSeq);
	//���� ������
	pActionMoveSeq->AutoRelease();
}

cActionMove * cMainGame::CreateActionMove(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	cActionMove* actionMove = new cActionMove(from, to);
	float distance = D3DXVec3Length(&(from - to));
	actionMove->SetActionTime(distance / PLAYER_MOVE_SPEED);
	actionMove->SetTarget(m_pPlayer);
	//���� ������
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
				//���� pos���̿� ��ֹ��� �ִ�.
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
		//��� ��ֹ��� �浹 üũ�� �Ѵ�.
		if (CheckRayCollisionWithObstaclesFromTo(pos, sortedNodes[i].node->GetPosition(), NULL)==false) {
			//������ �浹�ϴ� ��ֹ��� �ϳ��� ����!
			return sortedNodes[i].node;
		}
	}
	//��ġ pos���� ���̴� ��尡 �ϳ��� ����!
	return nullptr;
}

tagNavNode* cMainGame::GetNearestNavNode(D3DXVECTOR3 pos)
{
	return m_navGraph->GetNearestNode(pos);
}
