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
			//List에서 노드를 구분할 방법이 없으므로 번호를 붙여준다.
			m_aNode[row][col].nNum = ++nNum;

			//자기 자신의 Row Col 위치를 가지고 있는다.
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
					cout << "▩";
				}
				else if (m_aNode[row][col].isRoute == true) { cout << "■"; }
				else if (m_aNode[row][col].isRoute == false) { cout << "□"; }
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
	SetDir(m_aNode);//부모를 가리키고 있는 노드는 그 방향을 정해줌

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
			//블록은 검색할 일이 없으니 닫아져 있는 상태로 초기화
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

	aNode[nStartRow][nStartCol].fG = 0;		//시작 노드는 자기 자신과의 거리가 0

	aNode[nStartRow][nStartCol].fF =
		aNode[nStartRow][nStartCol].fG + aNode[nStartRow][nStartCol].nH;

	aNode[nStartRow][nStartCol].eState = ST_NODE::STATE_CLOSING;
	aNode[nStartRow][nStartCol].pParent = nullptr;	//시작지가 Root가 된다.
													//aNode[nStartRow][nStartCol].nDir = ST_NODE::DIR_NONE;

													//시작 노드를 Open 리스트에 추가
	lstOpen.push_back(&aNode[nStartRow][nStartCol]);

	//최초 Open된 시작 노드를 중심으로 8방향 확장
	ExtendEightWay(aNode, nStartRow, nStartCol, lstOpen, lstClose);

	/**********/
	//Complete	: F값이 가장 적은 노드를 찾는 메서드 필요.
	//Patched	- FindLowestF ExtendEightWay 수정 요망(대각선 최단거리 선택에 문제있음)
	//Complete	: List를 기준으로 순환하는 구조 필요.
	//Complete	: Block 피하기
	/**********/
	while (lstOpen.size() != 0 && m_isEnd == false)
	{
		//if (lstOpen.size() == 0)
		//{
		//	//OpenList에 값이 없다는 것은 목적지에 도착했거나, 길이 없거나 둘중 하난데_
		//	//길이 없을 경우에 대한 처리가 아직 없다.
		//	Print();
		//	break;
		//}
		ST_NODE* pstNode = nullptr;
		//가장 F값이 적은 노드를 찾아서
		FindLowestF(&pstNode, lstOpen);
		//8방향으로 확장한다.
		ExtendEightWay(aNode, pstNode->nRow, pstNode->nCol, lstOpen, lstClose);

		//Print();
	}

}
void cAstar::FindLowestF(OUT ST_NODE** ppstNode, IN list<ST_NODE*>& lstOpen)
{
	ST_NODE* pNode = nullptr;

	//Open list중 가장 F값이 작은 노드를 찾는다.
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
	//좌상단부터 우하단까지 중심노드 주변 8개의 노드를 순회함
	for (int row = nRow - 1; row <= nRow + 1; ++row)
	{
		for (int col = nCol - 1; col <= nCol + 1; ++col)
		{
			if (m_isEnd == true)
			{
				return;
			}
			//인접 row나 col이 배열의 범위에 벗어나는게 있으면 건너뜀
			if (row < 0 || row >= NODE_ROW ||
				col < 0 || col >= NODE_COL)
			{
				continue;
			}
			//이미 Close된 노드도 건너뜀(경고!)
			else if (aNode[row][col].eState == ST_NODE::STATE_CLOSED)
			{
				continue;
			}
			//확장의 중심노드를 만나도 건너뜀(Closing 상태일듯)
			else if (row == nRow && col == nCol)
			{
				continue;
			}

			//이제 바꿀 노드들이 해당됨. 가로세로 || 대각선

			//행과 열이 모두 다르다는것은 대각선에 위치했다는것을 의미
			else if ((row != nRow) && (col != nCol))
			{
				//대각선 위치에 Block이 존재하면 피해가야하므로 Open하지 않는다.
				if (CheckBlock(aNode, stParent, aNode[row][col]) == true)
				{
					continue;
				}
				OpenNode(stParent, aNode[row][col], lstOpen, NODE_DIAGONAL);
			}

			//행과 열중 하나라도 같다면 가로/세로에 위치했다는것을 의미
			else if ((row == nRow) || (col == nCol))
			{
				OpenNode(stParent, aNode[row][col], lstOpen, NODE_SIZE);
			}

		}//for col
	}//for row

	 //int a = 0;
	 //만약 중심(부모)노드가 Open리스트에 존재한다면 삭제한다.(용무끝)

	lstOpen.remove(&stParent);

	//작업이 끝났으면 중심(부모)노드는 Close 리스트에 추가한다.
	stParent.eState = ST_NODE::STATE_CLOSED;
	lstClose.push_back(&stParent);

}

