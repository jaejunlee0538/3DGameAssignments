#pragma once
#include <CameraIface.h>
#include <GameObject.h>
namespace SGA {
	class PlayerCamera :
		public SGA::CameraIface
	{
	public:
		PlayerCamera();
		virtual ~PlayerCamera();

		void update();

		void setTarget(GameObject* target) {
			_target = target;
		}

		void fixAxisX(bool enable) {
			_followX = !enable;
		}
		void fixAxisY(bool enable) {
			_followY = !enable;
		}
		void enableWorldLimit(bool enable, const RECT* worldRECT) {
			_isWorldLimitEnabled = enable;
			if (worldRECT != NULL) {
				_worldRECT = *worldRECT;
			}
		}
		bool isWorldLimitEnabled() const {
			return _isWorldLimitEnabled;
		}
		void enableSpring(bool enable) {
			_isSpringEnabled = enable;
		}
		bool isSpringEnabled() const {
			return _isSpringEnabled;
		}
		void setSpring(float k) {
			_K = k;
		}
		void setDamping(float d) {
			_D = d;
		}
		void setMaxSpeed(float _maxSpeed) {
			_maxSpeed = _maxSpeed;
		}

	protected:
		void follow();
		void updateSpring();
		void limitInWorld();
	private:
		bool _isWorldLimitEnabled;
		RECT _worldRECT;

		bool _isSpringEnabled;
		float _K, _D, _maxSpeed;
		float _eX, _eY;
		GameObject* _target;

		bool _followX, _followY;
	};
}
