#include "stdafx.h"
#include "SokobanGame.h"
#include "cFrame.h"
#include "cObjLoader.h"
#include "cAseLoader.h"
#include "cCamera.h"

const float BOX_SCALE = 0.5f;
const float BOX_SIZE = 2.0 * BOX_SCALE;
const int MOVE_TIME = 500;

GridObject::GridObject()
	:m_currentObj(NULL)
{
	
}

void GridObject::Setup(POINT initPos, LookDirections lookDir, float yOffset)
{
	m_currentIdx.x = initPos.x;
	m_currentIdx.y = initPos.y;

	m_currentXYZ.x = initPos.x * BOX_SIZE;
	m_currentXYZ.y = yOffset;
	m_currentXYZ.z = initPos.y * BOX_SIZE;

	m_lookDir = lookDir;

	m_isMoving = false;
}

void GridObject::Update(int time, POINT realPos) {
	if (isMoving() == false && (realPos.x != m_currentIdx.x || realPos.y != m_currentIdx.y)) {
		m_lerp.init(m_currentIdx.x * BOX_SIZE, m_currentXYZ.y, m_currentIdx.y * BOX_SIZE,
			realPos.x * BOX_SIZE, m_currentXYZ.y, realPos.y * BOX_SIZE);
		m_currentIdx = realPos;
		m_isMoving = true;
		m_movePhase = 0.0f;
	}

	if (isMoving()) {
		m_movePhase += 0.05;
		if (m_movePhase >= 1.0f) {
			//다 움직임!
			m_movePhase = 1.0f;
			m_isMoving = false;
		}
		//이동용 애니메이션
		m_lerp.calculateWithPhase(m_movePhase);
		m_currentXYZ.x = m_lerp.getX();
		m_currentXYZ.y = m_lerp.getY();
		m_currentXYZ.z = m_lerp.getZ();
	}

	m_currentTime = time;
}

void GridObject::Render()
{
	if (m_currentObj) {
		//월드 행렬 업데이트
		D3DXMATRIX matR, matT;
		D3DXMatrixRotationY(&matR, GetAngle());
		D3DXMatrixTranslation(&matT, m_currentXYZ.x, m_currentXYZ.y, m_currentXYZ.z);
		m_matWorld = matR * matT;

		m_currentObj->Update(m_currentTime, &m_matWorld);
		m_currentObj->Render();
	}
}

float GridObject::GetAngle() const
{
	switch (m_lookDir) {
	case DIR_UP:
		return D3DX_PI;
	case DIR_RIGHT:
		return D3DX_PI * 3 / 2;
	case DIR_DOWN:
		return 0;
	case DIR_LEFT:
		return D3DX_PI / 2;
	}
	assert(false);
}

void GridObject::TurnLeft()
{
	int dir = m_lookDir;
	dir--;
	if (dir < 0) {
		dir = NUM_DIRS - 1;
	}
	m_lookDir = static_cast<LookDirections>(dir);
}

void GridObject::TurnRight()
{
	int dir = m_lookDir;
	dir++;
	if (dir == NUM_DIRS) {
		dir = 0;
	}
	m_lookDir = static_cast<LookDirections>(dir);
}

void GridObject::SetCurrentObject(cFrame * currentObj)
{
	m_currentObj = currentObj;
}

cFrame * GridObject::GetCurrentObject()
{
	return m_currentObj;
}

PlayerObject::PlayerObject(cFrame* pRunning, cFrame* pIdle)
	:m_pRunning(pRunning), m_pIdle(pIdle)
{
}

PlayerObject::~PlayerObject()
{
}

void PlayerObject::Update(int time, POINT realPos)
{
	GridObject::Update(time, realPos);
	if (isMoving()==true && GetCurrentObject() != m_pRunning) {
		SetCurrentObject(m_pRunning);
	}
	else if (isMoving() == false && GetCurrentObject() != m_pIdle) {
		SetCurrentObject(m_pIdle);
	}
}

BoxObject::BoxObject(cFrame * pBox)
{
	SetCurrentObject(pBox);
}

SokobanGame::SokobanGame()
{
	Reset();
}

