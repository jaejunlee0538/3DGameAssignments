#pragma once


class cAstar
{
public:
	ST_NODE			m_aNode[NODE_ROW][NODE_COL];	//(���Ͽ��� �ε���)��� Type �����迭
	string			m_sPath;						//���ϰ��
													//ST_NODE*		m_pstStart;						//����� ���(�ּҸ� �����ϴ� �޸� �帧�� ������ ������� ���۵��� �����Ѽ� �̷��� ���� �ʴ´�)
												//ST_NODE*		m_pstDest;						//������ ���
	int				m_nStartRow;
	int				m_nStartCol;
	int				m_nDestRow;
	int				m_nDestCol;
	list<ST_NODE*>	m_lstOpen;						//Open ����Ʈ
	list<ST_NODE*>	m_lstClose;						//Close ����Ʈ
	list<ST_NODE*>	m_lstRoute;						//�߰��� �ִܰŸ� ����Ʈ
	bool			m_isEnd;						//�ִܷ�Ʈ�� �߰��ߴ°�?

public:
	cAstar();
	~cAstar();
	void cAstar::InitNode(ST_NODE& stNode);
	void cAstar::FileLoad(string sPath);			//���Ͽ��� ��� ������ �о�´�
	void cAstar::MapLoad(string sPath);
	void cAstar::SetH(ST_NODE aNode[][NODE_COL],
		int nStartRow, int nStartCol,
		int nDestRow, int nDestCol);	//��� ��忡 H�� ����
	void cAstar::SetG(ST_NODE aNode[][NODE_COL],
		int nRow, int nCol, float fG);				//�ش� ��忡 G�� ����

	void cAstar::FindShortestRoute(
		ST_NODE aNode[][NODE_COL],
		list<ST_NODE*>& lstOpen,
		list<ST_NODE*>& lstClose);					//

													//�߽� ��带 �������� 8���� ���.
													//�ڽ��� �ֺ��� �˾ƾ� �ϱ⿡ ����� ������ �������� �ʰ� Row�� Col�� �ѱ��.
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
	void cAstar::Setup();							//�ʱ�ȭ
	void cAstar::Print();
	void cAstar::NavigateAstar(int StartRow,int StartCol,int DestRow,int DestCol);
	bool cAstar::ClickBlock(int clickRow,int clickCol);

};

