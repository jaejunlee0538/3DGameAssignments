#pragma once
class cBoxTextured
{
public:
	cBoxTextured();
	~cBoxTextured();

	void init(float width, float height, float depth, D3DXVECTOR2 uvOffset, D3DXVECTOR3 uvLength);
	void FlipX();
	void Render();
	void Update(SGA::UserInputData& input);
	void SetPosition(float x, float y, float z);
	D3DXVECTOR3 GetPosition() const;
	void Rotate(float angleX, float angleY, float angleZ);
	void Translate(float x, float y, float z);
	void TranslateLocal(float x, float y, float z);

private:
	tagVertexTextured _vertices[36];
	D3DXMATRIX _matWorld;
};

