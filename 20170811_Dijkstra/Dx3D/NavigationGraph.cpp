#include "stdafx.h"
#include "NavigationGraph.h"
#include <fstream>
void CheckError(HRESULT result) {
	int a = 10;
	switch (result) {
	case D3DERR_WRONGTEXTUREFORMAT:
		a = 20;
		break;
	case D3DERR_UNSUPPORTEDCOLOROPERATION:
		a = 20;
		break;
	case D3DERR_UNSUPPORTEDCOLORARG:	a = 20;
		break;
	case D3DERR_UNSUPPORTEDALPHAOPERATION:	a = 20;
		break;
	case D3DERR_UNSUPPORTEDALPHAARG:	a = 20;
		break;
	case D3DERR_TOOMANYOPERATIONS:	a = 20;
		break;
	case D3DERR_CONFLICTINGTEXTUREFILTER:	a = 20;
		break;
	case D3DERR_UNSUPPORTEDFACTORVALUE:	a = 20;
		break;
	case D3DERR_CONFLICTINGRENDERSTATE:	a = 20;
		break;
	case D3DERR_UNSUPPORTEDTEXTUREFILTER:	a = 20;
		break;
	case D3DERR_CONFLICTINGTEXTUREPALETTE:	a = 20;
		break;
	case D3DERR_DRIVERINTERNALERROR:	a = 20;
		break;
	case D3DERR_NOTFOUND:	a = 20;
		break;
	case D3DERR_MOREDATA:	a = 20;
		break;
	case D3DERR_DEVICELOST:	a = 20;
		break;
	case D3DERR_DEVICENOTRESET:	a = 20;
		break;
	case D3DERR_NOTAVAILABLE:	a = 20;
		break;
	case D3DERR_OUTOFVIDEOMEMORY:	a = 20;
		break;
	case D3DERR_INVALIDDEVICE:	a = 20;
		break;
	case D3DERR_INVALIDCALL:	a = 20;
		break;
	case D3DERR_DRIVERINVALIDCALL:	a = 20;
		break;
	case D3DERR_WASSTILLDRAWING:	a = 20;
		break;
	case D3DOK_NOAUTOGEN:	a = 20;
		break;
	case D3DERR_DEVICEREMOVED:	a = 20;
		break;
	case S_NOT_RESIDENT:	a = 20;
		break;
	case S_RESIDENT_IN_SHARED_MEMORY:	a = 20;
		break;
	case S_PRESENT_MODE_CHANGED:	a = 20;
		break;
	case S_PRESENT_OCCLUDED:	a = 20;
		break;
	case D3DERR_DEVICEHUNG:	a = 20;
		break;
	case D3DERR_UNSUPPORTEDOVERLAY:	a = 20;
		break;
	case D3DERR_UNSUPPORTEDOVERLAYFORMAT:	a = 20;
		break;
	case D3DERR_CANNOTPROTECTCONTENT:	a = 20;
		break;
	case D3DERR_UNSUPPORTEDCRYPTO:	a = 20;
		break;
	case D3DERR_PRESENT_STATISTICS_DISJOINT:	a = 20;
		break;
	case E_OUTOFMEMORY:	a = 20;
		break;
	}
}
NavigationGraph::NavigationGraph()
	:m_pFont(NULL), m_pNodeMesh(NULL), m_pVBEdge(NULL)
{
	//////////////////////노드 매터리얼 생성////////////////////////////
	m_mtlNodeMesh.Diffuse = D3DXCOLOR(0, 0, 0, 0);
	m_mtlNodeMesh.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_mtlNodeMesh.Emissive = D3DXCOLOR(0, 0, 0, 0);
	m_mtlNodeMesh.Power = 20.0f;

	m_mtlNodePickedMesh.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_mtlNodePickedMesh.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_mtlNodePickedMesh.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_mtlNodePickedMesh.Emissive = D3DXCOLOR(1, 0, 0, 1);
	m_mtlNodePickedMesh.Power = 20.0f;

	//////////////////////구 생성////////////////////////////
	D3DXCreateSphere(g_pD3DDevice, 0.3, 10, 10, &m_pNodeMesh, NULL);
	std::vector<DWORD> adjacency(m_pNodeMesh->GetNumFaces() * 3);
	m_pNodeMesh->GenerateAdjacency(0.001f, adjacency.data());
	m_pNodeMesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		adjacency.data(),
		0, 0, 0);

	//////////////////////글꼴 생성////////////////////////////
	D3DXFONT_DESC fd;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
	fd.Height = 30;
	fd.Width = 15;
	fd.Weight = FW_BOLD;
	fd.Italic = false;
	fd.CharSet = DEFAULT_CHARSET;
	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily = FF_DONTCARE;
	//AddFontResource("umberto.ttf");
	strcpy_s(fd.FaceName, "고딕");	//글꼴 스타일
	D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);
}