void cAstar::OpenNode(
	IN OUT ST_NODE& stParent,
	IN OUT ST_NODE& stNode,
	OUT list<ST_NODE*>& lstOpen,
	IN float nLength)
{
	//ST_NODE& stNode = aNode[row][col];	//대상노드

	//목적지를 발견했을때
	if (stNode.eType == ST_NODE::TYPE_DEST)
	{
		stNode.pParent = &stParent;
		SetShortestRoute(OUT m_lstRoute, IN &stNode);
		m_isEnd = true;
		return;
	}
	//아직 Open되지 않은 노드일때
	if (stNode.eState == ST_NODE::STATE_PURE)
	{
		stNode.eState = ST_NODE::STATE_OPEN;
		stNode.pParent = &stParent;
		stNode.fG = stParent.fG + nLength;
		stNode.fF = stNode.fG + (float)stNode.nH;
		lstOpen.push_back(&stNode);		//Open 리스트에 추가해줌
	}
	//이미 Open되어 있는 노드라면 갱신여부를 판단한다
	else if (stNode.eState == ST_NODE::STATE_OPEN)
	{
		//중심(부모)노드의 G에 중심과의 거리를 더한것이_
		//대상이 가지고 있는G보다 작다면(더 효율적이라면) 갱신한다.
		if (stNode.fG > (stParent.fG + nLength))
		{
			stNode.pParent = &stParent;
			stNode.fG = stParent.fG + nLength;
			stNode.fF = stNode.fG + (float)stNode.nH;
			//리스트에는 주소가 들어가있으니까 본체가 바뀌면 알아서 바뀌겠지만,
			//뭔가 처리가 필요한지 ... 경고!
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
			//PURE에 어떻게 부모가 있을 수 있지? 일단 보류하고 넘어가서 출력후에 다시 보자. 경고!
			/*if (ST_NODE::STATE_PURE == stNode->eState)
			{
			break;
			}*/
			//부모노드가 없으면 방향을 정할 수 없다.(아마 PURE일 가능성이 높다)
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

	//도착지부터 자신의 부모를 따라 역순으로 정렬하면 출발지->도착지의 경로가 나온다.
	while (pNode != nullptr)
	{
		pNode->isRoute = true;		//최단경로에 포함
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
	//배열 범위를 벗어나는거 체크하는건데.. 이게 꼭 필요한가?
	/*if (stNode.nRow < 0 || stNode.nCol < 0 ||
	stNode.nRow >= NODE_ROW || stNode.nCol >= NODE_COL)
	{
	return false;
	}*/

	//부모의 Row 자식의 Col / 자식의 Row 부모의 Col 위치가 서로의 대각선이더라.
	//이 두 위치중 하나라도 Block이면 대각선으로 진행할 수 없다.
	//그러므로 true가 반환되면 다른 길을 찾아야함.
	if (aNode[stParent.nRow][stNode.nCol].eType == ST_NODE::TYPE_BLOCK ||
		aNode[stNode.nRow][stParent.nCol].eType == ST_NODE::TYPE_BLOCK)
	{
		return true;
	}

	return false;
}

