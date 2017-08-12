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

	//거리가 멀면 우선순위가 낮다.
	bool operator<(const tagNodeWithDistance& other) const {
		if (distance >= other.distance)
			return true;
		return false;
	}
};

typedef std::vector<DijkstraNode> DijkstraPath;
typedef std::priority_queue<tagNodeWithDistance> SortedNodeWithDistance;

class NavigationGraph
{
public:
	NavigationGraph();
	~NavigationGraph();

	void AddNode(float x, float y, float z);
	bool AddEdge(int id1, int id2);
	bool AddEdge(int id1, int id2, float cost);
	void GenerateNodeListWithDistance(D3DXVECTOR3 pos, SortedNodeWithDistance& sortedNodes);
	void Dijkstra(IN int idOfDestNode, OUT DijkstraPath& path);
	float GetDistanceSqToNode(D3DXVECTOR3 pos, int nodeID);
	float GetDistanceToNode(D3DXVECTOR3 pos, int nodeID);
	tagNavNode* GetNearestNode(D3DXVECTOR3 pos);
	bool Empty() const;
private:
	std::vector<tagNavNode*> m_nodes;
	std::vector<tagNavEdge*> m_edges;
	friend class NavigationGraphRenderer;
	friend std::ostream& operator<<(std::ostream& os, NavigationGraph& graph);
	friend std::istream& operator>>(std::istream& is, NavigationGraph& graph);
};

std::ostream& operator << (std::ostream& os, NavigationGraph& graph);
std::istream& operator >> (std::istream& is, NavigationGraph& graph);

class NavigationGraphRenderer {
public:
	NavigationGraphRenderer();
	~NavigationGraphRenderer();
	void SetGraph(NavigationGraph* graph);
	void UpdateEdgeVertexBuffer();
	void Render();
	bool PickNode(IN D3DXVECTOR3 rayPos, IN D3DXVECTOR3 rayDir, OUT int& nodeIdx);
	const std::set<int>& GetPickedNode() { return m_pickedNode; }
	void SetPickedNode(int idx);
	void ClearPickedNode();
private:
	NavigationGraph*			m_pGraph;
	LPD3DXFONT					m_pFont;
	D3DMATERIAL9				m_mtlNodeMesh;
	D3DMATERIAL9				m_mtlNodePickedMesh;
	LPD3DXMESH					m_pNodeMesh;
	LPDIRECT3DVERTEXBUFFER9		m_pVBEdge;
	size_t						m_numEdges;
	std::set<int>				m_pickedNode;
};