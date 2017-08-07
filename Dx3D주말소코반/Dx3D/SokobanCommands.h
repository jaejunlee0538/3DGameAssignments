#pragma once
class SokobanGame;
class ICommand {
public:
	virtual void Do(SokobanGame& game) = 0;
	virtual void Undo(SokobanGame& game) = 0;
};

class CommandMove : public ICommand {
public:
	CommandMove(int fromIx, int fromIy, int deltaIx, int deltaIy, bool pushBox);

	void Do(SokobanGame& game) override;

	void Undo(SokobanGame& game) override;
private:
	int m_fromIx, m_fromIy;
	int m_deltaIx, m_deltaIy;
	bool m_bPushBox;
};

class CommandPlayerTurn : public ICommand {
public:
	enum TurnIDs {
		TURN_LEFT,
		TURN_RIGHT
	};
	CommandPlayerTurn(TurnIDs turnID, int nRepeat);
	void Do(SokobanGame& game) override;
	void Undo(SokobanGame& game) override;
private:
	TurnIDs m_turnID;
	int m_nTurn;
};