NavigationGraph::~NavigationGraph()
{
	for each(auto pNode in m_nodes) {
		delete pNode;
	}
	m_nodes.clear();
	for each(auto pEdge in m_edges) {
		delete pEdge;
	}
	m_edges.clear();

	SAFE_RELEASE(m_pNodeMesh);
	SAFE_RELEASE(m_pVBEdge);
	SAFE_RELEASE(m_pFont);
}

void NavigationGraph::AddNode(float x, float y, float z)
{
	m_nodes.push_back(new tagNavNode(m_nodes.size(), x, y, z));
}

bool NavigationGraph::AddEdge(int id1, int id2)
{
	assert(id1 >= 0 && id1 < m_nodes.size());
	assert(id2 >= 0 && id2 < m_nodes.size());
	tagNavNode* node1 = m_nodes[id1];
	tagNavNode* node2 = m_nodes[id2];
	float distance = sqrt(	pow(node1->x - node2->x, 2) +
							pow(node1->y - node2->y, 2) + 
							pow(node1->z - node2->z, 2));
	return AddEdge(id1, id2, distance);
}

bool NavigationGraph::AddEdge(int id1, int id2, float cost)
{
	assert(id1 >= 0 && id1 < m_nodes.size());
	assert(id2 >= 0 && id2 < m_nodes.size());
	tagNavNode* node1 = m_nodes[id1];
	tagNavNode* node2 = m_nodes[id2];
	//새로운 에지 생성
	m_edges.emplace_back(new tagNavEdge(node1, node2, cost));
	//노드에 에지 삽입
	node1->edges.push_back(m_edges.back());
	node2->edges.push_back(m_edges.back());
	return true;
}

void NavigationGraph::GetSortedNodeListWithDistance(IN D3DXVECTOR3 pos, OUT std::vector<tagNodeWithDistance>& sortedNodes)
{
	for (size_t i = 0; i < m_nodes.size(); ++i) {
		float distance = sqrt(pow(pos.x - m_nodes[i]->x, 2) + pow(pos.y - m_nodes[i]->y, 2) + pow(pos.z - m_nodes[i]->z, 2));
		sortedNodes.push_back(tagNodeWithDistance(m_nodes[i], distance));
	}
	std::sort(sortedNodes.begin(), sortedNodes.end());
}

void NavigationGraph::Dijkstra(IN int idOfDestNode, OUT DijkstraPath & path)
{
	assert(idOfDestNode >= 0 && idOfDestNode < m_nodes.size());
	size_t numNodes = m_nodes.size();
	std::vector<bool>	isChecked(numNodes, false);
	std::vector<float>	cost(numNodes, FLT_MAX);
	std::vector<int>	viaNodeID(numNodes, -1);
	int numCheckedNode = 0;

	//도착지 노드 초기화
	cost[idOfDestNode] = 0.0f;
	//탐색 시작
	int currentNodeID;
	while (numCheckedNode < numNodes) {
		//체크되지 않은 노드 중 비용이 가장 작은 노드를 선택한다.

		float minCost = FLT_MAX;
		currentNodeID = -1;
		for (size_t i = 0; i < numNodes; ++i) {
			if (isChecked[i] == false && cost[i] < minCost) {
				currentNodeID = i;
				minCost = cost[i];
			}
		}
		
		assert(currentNodeID>=0 && currentNodeID < numNodes);

		tagNavNode& currentNode = *m_nodes[currentNodeID];
		for (size_t iEdge = 0; iEdge < currentNode.edges.size(); ++iEdge) {
			tagNavEdge& edge = *currentNode.edges[iEdge];
			//에지가 가지고 있는 노드 포인터 둘중 하나는 currentNode와 같은 노드이다.
			tagNavNode& nextNode = edge.nodeA->id != currentNodeID ? *edge.nodeA : *edge.nodeB;
			if (isChecked[nextNode.id] == true)
				continue;
			float newCost = cost[currentNodeID] + edge.cost;
			if (newCost < cost[nextNode.id]) {
				cost[nextNode.id] = newCost;
				viaNodeID[nextNode.id] = currentNodeID;
			}
		}
		isChecked[currentNodeID] = true;
		numCheckedNode++;
	}

	//다익스트라 결과 출력
	path.resize(numNodes);
	for (size_t i = 0; i < numNodes; ++i) {
		path[i].cost = cost[i];
		path[i].node = m_nodes[i];
		path[i].parentID = viaNodeID[i];
	}
}

