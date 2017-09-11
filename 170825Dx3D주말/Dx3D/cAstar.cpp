#include "stdafx.h"
#include "cAstar.h"


cAstar::cAstar()
	: m_sPath("map.txt")
	, m_nStartRow(-1)
	, m_nStartCol(-1)
	, m_nDestRow(-1)
	, m_nDestCol(-1)
	, m_isEnd(false)

{
	m_lstRoute.clear();
}

cAstar::~cAstar()
{
	m_lstOpen.clear();
	m_lstClose.clear();
}
void cAstar::InitNode(ST_NODE& stNode)
{
	ST_NODE pNode;

	pNode.eState = ST_NODE::STATE_PURE;
	pNode.eType = ST_NODE::TYPE_EMPTY;
	pNode.nH = -1;
	pNode.fG = -1;
	pNode.fF = -1;
	pNode.isRoute = false;
	pNode.nNum = -1;
	pNode.nRow = -1;
	pNode.nCol = -1;
	pNode.nDir = 0;
	pNode.pParent = nullptr;
	stNode = pNode;
}
void cAstar::Setup()
{
	int nNum = -1;
	for (int row = 0; row < NODE_ROW; ++row)
	{
		for (int col = 0; col < NODE_COL; ++col)
		{
			InitNode(m_aNode[row][col]);
			//List���� ��带 ������ ����� �����Ƿ� ��ȣ�� �ٿ��ش�.
			m_aNode[row][col].nNum = ++nNum;

			//�ڱ� �ڽ��� Row Col ��ġ�� ������ �ִ´�.
			m_aNode[row][col].nRow = row;
			m_aNode[row][col].nCol = col;
		}
	}
	MapLoad(m_sPath);
}
void cAstar::Print()
{
	//nNum
	/*cout << "==nNum==" << endl;
	for (int row = 0; row < NODE_ROW; ++row)
	{
	for (int col = 0; col < NODE_COL; ++col)
	{
	cout << m_aNode[row][col].nNum << " ";
	}
	cout << endl;
	}*/

	//STATE
	cout << "==STATE==" << endl;
	for (int row = 0; row < NODE_ROW; ++row)
	{
		for (int col = 0; col < NODE_COL; ++col)
		{
			if (m_aNode[row][col].eState == ST_NODE::STATE_PURE)
			{
				cout << "P";
			}
			else if (m_aNode[row][col].eState == ST_NODE::STATE_OPEN)
			{
				cout << "O";
			}
			else if (m_aNode[row][col].eState == ST_NODE::STATE_CLOSING)
			{
				cout << "@";
			}
			else if (m_aNode[row][col].eState == ST_NODE::STATE_CLOSED)
			{
				cout << "C";
			}
			else
			{
				cout << "?";
			}
		}
		cout << endl;
	}
	if (m_lstRoute.size() != 0)
	{
		bool isRoute = false;
		for (int row = 0; row < NODE_ROW; ++row)
		{
			for (int col = 0; col < NODE_COL; ++col)
			{
				if (m_aNode[row][col].eType == ST_NODE::TYPE_BLOCK)
				{
					cout << "��";
				}
				else if (m_aNode[row][col].isRoute == true) { cout << "��"; }
				else if (m_aNode[row][col].isRoute == false) { cout << "��"; }
			}
			cout << endl;
		}
		for each(auto it in m_lstRoute)
		{
			cout << "(";
			cout << it->nRow << ",";
			cout << it->nCol << ") ";
			
		}
		
	}
	

}
void cAstar::NavigateAstar(int StartRow, int StartCol, int DestRow, int DestCol)
{
	MapLoad(m_sPath);

	m_nStartRow = StartRow;
	m_nStartCol = StartCol;
	m_nDestRow = DestRow;
	m_nDestCol = DestCol;


	m_aNode[m_nStartRow][m_nStartCol].eType = ST_NODE::TYPE_START;
	m_aNode[m_nStartRow][m_nStartCol].eState = ST_NODE::STATE_PURE;

	m_aNode[m_nDestRow][m_nDestCol].eType = ST_NODE::TYPE_DEST;
	m_aNode[m_nDestRow][m_nDestCol].eState = ST_NODE::STATE_PURE;

	m_isEnd = false;

	SetH(m_aNode, m_nStartRow, m_nStartCol, m_nDestRow, m_nDestCol);
	FindShortestRoute(m_aNode, m_lstOpen, m_lstClose);
	SetDir(m_aNode);//�θ� ����Ű�� �ִ� ���� �� ������ ������

}
bool cAstar::ClickBlock(int clickRow, int clickCol)
{
	if (m_aNode[clickRow][clickCol].eType == ST_NODE::TYPE_BLOCK) {
		return true;
	}
	return false;
}
void cAstar::FileLoad(string sPath)
{
	FILE *pFile;
	int nRow = 0;
	int nCol = -1;
	int nNum = -1;
	fopen_s(&pFile, sPath.c_str(), "r");

	cout << "==TYPE==" << endl;
	while (!feof(pFile))
	{
		char c;
		c = fgetc(pFile);
		++nCol;

		if (c == '\n')
		{
			nCol = -1;
			if (nRow < NODE_ROW - 1)
			{
				nRow++;
			}
		}

		else if (c == EOF)
		{
			break;
		}

		else if (c == 'E')
		{
			m_aNode[nRow][nCol].eType = ST_NODE::TYPE_EMPTY;
			m_aNode[nRow][nCol].eState = ST_NODE::STATE_PURE;
		}
		else if (c == 'B')
		{
			m_aNode[nRow][nCol].eType = ST_NODE::TYPE_BLOCK;
			//����� �˻��� ���� ������ �ݾ��� �ִ� ���·� �ʱ�ȭ
			m_aNode[nRow][nCol].eState = ST_NODE::STATE_CLOSED;
		}
		else if (c == 'S')
		{
			//m_pstStart = &m_aNode[nRow][nCol];
			m_nStartRow = nRow;
			m_nStartCol = nCol;
			m_aNode[nRow][nCol].eType = ST_NODE::TYPE_START;
			m_aNode[nRow][nCol].eState = ST_NODE::STATE_PURE;
		}
		else if (c == 'D')
		{
			//m_pstDest = &m_aNode[nRow][nCol];
			m_nDestRow = nRow;
			m_nDestCol = nCol;
			m_aNode[nRow][nCol].eType = ST_NODE::TYPE_DEST;
			m_aNode[nRow][nCol].eState = ST_NODE::STATE_PURE;
		}
		else if (c == '#')
		{
			int a = 0;
		}

		if (m_nStartRow != 2)
		{
			int a = 0;
		}
		else
		{
			int a = 0;
		}
		cout << c;
	}
	cout << endl;
	fclose(pFile);
}
void cAstar::MapLoad(string sPath)
{
	FILE *pFile;
	int nRow = 0;
	int nCol = -1;
	fopen_s(&pFile, sPath.c_str(), "r");

	while (!feof(pFile))
	{
		char c;
		c = fgetc(pFile);
		++nCol;

		if (c == '\n')
		{
			nRow++;
			nCol = 0;
		}

		else if (c == '0')
		{
			m_aNode[nRow][nCol].eType = ST_NODE::TYPE_EMPTY;
			m_aNode[nRow][nCol].eState = ST_NODE::STATE_PURE;
		}
		else if (c == '1')
		{
			m_aNode[nRow][nCol].eType = ST_NODE::TYPE_BLOCK;
			m_aNode[nRow][nCol].eState = ST_NODE::STATE_CLOSED;
		}

	}
	fclose(pFile);
}
void cAstar::SetH(ST_NODE aNode[][NODE_COL],
	int nStartRow, int nStartCol,
	int nDestRow, int nDestCol)
{
	for (int row = 0; row < NODE_ROW; ++row)
	{
		for (int col = 0; col < NODE_COL; ++col)
		{
			aNode[row][col].nH = abs(nDestRow - row) + abs(nDestCol - col);
			cout << aNode[row][col].nH;
		}
	}
}
void cAstar::SetG(ST_NODE aNode[][NODE_COL], int nRow, int nCol, float fG)
{
	aNode[nRow][nCol].fG = fG;
}

