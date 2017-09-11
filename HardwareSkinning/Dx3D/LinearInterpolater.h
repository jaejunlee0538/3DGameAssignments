#pragma once

class LinearInterpolator
{
public:
	LinearInterpolator() {}
	~LinearInterpolator() {}
	void init(float xFrom, float yFrom, float zFrom, float xTo, float yTo, float zTo);
	void calculateWithTime(float timeLength, float t);
	void calculateWithPhase(float phase);
	inline float getX() const { return _x; }
	inline float getY() const { return _y; }
	inline float getZ() const { return _z; }
private:
	float _xFrom, _xTo;
	float _yFrom, _yTo;
	float _zFrom, _zTo;
	float _x, _y, _z;
};