float NavigationGraph::GetDistanceToNode(D3DXVECTOR3 pos, int nodeID)
{
	if (nodeID < 0 || nodeID >= m_nodes.size())
		return FLT_MAX;
	return sqrt(pow(m_nodes[nodeID]->x - pos.x, 2) + pow(m_nodes[nodeID]->y - pos.y, 2) + pow(m_nodes[nodeID]->z - pos.z, 2));
}

tagNavNode * NavigationGraph::GetNearestNode(D3DXVECTOR3 pos)
{
	float dist = FLT_MAX;
	tagNavNode* nearestNode = nullptr;
	for (size_t i = 0; i < m_nodes.size(); ++i) {
		float distanceSq = pow(pos.x - m_nodes[i]->x, 2) + pow(pos.y - m_nodes[i]->y, 2) + pow(pos.z - m_nodes[i]->z, 2);
		if (distanceSq < dist) {
			nearestNode = m_nodes[i];
			dist = distanceSq;
		}
	}
	return nearestNode;
}

bool NavigationGraph::IsEmpty() const
{
	return m_nodes.empty() || m_edges.empty();
}

void NavigationGraph::Save(const char * fileName)
{
	std::ofstream file(fileName);
	assert(file.good());

	file << m_nodes.size() << std::endl;
	for (size_t i = 0; i < m_nodes.size(); ++i) {
		file << m_nodes[i]->id << " " << m_nodes[i]->x << " " << m_nodes[i]->y << " " << m_nodes[i]->z << std::endl;
	}
	file << m_edges.size() << std::endl;
	for (size_t i = 0; i < m_edges.size(); ++i) {
		file << m_edges[i]->nodeA->id << " " << m_edges[i]->nodeB->id << " " << m_edges[i]->cost << std::endl;
	}
}

bool NavigationGraph::Load(const char * fileName)
{
	std::ifstream file(fileName);
	if (file.good() == false) {
		return false;
	}
	size_t numNodes;
	file >> numNodes;
	m_nodes.resize(numNodes);
	for (size_t i = 0; i < numNodes; ++i) {
		tagNavNode* node = new tagNavNode;
		file >> node->id >> node->x >> node->y >> node->z;
		m_nodes[i] = node;
	}
	size_t numEdges;
	file >> numEdges;
	float cost;
	int nodeA, nodeB;
	m_edges.clear();
	for (size_t i = 0; i < numEdges; ++i) {
		file >> nodeA >> nodeB >> cost;
		AddEdge(nodeA, nodeB, cost);
	}
	UpdateEdgeVertexBuffer();
	return true;
}

