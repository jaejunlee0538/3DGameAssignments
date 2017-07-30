#include "stdafx.h"
#include "BezierCurve.h"
namespace SGA {
	BezierCurve::BezierCurve()
		:_mode(INTERPOLATE_LINEAR), _totalLength(0)
	{
	}

	BezierCurve::~BezierCurve()
	{
	}

	void BezierCurve::reset() {
		_pts.clear();
		_lengthes.clear();
		_totalLength = 0;
	}

	void BezierCurve::pushBackPoint(float x, float y, float z)
	{
		_pts.push_back(PointType(x, y, z));
		if (_pts.size() >= 2) {
			size_t idx = _pts.size() - 1;
			_lengthes.push_back(D3DXVec3LengthSq(&(_pts[idx]- _pts[idx-1])));
			_totalLength += _lengthes.back();
		}
	}

	const BezierCurve::PointType& BezierCurve::computeFromTime(float timeLength, float t){
		return computeFromPhase(t / timeLength);
	}

	const BezierCurve::PointType& BezierCurve::computeFromPhase(float phase)
	{
		switch (_mode) {
		case INTERPOLATE_BEZIER:
			computeBezier(_pts, phase);
			break;
		case INTERPOLATE_LINEAR:
			computeLinear(phase);
			break;
		}
		return _currentPoint;
	}

	void BezierCurve::computeLinear(float phase)
	{
		float totalLength = phase*_totalLength;
		size_t idxPts;
		for (idxPts = 1; idxPts < _pts.size() && totalLength > _lengthes[idxPts-1]; ++idxPts) {
			totalLength -= _lengthes[idxPts];
		}
		if (idxPts >= _pts.size()) {
			//phase�� 1�� �Ѿ
			_currentPoint = _pts.back();
			_dirVector = _pts[_pts.size() - 1] - _pts[_pts.size() - 2];
			D3DXVec3Normalize(&_dirVector, &_dirVector);
		}
		else {
			_currentPoint = _pts[idxPts - 1] + totalLength / _lengthes[idxPts-1] * (_pts[idxPts] - _pts[idxPts - 1]);
			_dirVector = _pts[idxPts] - _pts[idxPts - 1];
			D3DXVec3Normalize(&_dirVector, &_dirVector);
		}
	}

	void BezierCurve::computeBezier(const std::vector<PointType>& pts, const float& phase)
	{
		//phase�� 0~1������ ����
		if (pts.size() < 2)
			return;
		if (pts.size() == 2) {
			_currentPoint = pts[0] + (pts[1] - pts[0]) * phase;//���� ��ġ
			_dirVector = pts[1] - pts[0];//���� ���� ���
			D3DXVec3Normalize(&_dirVector, &_dirVector);
		}
		else {
			//�Էµ� ��(pts)�� 2�� ���� ������ 
			std::vector<PointType> newPoints(pts.size() - 1);
			for (size_t i = 0; i < pts.size() - 1; ++i) {
				newPoints[i] = pts[i] + (pts[i+1] - pts[i]) * phase;
			}
			//
			computeBezier(newPoints, phase);
		}
	}
}