void cAstar::FindShortestRoute(ST_NODE aNode[][NODE_COL],
	list<ST_NODE*>& lstOpen, list<ST_NODE*>& lstClose)
{
	lstOpen.clear();
	lstClose.clear();

	int nStartRow = m_nStartRow;
	int nStartCol = m_nStartCol;

	aNode[nStartRow][nStartCol].fG = 0;		//���� ���� �ڱ� �ڽŰ��� �Ÿ��� 0

	aNode[nStartRow][nStartCol].fF =
		aNode[nStartRow][nStartCol].fG + aNode[nStartRow][nStartCol].nH;

	aNode[nStartRow][nStartCol].eState = ST_NODE::STATE_CLOSING;
	aNode[nStartRow][nStartCol].pParent = nullptr;	//�������� Root�� �ȴ�.
													//aNode[nStartRow][nStartCol].nDir = ST_NODE::DIR_NONE;

													//���� ��带 Open ����Ʈ�� �߰�
	lstOpen.push_back(&aNode[nStartRow][nStartCol]);

	//���� Open�� ���� ��带 �߽����� 8���� Ȯ��
	ExtendEightWay(aNode, nStartRow, nStartCol, lstOpen, lstClose);

	/**********/
	//Complete	: F���� ���� ���� ��带 ã�� �޼��� �ʿ�.
	//Patched	- FindLowestF ExtendEightWay ���� ���(�밢�� �ִܰŸ� ���ÿ� ��������)
	//Complete	: List�� �������� ��ȯ�ϴ� ���� �ʿ�.
	//Complete	: Block ���ϱ�
	/**********/
	while (lstOpen.size() != 0 && m_isEnd == false)
	{
		//if (lstOpen.size() == 0)
		//{
		//	//OpenList�� ���� ���ٴ� ���� �������� �����߰ų�, ���� ���ų� ���� �ϳ���_
		//	//���� ���� ��쿡 ���� ó���� ���� ����.
		//	Print();
		//	break;
		//}
		ST_NODE* pstNode = nullptr;
		//���� F���� ���� ��带 ã�Ƽ�
		FindLowestF(&pstNode, lstOpen);
		//8�������� Ȯ���Ѵ�.
		ExtendEightWay(aNode, pstNode->nRow, pstNode->nCol, lstOpen, lstClose);

		//Print();
	}

}
void cAstar::FindLowestF(OUT ST_NODE** ppstNode, IN list<ST_NODE*>& lstOpen)
{
	ST_NODE* pNode = nullptr;

	//Open list�� ���� F���� ���� ��带 ã�´�.
	for each(auto it in lstOpen)
	{
		if (pNode == nullptr)
		{
			pNode = it;
			continue;
		}

		if (it->fF < pNode->fF)
		{
			pNode = it;
		}
	}
	(*ppstNode) = pNode;
}

