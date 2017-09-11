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

	//pos���� ����� ������� ���ĵ� ��� ����� ���۷����� ��ȯ�Ѵ�.
	void GetSortedNodeListWithDistance(IN D3DXVECTOR3 pos, OUT std::vector<tagNodeWithDistance>& sortedNodes);
	//pos���� ��� nodeID������ �Ÿ��� ��ȯ�Ѵ�.
	float GetDistanceToNode(D3DXVECTOR3 pos, int nodeID);
	float GetDistanceNodeToNode(int id1, int id2) const;
	float GetDistanceNodeToNode(tagNavNode* n1, tagNavNode* n2);

	//pos���� ���� ����� ��带 ��ȯ�Ѵ�.
	tagNavNode* GetNearestNode(D3DXVECTOR3 pos);

	//�׷����� ������� true�� ��ȯ�Ѵ�.
	bool IsEmpty() const;

	//���� ���� �� �ε�
	void Save(const char* fileName);
	bool Load(const char* fileName);

	//������ ���� ����Լ�
	void Render();
	bool CheckPicking(IN D3DXVECTOR3 rayPos, IN D3DXVECTOR3 rayDir, OUT int& nodeIdx);
	void UpdateEdgeVertexBuffer();
	void SetNodePicked(int nodeIdx);
	void ClearPickedNode();
private:
	std::vector<tagNavNode*> m_nodes;
	std::vector<tagNavEdge*> m_edges;
	std::set<int> m_isNodesSelected;

	//������ ���� �������
	LPD3DXFONT					m_pFont;
	D3DMATERIAL9				m_mtlNodeMesh;
	D3DMATERIAL9				m_mtlNodePickedMesh;
	LPD3DXMESH					m_pNodeMesh;
	LPDIRECT3DVERTEXBUFFER9		m_pVBEdge;
};