SokobanGame::~SokobanGame()
{
	Reset();
	Uninitalize();
}

void SokobanGame::Init()
{
	{
		cAseLoader aseLoader;
		m_pPlayerRunning = aseLoader.Load("woman/woman_01_all.ASE");
		//Scene 시간 설정
		m_Firstframe = aseLoader.getFirstframe()* aseLoader.getTicksperframe();
		m_Lastframe = aseLoader.getLastframe()*aseLoader.getTicksperframe();
		m_FrameSpeed = (aseLoader.getFrameSpeed()*aseLoader.getTicksperframe()) / 1000.f;

	}
	{
		cAseLoader aseLoader;
		m_pPlayerIdle = aseLoader.Load("woman/woman_01_all_stand.ASE");
	}
	std::vector<cFrame*> tmp;
	{
		cObjLoader objLoader;
		//Floor
		objLoader.Load("./Resource/FloorBox.obj", tmp);
		m_pFloor = tmp[0];
		tmp.clear();
	}
	{
		cObjLoader objLoader;
		//Object
		objLoader.Load("./Resource/ObjectBox.obj", tmp);
		m_pObject = tmp[0];
		tmp.clear();
	}
	{
		cObjLoader objLoader;
		//Wall
		objLoader.Load("./Resource/WallBox.obj", tmp);
		m_pWall = tmp[0];
		tmp.clear();
	}
	{
		cObjLoader objLoader;
		//Destination
		objLoader.Load("./Resource/DestinationBox.obj", tmp);
		m_pDest = tmp[0];
		tmp.clear();
	}
	m_pFloor->ScaleVertices(BOX_SCALE);
	m_pObject->ScaleVertices(BOX_SCALE);
	m_pWall->ScaleVertices(BOX_SCALE);
	m_pDest->ScaleVertices(BOX_SCALE);
}

void SokobanGame::LoadGame(const char * mapFile)
{
	Reset();

	vector<GridObject*> floor1, floor2;
	int ix, iy;
	FILE* fp = 0;
	fopen_s(&fp, mapFile, "r");

	ix = 0; 
	iy = 0;
	while (!feof(fp))
	{
		char c = fgetc(fp);
		if (feof(fp))
			break;
		if (c == '\n')
		{
			m_mapFloor1.push_back(floor1);
			m_mapFloor2.push_back(floor2);

			floor1.clear();
			floor2.clear();
			iy++;
			ix = 0;
		}
		else
		{
			if (c == ' ') {
				floor1.push_back(NULL);
				floor2.push_back(NULL);
			}
			else {
				int kindBox = atoi(&c);
				switch (kindBox) {
				case OBJ_FLOOR:
					floor1.push_back(CreateFloor(ix, iy));
					floor2.push_back(NULL);
					break;
				case OBJ_BALL:
					floor1.push_back(CreateFloor(ix, iy));
					floor2.push_back(CreateBall(ix, iy));
					break;
				case OBJ_DESTINATION:
					floor1.push_back(CreateDestination(ix, iy));
					floor2.push_back(NULL);
					break;
				case OBJ_WALL:
					floor1.push_back(CreateFloor(ix, iy));
					floor2.push_back(CreateWall(ix, iy));
					break;
				case OBJ_PLAYER:
					floor1.push_back(CreateFloor(ix, iy));
					m_pPlayer = CreatePlayer(ix, iy);
					floor2.push_back(m_pPlayer);
					break;
				}
			}
			ix++;
		}
	}
	if (floor1.empty() == false) {
		m_mapFloor1.push_back(floor1);
		m_mapFloor2.push_back(floor2);
	}
	m_szMapY = m_mapFloor1.size();
	m_szMapX = m_mapFloor1.front().size();
	fclose(fp);

	if (m_cbStageLoad) {
		std::string stageName = mapFile;
		int begin = stageName.find("stage");
		stageName = stageName.substr(begin, stageName.find(".txt")- begin);
		m_cbStageLoad(stageName);
	}
}