void cAstar::ExtendEightWay(ST_NODE aNode[][NODE_COL], int nRow, int nCol,
	list<ST_NODE*>& lstOpen, list<ST_NODE*>& lstClose)
{
	ST_NODE& stParent = aNode[nRow][nCol];
	//�»�ܺ��� ���ϴܱ��� �߽ɳ�� �ֺ� 8���� ��带 ��ȸ��
	for (int row = nRow - 1; row <= nRow + 1; ++row)
	{
		for (int col = nCol - 1; col <= nCol + 1; ++col)
		{
			if (m_isEnd == true)
			{
				return;
			}
			//���� row�� col�� �迭�� ������ ����°� ������ �ǳʶ�
			if (row < 0 || row >= NODE_ROW ||
				col < 0 || col >= NODE_COL)
			{
				continue;
			}
			//�̹� Close�� ��嵵 �ǳʶ�(���!)
			else if (aNode[row][col].eState == ST_NODE::STATE_CLOSED)
			{
				continue;
			}
			//Ȯ���� �߽ɳ�带 ������ �ǳʶ�(Closing �����ϵ�)
			else if (row == nRow && col == nCol)
			{
				continue;
			}

			//���� �ٲ� ������ �ش��. ���μ��� || �밢��

			//��� ���� ��� �ٸ��ٴ°��� �밢���� ��ġ�ߴٴ°��� �ǹ�
			else if ((row != nRow) && (col != nCol))
			{
				//�밢�� ��ġ�� Block�� �����ϸ� ���ذ����ϹǷ� Open���� �ʴ´�.
				if (CheckBlock(aNode, stParent, aNode[row][col]) == true)
				{
					continue;
				}
				OpenNode(stParent, aNode[row][col], lstOpen, NODE_DIAGONAL);
			}

			//��� ���� �ϳ��� ���ٸ� ����/���ο� ��ġ�ߴٴ°��� �ǹ�
			else if ((row == nRow) || (col == nCol))
			{
				OpenNode(stParent, aNode[row][col], lstOpen, NODE_SIZE);
			}

		}//for col
	}//for row

	 //int a = 0;
	 //���� �߽�(�θ�)��尡 Open����Ʈ�� �����Ѵٸ� �����Ѵ�.(�빫��)

	lstOpen.remove(&stParent);

	//�۾��� �������� �߽�(�θ�)���� Close ����Ʈ�� �߰��Ѵ�.
	stParent.eState = ST_NODE::STATE_CLOSED;
	lstClose.push_back(&stParent);

}

