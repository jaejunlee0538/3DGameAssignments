#pragma once

class cBox
{
public:
	cBox();
	cBox(float width, float height, float depth);
	~cBox();
	void init(float width, float height, float depth);
	void Render();
	void Update(SGA::UserInputData& input);
	void SetPosition(float x, float y, float z);
	D3DXVECTOR3 GetPosition() const;
	void Rotate(float angleX, float angleY, float angleZ);
	void Translate(float x, float y, float z);
	void TranslateLocal(float x, float y, float z);
	tagVertex _vertices[36];

	D3DXMATRIX _matWorld;
	//D3DXVECTOR3 _pos;
};