void SokobanGame::Reset()
{
	for (size_t iy = 0; iy < m_mapFloor1.size(); ++iy) {
		for (size_t ix = 0; ix < m_mapFloor1.front().size(); ++ix) {
			SAFE_DELETE(m_mapFloor1[iy][ix]);
		}
	}
	for (size_t iy = 0; iy < m_mapFloor2.size(); ++iy) {
		for (size_t ix = 0; ix < m_mapFloor2.front().size(); ++ix) {
			SAFE_DELETE(m_mapFloor2[iy][ix]);
		}
	}
	m_nMovements = 0;
	if (m_cbMovementUpdate) {
		m_cbMovementUpdate(m_nMovements);
	}
	m_szMapY = 0;
	m_szMapX = 0;
	m_mapFloor1.clear();
	m_mapFloor2.clear();
	m_pPlayer = NULL;
}

void SokobanGame::Uninitalize()
{
	assert(m_pPlayer == NULL);

	SAFE_DESTROY(m_pPlayerRunning);
	SAFE_DESTROY(m_pPlayerIdle);
	SAFE_DESTROY(m_pDest);
	SAFE_DESTROY(m_pFloor);
	SAFE_DESTROY(m_pObject);
	SAFE_DESTROY(m_pWall);

	m_cbMovementUpdate = NULL;
	m_cbGameClear = NULL;
}

bool SokobanGame::IsGameCleared()
{
	if (m_pPlayer && m_pPlayer->isMoving() == false) {
		for (size_t iy = 0; iy < m_szMapY; ++iy) {
			for (size_t ix = 0; ix < m_szMapX; ++ix) {
				if (m_mapFloor1[iy][ix] && m_mapFloor1[iy][ix]->GetType() == OBJ_DESTINATION) {
					if (m_mapFloor2[iy][ix] == NULL || m_mapFloor2[iy][ix]->GetType() != OBJ_BALL) {
						return false;
					}
				}
			}
		}
		return true;
	}
	return false;
}

void SokobanGame::Update(int time)
{
	if (m_pPlayer && m_pPlayer->isMoving() == false) {
		if (g_pKeyManager->isOnceKeyDown('W'))
		{
			int deltaIx, deltaIz;
			switch (m_pPlayer->GetLookDirection()) {
			case GridObject::DIR_UP:
				deltaIx = 0; deltaIz = 1;
				break;
			case GridObject::DIR_RIGHT:
				deltaIx = 1; deltaIz = 0;
				break;
			case GridObject::DIR_DOWN:
				deltaIx = 0; deltaIz = -1;
				break;
			case GridObject::DIR_LEFT:
				deltaIx = -1; deltaIz = 0;
				break;
			}
			int playerIdxX, playerIdxZ;
			int nextIdxX, nextIdxZ;
			playerIdxX = m_pPlayer->GetCurrentIdx().x;
			playerIdxZ = m_pPlayer->GetCurrentIdx().y;
			nextIdxX = playerIdxX + deltaIx;
			nextIdxZ = playerIdxZ + deltaIz;
			if (m_mapFloor2[nextIdxZ][nextIdxX] != NULL) {
				//플레이어가 진행하려는 방향에 무언가가 있다.
				switch (m_mapFloor2[nextIdxZ][nextIdxX]->GetType()) {
				case OBJ_WALL:
					//아무것도 하지 않는다.
					break;
				case OBJ_BALL:
				{
					int nextNextIdxX = nextIdxX + deltaIx;
					int nextNextIdxZ = nextIdxZ + deltaIz;
					if (nextNextIdxX >= 0 && nextNextIdxX < m_szMapX && nextNextIdxZ >= 0 && nextNextIdxZ < m_szMapY) {
						if (m_mapFloor2[nextNextIdxZ][nextNextIdxX] == NULL) {
							m_mapFloor2[nextNextIdxZ][nextNextIdxX] = m_mapFloor2[nextIdxZ][nextIdxX];
							m_mapFloor2[nextIdxZ][nextIdxX] = m_mapFloor2[playerIdxZ][playerIdxX];
							m_mapFloor2[playerIdxZ][playerIdxX] = NULL;
							m_nMovements++;
							if (m_cbMovementUpdate) {
								m_cbMovementUpdate(m_nMovements);
							}
						}
						else {
							//공 너머에 장애물이 있기 때문에 움직일 수 없다.
						}
					}
				}
				break;
				}
			}
			else {
				m_mapFloor2[playerIdxZ + deltaIz][playerIdxX + deltaIx] = m_mapFloor2[playerIdxZ][playerIdxX];
				m_mapFloor2[playerIdxZ][playerIdxX] = NULL;
				m_nMovements++;
				if (m_cbMovementUpdate) {
					m_cbMovementUpdate(m_nMovements);
				}
			}
		}
		if (g_pKeyManager->isOnceKeyDown('S'))
		{
			m_pPlayer->TurnLeft();
			m_pPlayer->TurnLeft();
		}
		if (g_pKeyManager->isOnceKeyDown('A'))
		{
			m_pPlayer->TurnLeft();
		}
		if (g_pKeyManager->isOnceKeyDown('D'))
		{
			m_pPlayer->TurnRight();
		}
	}
	for (size_t iy = 0; iy < m_szMapY; ++iy) {
		for (size_t ix = 0; ix < m_szMapX; ++ix) {
			if (m_mapFloor1[iy][ix]) {
				m_mapFloor1[iy][ix]->Update(time, { (LONG)ix, (LONG)iy });
			}
		}
	}
	for (size_t iy = 0; iy < m_szMapY; ++iy) {
		for (size_t ix = 0; ix < m_szMapX; ++ix) {
			if (m_mapFloor2[iy][ix]) {
				m_mapFloor2[iy][ix]->Update(time, { (LONG)ix, (LONG)iy });
			}
		}
	}
	if (IsGameCleared() && m_cbGameClear) {
		m_cbGameClear(m_nMovements);
	}
}

