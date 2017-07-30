#pragma once
#include "GameObject.h"

namespace SGA {
	class cBoxTextured;
	class Minecrafcharacter : public GameObject
	{
	public:
		enum States {
			STATE_IDLE,
			STATE_WALK,
			STATE_SPINATTACK,
			STATE_KICKATTACK
		};
		Minecrafcharacter();
		~Minecrafcharacter();
		void setup(D3DTexturePtr texture);
		void update(bool isDirty) override;

		void render() const override;
		void setState(States state);
		States getState() { return _state; }
		void setAnimation(const char* animName);
	private:
		void initAnimation();
		void initAnimationClip(AnimationClip& animClip);
		void appendKeyFramesRotation(KeyFrames& keyFrames, float tBegin, float tEnd, float dt, D3DXVECTOR3 angularSpeed, D3DXVECTOR3 angleOffset, D3DXVECTOR3 scale, D3DXVECTOR3 magnitudeOffset);
		void appendKeyFramesLinearInterpolate(KeyFrames& keyFrames, float tBegin, float tEnd, float dt, D3DXVECTOR3 posBegin, D3DXVECTOR3 posEnd, D3DXVECTOR3 angleBegin, D3DXVECTOR3 angleEnd);

		States _state;
		D3DMATERIAL9 _materialCube;

		AnimationClip * _currentAnimation = nullptr;
		std::map<std::string, AnimationClip> _animations;

		bool _spinAttackReady;
		bool _kickApplied;
		float _time;
		cBoxTextured * m_head = nullptr;
		cBoxTextured * m_body = nullptr;
		cBoxTextured * m_armL = nullptr;
		cBoxTextured * m_armR = nullptr;
		cBoxTextured * m_legL = nullptr;
		cBoxTextured * m_legR = nullptr;
	};
}

