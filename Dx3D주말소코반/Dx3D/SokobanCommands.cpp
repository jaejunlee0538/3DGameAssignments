#include "stdafx.h"
#include "SokobanCommands.h"
#include "SokobanGame.h"

CommandPlayerTurn::CommandPlayerTurn(TurnIDs turnID, int nRepeat)
	:m_turnID(turnID), m_nTurn(nRepeat)
{
}

void CommandPlayerTurn::Do(SokobanGame & game) {
	switch (m_turnID) {
	case TURN_LEFT:
		for (int i = 0; i < m_nTurn; ++i) {
			game.GetPlayer()->TurnLeft();
		}
		break;
	case TURN_RIGHT:
		for (int i = 0; i < m_nTurn; ++i) {
			game.GetPlayer()->TurnRight();
		}
		break;
	}
}

void CommandPlayerTurn::Undo(SokobanGame & game) {
	switch (m_turnID) {
	case TURN_LEFT:
		for (int i = 0; i < m_nTurn; ++i) {
			game.GetPlayer()->TurnRight();
		}
		break;
	case TURN_RIGHT:
		for (int i = 0; i < m_nTurn; ++i) {
			game.GetPlayer()->TurnLeft();
		}
		break;
	}
}

CommandMove::CommandMove(int fromIx, int fromIy, int deltaIx, int deltaIy, bool pushBox)
	:m_fromIx(fromIx), m_fromIy(fromIy), m_deltaIx(deltaIx), m_deltaIy(deltaIy), m_bPushBox(pushBox)
{
}

void CommandMove::Do(SokobanGame & game) {
	GridObject* player = game.GetGridObject(m_fromIx, m_fromIy);
	if (m_bPushBox) {
		GridObject* obj = game.GetGridObject(m_fromIx + m_deltaIx, m_fromIy + m_deltaIy);
		game.SetGridObject(m_fromIx + 2 * m_deltaIx, m_fromIy + 2 * m_deltaIy, obj);
	}
	game.SetGridObject(m_fromIx + m_deltaIx, m_fromIy + m_deltaIy, player);
	game.SetGridObject(m_fromIx, m_fromIy, NULL);
}

void CommandMove::Undo(SokobanGame & game) {
	if (m_bPushBox) {
		GridObject* player = game.GetGridObject(m_fromIx + m_deltaIx, m_fromIy + m_deltaIy);
		GridObject* obj = game.GetGridObject(m_fromIx + 2*m_deltaIx, m_fromIy + 2*m_deltaIy);
		game.SetGridObject(m_fromIx + m_deltaIx, m_fromIy + m_deltaIy, obj);
		game.SetGridObject(m_fromIx, m_fromIy, player);
		game.SetGridObject(m_fromIx + 2 * m_deltaIx, m_fromIy + 2 * m_deltaIy, NULL);
	}
	else {
		GridObject* player = game.GetGridObject(m_fromIx + m_deltaIx, m_fromIy + m_deltaIy);
		game.SetGridObject(m_fromIx, m_fromIy, player);
		game.SetGridObject(m_fromIx + m_deltaIx, m_fromIy + m_deltaIy, NULL);
	}
	
	
}