void cAstar::OpenNode(
	IN OUT ST_NODE& stParent,
	IN OUT ST_NODE& stNode,
	OUT list<ST_NODE*>& lstOpen,
	IN float nLength)
{
	//ST_NODE& stNode = aNode[row][col];	//�����

	//�������� �߰�������
	if (stNode.eType == ST_NODE::TYPE_DEST)
	{
		stNode.pParent = &stParent;
		SetShortestRoute(OUT m_lstRoute, IN &stNode);
		m_isEnd = true;
		return;
	}
	//���� Open���� ���� ����϶�
	if (stNode.eState == ST_NODE::STATE_PURE)
	{
		stNode.eState = ST_NODE::STATE_OPEN;
		stNode.pParent = &stParent;
		stNode.fG = stParent.fG + nLength;
		stNode.fF = stNode.fG + (float)stNode.nH;
		lstOpen.push_back(&stNode);		//Open ����Ʈ�� �߰�����
	}
	//�̹� Open�Ǿ� �ִ� ����� ���ſ��θ� �Ǵ��Ѵ�
	else if (stNode.eState == ST_NODE::STATE_OPEN)
	{
		//�߽�(�θ�)����� G�� �߽ɰ��� �Ÿ��� ���Ѱ���_
		//����� ������ �ִ�G���� �۴ٸ�(�� ȿ�����̶��) �����Ѵ�.
		if (stNode.fG > (stParent.fG + nLength))
		{
			stNode.pParent = &stParent;
			stNode.fG = stParent.fG + nLength;
			stNode.fF = stNode.fG + (float)stNode.nH;
			//����Ʈ���� �ּҰ� �������ϱ� ��ü�� �ٲ�� �˾Ƽ� �ٲ������,
			//���� ó���� �ʿ����� ... ���!
		}
	}
}
void cAstar::SetDir(IN OUT ST_NODE aNode[][NODE_COL])
{
	ST_NODE* stNode = nullptr;
	ST_NODE* stParent = nullptr;
	for (int row = 0; row < NODE_ROW; ++row)
	{
		for (int col = 0; col < NODE_COL; ++col)
		{
			stNode = &aNode[row][col];
			//PURE�� ��� �θ� ���� �� ����? �ϴ� �����ϰ� �Ѿ�� ����Ŀ� �ٽ� ����. ���!
			/*if (ST_NODE::STATE_PURE == stNode->eState)
			{
			break;
			}*/
			//�θ��尡 ������ ������ ���� �� ����.(�Ƹ� PURE�� ���ɼ��� ����)
			if (stNode->pParent == nullptr)
			{
				break;
			}
			stParent = aNode[row][col].pParent;

			//row
			if (stNode->nRow < stParent->nRow)
			{
				stNode->nDir |= ST_NODE::DIR_UP;
			}
			else if (stNode->nRow > stParent->nRow)
			{
				stNode->nDir |= ST_NODE::DIR_DOWN;
			}
			//col
			if (stNode->nCol < stParent->nCol)
			{
				stNode->nDir |= ST_NODE::DIR_LEFT;
			}
			else if (stNode->nCol > stParent->nCol)
			{
				stNode->nDir |= ST_NODE::DIR_RIGHT;
			}
		}
	}



}
void cAstar::SetShortestRoute(OUT list<ST_NODE*>& lstNode, IN ST_NODE* pstNode)
{
	lstNode.clear();

	ST_NODE* pNode = nullptr;
	pNode = pstNode;

	//���������� �ڽ��� �θ� ���� �������� �����ϸ� �����->�������� ��ΰ� ���´�.
	while (pNode != nullptr)
	{
		pNode->isRoute = true;		//�ִܰ�ο� ����
		lstNode.push_front(pNode);
		pNode = pNode->pParent;
	}
	//MapLoad(m_sPath);
}

bool cAstar::CheckBlock(
	IN ST_NODE aNode[][NODE_COL],
	IN ST_NODE& stParent,
	IN ST_NODE& stNode)
{
	//�迭 ������ ����°� üũ�ϴ°ǵ�.. �̰� �� �ʿ��Ѱ�?
	/*if (stNode.nRow < 0 || stNode.nCol < 0 ||
	stNode.nRow >= NODE_ROW || stNode.nCol >= NODE_COL)
	{
	return false;
	}*/

	//�θ��� Row �ڽ��� Col / �ڽ��� Row �θ��� Col ��ġ�� ������ �밢���̴���.
	//�� �� ��ġ�� �ϳ��� Block�̸� �밢������ ������ �� ����.
	//�׷��Ƿ� true�� ��ȯ�Ǹ� �ٸ� ���� ã�ƾ���.
	if (aNode[stParent.nRow][stNode.nCol].eType == ST_NODE::TYPE_BLOCK ||
		aNode[stNode.nRow][stParent.nCol].eType == ST_NODE::TYPE_BLOCK)
	{
		return true;
	}

	return false;
}

