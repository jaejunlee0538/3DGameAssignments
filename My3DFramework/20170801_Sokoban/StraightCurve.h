#pragma once
#include <vector>
namespace SGA {

	class StraightCurve
	{
	public:
		typedef D3DXVECTOR3 PointType;
	public:
		StraightCurve();
		~StraightCurve();
		void reset();
		void pushBackPoint(float x, float y, float z);
		const PointType& getCurrentPosition() const { return _currentPoint; }
		const PointType& getCurrentDirection() const { return _dir; }
		const PointType& computeFromTime(float timeLength, float t);
		const PointType& computeFromPhase(float phase);
	private:
		void compute(const std::vector<PointType>& pts, const float& phase);
	private:
		std::vector<PointType> _pts;
		PointType _currentPoint;
		PointType _dir;
	};
}