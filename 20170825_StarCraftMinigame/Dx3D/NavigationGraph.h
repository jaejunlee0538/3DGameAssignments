#pragma once
struct tagNavEdge;

struct tagNavNode{
	int id;
	float x, y, z;
	std::vector<tagNavEdge*> edges;

	tagNavNode() {

	}
	tagNavNode(int id, float x, float y, float z)
	:id(id), x(x), y(y), z(z){

	}

	D3DXVECTOR3 GetPosition() {
		return D3DXVECTOR3(x, y, z);
	}
};

struct tagNavEdge {
	float			cost;
	tagNavNode*		nodeA;
	tagNavNode*		nodeB;
	tagNavEdge(){}
	tagNavEdge(tagNavNode* nodeA, tagNavNode* nodeB, float cost)
	:cost(cost), nodeA(nodeA), nodeB(nodeB){

	}
};

struct DijkstraNode {
	tagNavNode* node;
	int parentID;
	float cost;

	DijkstraNode():
	node(nullptr), parentID(-1){

	}
	DijkstraNode(tagNavNode* node, int parentId, float cost)
	:node(node), parentID(parentId), cost(cost){

	}
};

struct tagNodeWithDistance {
	tagNavNode* node;
	float distance;

	tagNodeWithDistance(tagNavNode* node, float distance)
		:node(node), distance(distance) {

	}

	bool operator<(const tagNodeWithDistance& other) const {
		if (distance >= other.distance)
			return false;
		return true;
	}
};

typedef std::vector<DijkstraNode> DijkstraPath;
typedef std::list<tagNavNode*> AStarPath;
class NavigationGraph
{
public:
	NavigationGraph();
	~NavigationGraph();

	int AddNode(float x, float y, float z);
	bool AddEdge(int id1, int id2);
	bool AddEdge(int id1, int id2, float cost);
	void Dijkstra(IN int idOfDestNode, OUT DijkstraPath& path);
	void AStar(int idOfStartNode, int idOfDestNode, AStarPath& path);

	//pos에서 가까운 순서대로 정렬된 노드 목록을 레퍼런스로 반환한다.
	void GetSortedNodeListWithDistance(IN D3DXVECTOR3 pos, OUT std::vector<tagNodeWithDistance>& sortedNodes);
	//pos에서 노드 nodeID까지의 거리를 반환한다.
	float GetDistanceToNode(D3DXVECTOR3 pos, int nodeID);
	float GetDistanceNodeToNode(int id1, int id2) const;
	float GetDistanceNodeToNode(tagNavNode* n1, tagNavNode* n2);

	//pos에서 가장 가까운 노드를 반환한다.
	tagNavNode* GetNearestNode(D3DXVECTOR3 pos);

	//그래프가 비었으면 true를 반환한다.
	bool IsEmpty() const;

	//파일 저장 및 로드
	void Save(const char* fileName);
	bool Load(const char* fileName);

	//렌더링 관련 멤버함수
	void Render();
	bool CheckPicking(IN D3DXVECTOR3 rayPos, IN D3DXVECTOR3 rayDir, OUT int& nodeIdx);
	void UpdateEdgeVertexBuffer();
	void SetNodePicked(int nodeIdx);
	void ClearPickedNode();
private:
	std::vector<tagNavNode*> m_nodes;
	std::vector<tagNavEdge*> m_edges;
	std::set<int> m_isNodesSelected;

	//렌더링 관련 멤버변수
	LPD3DXFONT					m_pFont;
	D3DMATERIAL9				m_mtlNodeMesh;
	D3DMATERIAL9				m_mtlNodePickedMesh;
	LPD3DXMESH					m_pNodeMesh;
	LPDIRECT3DVERTEXBUFFER9		m_pVBEdge;
};
