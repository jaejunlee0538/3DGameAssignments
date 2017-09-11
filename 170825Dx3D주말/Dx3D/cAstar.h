#pragma once


class cAstar
{
public:
	ST_NODE			m_aNode[NODE_ROW][NODE_COL];	//(파일에서 로드한)노드 Type 정보배열
	string			m_sPath;						//파일경로
													//ST_NODE*		m_pstStart;						//출발지 노드(주소를 저장하니 메모리 흐름에 문제가 생기는지 오작동을 일으켜서 이렇게 하지 않는다)
												//ST_NODE*		m_pstDest;						//목적지 노드
	int				m_nStartRow;
	int				m_nStartCol;
	int				m_nDestRow;
	int				m_nDestCol;
	list<ST_NODE*>	m_lstOpen;						//Open 리스트
	list<ST_NODE*>	m_lstClose;						//Close 리스트
	list<ST_NODE*>	m_lstRoute;						//발견한 최단거리 리스트
	bool			m_isEnd;						//최단루트를 발견했는가?

public:
	cAstar();
	~cAstar();
	void cAstar::InitNode(ST_NODE& stNode);
	void cAstar::FileLoad(string sPath);			//파일에서 노드 정보를 읽어온다
	void cAstar::MapLoad(string sPath);
	void cAstar::SetH(ST_NODE aNode[][NODE_COL],
		int nStartRow, int nStartCol,
		int nDestRow, int nDestCol);	//모든 노드에 H값 세팅
	void cAstar::SetG(ST_NODE aNode[][NODE_COL],
		int nRow, int nCol, float fG);				//해당 노드에 G값 세팅

	void cAstar::FindShortestRoute(
		ST_NODE aNode[][NODE_COL],
		list<ST_NODE*>& lstOpen,
		list<ST_NODE*>& lstClose);					//

													//중심 노드를 기준으로 8방향 계산.
													//자신의 주변을 알아야 하기에 대상의 참조를 전달하지 않고 Row와 Col을 넘긴다.
	void cAstar::ExtendEightWay(
		ST_NODE aNode[][NODE_COL],
		int nRow, int nCol,
		list<ST_NODE*>& lstOpen,
		list<ST_NODE*>& lstClose);

	void cAstar::SetShortestRoute(
		OUT list<ST_NODE*>& lstNode,
		IN ST_NODE* pstNode);

	void cAstar::FindLowestF(
		OUT ST_NODE** ppstNode,
		IN list<ST_NODE*>& lstOpen);

	bool cAstar::CheckBlock(
		IN ST_NODE aNode[][NODE_COL],
		IN ST_NODE& stParent,
		IN ST_NODE& stNode);

	void cAstar::OpenNode(
		IN OUT ST_NODE& stParent,
		IN OUT ST_NODE& stNode,
		OUT list<ST_NODE*>& lstOpen,
		IN float nLength);
	void cAstar::SetDir(IN OUT ST_NODE aNode[][NODE_COL]);
	void cAstar::Setup();							//초기화
	void cAstar::Print();
	void cAstar::NavigateAstar(int StartRow,int StartCol,int DestRow,int DestCol);
	bool cAstar::ClickBlock(int clickRow,int clickCol);

};

