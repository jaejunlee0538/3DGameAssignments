#pragma once
class Pyramid
{
public:
	Pyramid();
	~Pyramid();
	void init(float width, float height, float depth, D3DCOLOR color);
	void Render() const;
	void Update(SGA::UserInputData& input);
	void SetPosition(float x, float y, float z);
	void SetAngle(float angleX, float angleY, float angleZ);
private:
	VertexPosNorm _vertices[5];
	D3DMATERIAL9 _material;
	int _indices[18];
	D3DXMATRIX _matWorld;
};