void SokobanGame::Render()
{
	for (size_t iy = 0; iy < m_szMapY; ++iy) {
		for (size_t ix = 0; ix < m_szMapX; ++ix) {
			if (m_mapFloor1[iy][ix]) {
				m_mapFloor1[iy][ix]->Render();
			}
		}
	}
	for (size_t iy = 0; iy < m_szMapY; ++iy) {
		for (size_t ix = 0; ix < m_szMapX; ++ix) {
			if (m_mapFloor2[iy][ix]) {
				m_mapFloor2[iy][ix]->Render();
			}
		}
	}
}

bool SokobanGame::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return false;
}

D3DXVECTOR3 SokobanGame::GetPlayerPosition() {
	assert(m_pPlayer);
	return m_pPlayer->GetCurrentXYZ();
}

float SokobanGame::GetPlayerLookAngle()
{
	return m_pPlayer->GetAngle();
}

BoxObject * SokobanGame::CreateFloor(int ix, int iy)
{
	BoxObject* box = new BoxObject(m_pFloor);
	box->Setup({ ix, iy }, GridObject::DIR_RIGHT, -0.5*BOX_SIZE);
	box->SetType(OBJ_FLOOR);
	return box;
}

BoxObject * SokobanGame::CreateDestination(int ix, int iy)
{
	BoxObject* box = new BoxObject(m_pDest);
	box->Setup({ ix, iy }, GridObject::DIR_RIGHT, -0.5*BOX_SIZE);
	box->SetType(OBJ_DESTINATION);
	return box;
}

BoxObject * SokobanGame::CreateWall(int ix, int iy)
{
	BoxObject* box = new BoxObject(m_pWall);
	box->Setup({ ix, iy }, GridObject::DIR_RIGHT, 0.5*BOX_SIZE);
	box->SetType(OBJ_WALL);
	return box;
}

BoxObject * SokobanGame::CreateBall(int ix, int iy)
{
	BoxObject* box = new BoxObject(m_pObject);
	box->Setup({ ix, iy }, GridObject::DIR_RIGHT, 0.5*BOX_SIZE);
	box->SetType(OBJ_BALL);
	return box;
}

PlayerObject * SokobanGame::CreatePlayer(int ix, int iy)
{
	PlayerObject* player = new PlayerObject(m_pPlayerRunning, m_pPlayerIdle);
	player->SetType(OBJ_PLAYER);
	player->Setup({ ix, iy }, GridObject::DIR_RIGHT, 0);
	return player;
}


