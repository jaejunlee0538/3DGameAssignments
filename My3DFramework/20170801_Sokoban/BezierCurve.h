#pragma once
#include <vector>
namespace SGA {

	class BezierCurve
	{
	public:
		enum InterpolationModes {
			INTERPOLATE_BEZIER,
			INTERPOLATE_LINEAR
		};
		typedef D3DXVECTOR3 PointType;
	public:
		BezierCurve();
		~BezierCurve();
		void reset();
		void pushBackPoint(float x, float y, float z);
		void setMode(InterpolationModes mode) { _mode = mode; }
		const PointType& getCurrentPosition() const { return _currentPoint; }
		const PointType& getCurrentDirection() const { return _dirVector; }
		const PointType& computeFromTime(float timeLength, float t);
		const PointType& computeFromPhase(float phase);
	private:
		void computeLinear(float phase);
		void computeBezier(const std::vector<PointType>& pts, const float& phase);
	private:
		InterpolationModes _mode;
		std::vector<PointType> _pts;
		std::vector<float> _lengthes;
		float _totalLength;
		PointType _currentPoint;
		PointType _dirVector;
	};
}