#pragma once
class Grid
{
public:
	Grid();
	~Grid();
	void init(int nX, int nZ, float sizeX, float sizeZ);

	float getXWidth() const;
	float getYWidth() const;

	void render() const;

	MAKE_VAR_AND_GETSETTER_REFERENCE(private, D3DCOLOR, _thickLineColor, ThickLineColor);
	MAKE_VAR_AND_GETSETTER_REFERENCE(private, D3DCOLOR, _thinLineColor, ThinLineColor);
private:
	D3DXMATRIX _matPosition;
	std::vector<VertexPosDiff> _vertices;
	int _thickLineInterval;
	float _w, _h;
};