void NavigationGraph::Render()
{
	D3DXMATRIX matWorld;

	D3DXMATRIX matView, matProj, matIdentity;
	D3DVIEWPORT9 viewport;
	char buffer[20];
	RECT rect{ 0,0,0,0 };
	D3DXMatrixIdentity(&matIdentity);
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	g_pD3DDevice->GetViewport(&viewport);

	//노드 렌더링
	for (size_t i = 0; i < m_nodes.size(); ++i) {
		D3DXVECTOR3 nodePos(m_nodes[i]->x, m_nodes[i]->y, m_nodes[i]->z);
		g_pD3DDevice->SetTexture(0, 0);
		//노드 선택 여부에 따라 매터리얼을 다르게 설정한다.
		if (m_isNodesSelected.find(i) != m_isNodesSelected.end()) {
			g_pD3DDevice->SetMaterial(&m_mtlNodePickedMesh);
		}
		else {
			g_pD3DDevice->SetMaterial(&m_mtlNodeMesh);
		}
		//월드 행렬 설정
		D3DXMatrixTranslation(&matWorld, nodePos.x, nodePos.y, nodePos.z);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		//구 메쉬 렌더링
		m_pNodeMesh->DrawSubset(0);

		//노드 번호 렌더링
		D3DXVec3Project(&nodePos, &nodePos, &viewport, &matProj, &matView, &matIdentity);
		rect.left = rect.right = nodePos.x;
		rect.top = rect.bottom = nodePos.y;
		sprintf_s(buffer, 20, "%d", m_nodes[i]->id);
		m_pFont->DrawText(
			NULL, buffer, strlen(buffer), &rect, DT_CENTER | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	}

	//에지 렌더링
	if (m_pVBEdge) {
		D3DXMATRIX identity;
		D3DXMatrixIdentity(&identity);
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &identity);
		g_pD3DDevice->SetTexture(0, 0);
		g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
		g_pD3DDevice->SetStreamSource(0, m_pVBEdge, 0, sizeof(ST_PC_VERTEX));
		g_pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, m_edges.size());
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

bool NavigationGraph::CheckPicking(IN D3DXVECTOR3 rayPos, IN D3DXVECTOR3 rayDir, OUT int & nodeIdx)
{

	D3DXMATRIX matWorld;
	for (size_t i = 0; i < m_nodes.size(); ++i) {
		D3DXVECTOR3 localRayPos = rayPos - D3DXVECTOR3(m_nodes[i]->x, m_nodes[i]->y, m_nodes[i]->z);
		BOOL hitted = FALSE;
		DWORD hitIdx;
		float dist;
		float u, v;
		D3DXINTERSECTINFO intersect;
		DWORD allHitCount;
		HRESULT ret = D3DXIntersectSubset(m_pNodeMesh, 0, &localRayPos, &rayDir, &hitted, &hitIdx, &u, &v, &dist, (LPD3DXBUFFER*)&intersect, &allHitCount);
		//CheckError(ret);
		assert(ret == D3D_OK);
		if (hitted == TRUE) {
			nodeIdx = i;
			return true;
		}
	}
	return false;
}

void NavigationGraph::UpdateEdgeVertexBuffer()
{
	SAFE_RELEASE(m_pVBEdge);
	if (m_edges.empty()) {
		return;
	}
	g_pD3DDevice->CreateVertexBuffer(
		m_edges.size() * 2 * sizeof(ST_PC_VERTEX), D3DUSAGE_WRITEONLY, ST_PC_VERTEX::FVF,
		D3DPOOL_MANAGED, &m_pVBEdge, NULL);

	ST_PC_VERTEX * vertex;
	m_pVBEdge->Lock(0, m_edges.size() * 2, (void**)&vertex, 0);
	for (size_t i = 0; i < m_edges.size(); ++i) {
		size_t vertexIdx = i * 2;
		vertex[vertexIdx].p = { m_edges[i]->nodeA->x,m_edges[i]->nodeA->y ,m_edges[i]->nodeA->z };
		vertex[vertexIdx].c = D3DCOLOR_XRGB(0, 0, 0);
		vertex[vertexIdx + 1].p = { m_edges[i]->nodeB->x,m_edges[i]->nodeB->y ,m_edges[i]->nodeB->z };
		vertex[vertexIdx + 1].c = D3DCOLOR_XRGB(0, 0, 0);
	}
	m_pVBEdge->Unlock();
}

void NavigationGraph::SetNodePicked(int nodeIdx)
{
	m_isNodesSelected.insert(nodeIdx);
}

void NavigationGraph::ClearPickedNode()
{
	m_isNodesSelected.clear();
}