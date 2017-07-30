#include "stdafx.h"
#include "StraightCurve.h"
namespace SGA {
	StraightCurve::StraightCurve()
	{
	}

	StraightCurve::~StraightCurve()
	{
	}

	void StraightCurve::reset() {
		_pts.clear();
	}

	void StraightCurve::pushBackPoint(float x, float y, float z)
	{
		_pts.push_back(PointType(x, y, z));
	}

	const StraightCurve::PointType& StraightCurve::computeFromTime(float timeLength, float t) {
		return computeFromPhase(t / timeLength);
	}

	const StraightCurve::PointType& StraightCurve::computeFromPhase(float phase)
	{
		compute(_pts, phase);
		return _currentPoint;
	}

	void StraightCurve::compute(const std::vector<PointType>& pts, const float& phase)
	{
		if (pts.size() < 2)
			return;
		if (pts.size() == 2) {
			_currentPoint.x = pts[0].x + phase * (pts[1].x - pts[0].x);
			_currentPoint.y = pts[0].y + phase * (pts[1].y - pts[0].y);
			_currentPoint.z = pts[0].z + phase * (pts[1].z - pts[0].z);
			_dir.x = pts[1].x - pts[0].x;
			_dir.y = pts[1].y - pts[0].y;
			_dir.z = pts[1].z - pts[0].z;
			float len = sqrt(_dir.x*_dir.x + _dir.y*_dir.y + _dir.z*_dir.z);
			assert(len >= 0.00001f);
			_dir.x /= len;
			_dir.y /= len;
			_dir.z /= len;
		}
		else {
			std::vector<PointType> newPoints(pts.size() - 1);
			for (size_t i = 0; i < pts.size() - 1; ++i) {
				newPoints[i].x = pts[i].x + phase * (pts[i + 1].x - pts[i].x);
				newPoints[i].y = pts[i].y + phase * (pts[i + 1].y - pts[i].y);
				newPoints[i].z = pts[i].z + phase * (pts[i + 1].z - pts[i].z);
			}
			compute(newPoints, phase);
		}
	}
}