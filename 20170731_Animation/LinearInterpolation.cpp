#include "stdafx.h"
#include "LinearInterpolation.h"
namespace SGA {
	void LinearInterpolation::init(float xFrom, float yFrom, float zFrom, 
		float xTo, float yTo, float zTo)
	{
		_xFrom = xFrom;
		_xTo = xTo;
		_yFrom = yFrom;
		_yTo = yTo;
		_zFrom = zFrom;
		_zTo = zTo;
	}

	void LinearInterpolation::calculateWithTime(float timeLength, float t) {
		calculateWithPhase(t / timeLength);
	}

	void LinearInterpolation::calculateWithPhase(float phase) {
		_x = _xFrom + (_xTo - _xFrom) * phase;
		_y = _yFrom + (_yTo - _yFrom) * phase;
		_z = _zFrom + (_zTo - _zFrom) * phase;
	}
}