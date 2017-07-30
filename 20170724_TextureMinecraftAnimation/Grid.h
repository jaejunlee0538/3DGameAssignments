#pragma once

class Grid
{
public:
	Grid();
	~Grid();
	void init(int nX, int nZ, float sizeX, float sizeZ);
	float GetWidth() const;
	float GetHeight() const;
	void Render() const;
private:
	D3DXMATRIX _matPosition;
	std::vector<VertexPosDiff> _vertices;
	D3DCOLOR _colorThick, _colorTine;
	int _thickLineInterval;

	float _w, _h;
};

