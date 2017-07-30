#include "stdafx.h"
#include "MinecraftCharacter.h"
#include "cBoxTextured.h"
#define U(idx) (idx / 16.0f)
#define V(idx) (idx / 8.0f)
namespace SGA {
	MinecraftCharacter::MinecraftCharacter()
	{
		_time = 0;
	}

	MinecraftCharacter::~MinecraftCharacter()
	{
	}

	void MinecraftCharacter::setup(D3DTexturePtr texture)
	{
		//전면, 후면,왼쪽 측면, 오른쪽 측면, 상단, 하단
		m_head = new cBoxTextured();
		m_head->init(2, 2, 2, D3DXVECTOR3(0, -1, 0),texture, {/*전*/U(2),V(2), U(4), V(4), /*후*/U(6), V(2), U(8), V(4),/*좌*/ U(0), V(2), U(2), V(4),/*우*/ U(4), V(2), U(6), V(4),/*상*/ U(2),V(0), U(4), V(2),/*하*/U(4), V(0), U(6), V(2)});
		m_body = new cBoxTextured();
		m_body->init(2, 3, 1, D3DXVECTOR3(0, 0, 0),texture, {/*전*/U(5), V(5), U(7), V(8), /*후*/U(8), V(5), U(10), V(8),/*좌*/ U(4), V(5), U(5), V(8), /*우*/U(7), U(5), U(8), V(8), /*상*/U(5), V(4), U(7), V(5), /*하*/ U(7), V(4), U(9), V(5) });
		m_armL = new cBoxTextured();
		m_armL->init(1, 3, 1, D3DXVECTOR3(0.5, 1.5, 0), texture, {/*전*/U(11), V(5),U(12), V(8),/*후*/U(12), V(5),U(13), V(8) ,/*좌*/U(10), V(5),U(11), V(8) ,/*우*/ U(11), V(5),U(12), V(8),/*상*/ U(11), V(4),U(12), V(5),/*하*/ U(12), V(4),U(13), V(5) });
		m_armR = new cBoxTextured();
		m_armR->init(1, 3, 1, D3DXVECTOR3(-0.5, 1.5, 0), texture, {/*전*/U(12), V(5),U(11), V(8),/*후*/U(13), V(5),U(12), V(8) ,/*좌*/ U(11), V(5),U(12), V(8),/*우*/U(10), V(5),U(11), V(8),/*상*/ U(12), V(4),U(11), V(5),/*하*/ U(13), V(4),U(12), V(5) });
		m_legR = new cBoxTextured();
		m_legR->init(1, 3, 1, D3DXVECTOR3(0, 1.5, 0), texture, {/*전*/U(1), V(5),U(2), V(8),/*후*/U(3), V(5),U(4), V(8) ,/*좌*/U(0), V(5),U(1), V(8) ,/*우*/ U(2), V(5),U(3), V(8),/*상*/ U(1), V(4),U(2), V(5),/*하*/ U(2), V(4),U(3), V(5) });
		m_legL = new cBoxTextured();
		m_legL->init(1, 3, 1, D3DXVECTOR3(0, 1.5, 0), texture, {/*전*/U(2), V(5),U(1), V(8),/*후*/U(4), V(5),U(3), V(8) ,/*좌*/U(1), V(5),U(0), V(8) ,/*우*/ U(3), V(5),U(2), V(8),/*상*/ U(2), V(4),U(1), V(5),/*하*/ U(3), V(4),U(2), V(5) });

		m_head->setTag(TEXT("Head"));
		m_body->setTag(TEXT("Body"));
		m_armL->setTag(TEXT("ArmLeft"));
		m_armR->setTag(TEXT("ArmRight"));
		m_legL->setTag(TEXT("LegLeft"));
		m_legR->setTag(TEXT("LegRight"));

		m_head->setPosition(0, 1.5, 0);
		m_body->setPosition(0, 4.5, 0);
		m_armL->setPosition(-1, 1.5, 0);
		m_armR->setPosition(1, 1.5, 0);
		m_legL->setPosition(-0.5, -1.5, 0);
		m_legR->setPosition(0.5, -1.5, 0);

		m_body->insertChild(m_head);
		m_body->insertChild(m_armL);
		m_body->insertChild(m_armR);
		m_body->insertChild(m_legL);
		m_body->insertChild(m_legR);
		insertChild(m_body);

		Materials::initMaterial(_materialCube, Colors::WHITE, Colors::WHITE, Colors::WHITE*0.1f, Colors::BLACK, 5);

		initAnimation();
		setAnimation(TEXT("Idle"));
	}

	void MinecraftCharacter::update(bool isDirty)
	{
		_time += 1.0 / 60.0f;
		
		switch (_state) {
		case STATE_SPINATTACK:
		{
			if (_spinAttackReady) {

			}
			else {
				if (_time >= _currentAnimation->getClipEndtime()) {
					_spinAttackReady = true;
					setAnimation(TEXT("SpinAttackRotating"));
				}
			}
		}
		break;
		case STATE_KICKATTACK:
		{
			if (_time >= _currentAnimation->getClipEndtime()) {
				setState(STATE_IDLE);
			}
		}
			break;
		default:
			
		break;
		}
		if (_time >= _currentAnimation->getClipEndtime()) {
			_time -= _currentAnimation->getClipEndtime();
		}
		KeyFrameSnapshot animSnapshot;
		_currentAnimation->interpolate(_time, animSnapshot);
		this->setKeyFrameAnimation(animSnapshot);
		GameObject::update(isDirty);
	}

	void MinecraftCharacter::checkCollision(std::list<std::shared_ptr<cObstacle>>& obstacles)
	{
		static int count = 0;
		switch (_state) {
		case STATE_SPINATTACK:
		{
			if (_spinAttackReady) {
				if (count == 20) {
					for (auto it = obstacles.begin(); it != obstacles.end(); ++it) {
						if (D3DXVec3Length(&((*it)->getPosition() - getPosition())) < 6) {
							(*it)->setCurrentHP((*it)->getCurrentHP() - 1);
						}
					}
					count = 0;
				}
				count++;
			}
		}
			break;
		case STATE_KICKATTACK:
		{
			if (_currentAnimation->calculatePhase(_time) >= 0.7f && _kickApplied==false) {
				for (auto it = obstacles.begin(); it != obstacles.end(); ++it) {
					D3DXVECTOR3 fromMeToObs = ((*it)->getPosition() - getPosition());
					if (D3DXVec3LengthSq(&fromMeToObs) < 36.0f) {
						D3DXVECTOR3 lookDir(0, 0, 1);
						D3DXVec3TransformNormal(&lookDir, &lookDir, &getWorldMatrix());
						D3DXVec3Normalize(&fromMeToObs, &fromMeToObs);
						float angle = acosf(D3DXVec3Dot(&lookDir, &fromMeToObs));
						if (fabs(angle) < D3DXToRadian(20.0f)) {
							(*it)->setCurrentHP((*it)->getCurrentHP() - 1);
							_kickApplied = true;
						}
					}
				}
			}
		}
			break;
			
		}
	}

	void MinecraftCharacter::render() const{
		g_pD3DDevice->SetMaterial(&_materialCube);
		GameObject::render();
	}

	void MinecraftCharacter::setState(States state)
	{
		if (state != _state) {
			switch (state) {
			case STATE_IDLE:
				setAnimation(TEXT("Idle"));
				break;
			case STATE_WALK:
				setAnimation(TEXT("Walk"));
				break;
			case STATE_SPINATTACK:
				_spinAttackReady = false;
				setAnimation(TEXT("SpinAttackReady"));
				break;
			case STATE_KICKATTACK:
				setAnimation(TEXT("KickAttack"));
				_kickApplied = false;
				break;
			}
			_state = state;
			_time = 0;
		}
	}

	void MinecraftCharacter::setAnimation(const TCHAR * animName)
	{
		_currentAnimation = &_animations[animName];
	}

	void MinecraftCharacter::initAnimation()
	{
		auto& animWalk = _animations[TEXT("Walk")];
		initAnimationClip(animWalk);
		appendKeyFramesRotation(animWalk.boneAnimations[0].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesRotation(animWalk.boneAnimations[1].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, D3DXToRadian(360), 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesRotation(animWalk.boneAnimations[2].keyFrames, 0, 1, 0.03, D3DXVECTOR3(D3DXToRadian(360), 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesRotation(animWalk.boneAnimations[3].keyFrames, 0, 1, 0.03, D3DXVECTOR3(D3DXToRadian(360), 0, 0), D3DXVECTOR3(D3DXToRadian(180), 0, 0), D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesRotation(animWalk.boneAnimations[4].keyFrames, 0, 1, 0.03, D3DXVECTOR3(D3DXToRadian(360), 0, 0), D3DXVECTOR3(D3DXToRadian(180), 0, 0), D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesRotation(animWalk.boneAnimations[5].keyFrames, 0, 1, 0.03, D3DXVECTOR3(D3DXToRadian(360), 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 0, 0));

		auto& animIdle = _animations[TEXT("Idle")];
		initAnimationClip(animIdle);
		appendKeyFramesRotation(animIdle.boneAnimations[0].keyFrames, 0, 1, 0.03, D3DXVECTOR3(D3DXToRadian(360), 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.02, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesRotation(animIdle.boneAnimations[1].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesRotation(animIdle.boneAnimations[2].keyFrames, 0, 1, 0.03, D3DXVECTOR3(D3DXToRadian(360), 0, -D3DXToRadian(360)), D3DXVECTOR3(D3DXToRadian(90), 0, 0), D3DXVECTOR3(0.1, 0, 0.1), D3DXVECTOR3(0, 0, -0.1));
		appendKeyFramesRotation(animIdle.boneAnimations[3].keyFrames, 0, 1, 0.03, D3DXVECTOR3(D3DXToRadian(360), 0, D3DXToRadian(360)), D3DXVECTOR3(D3DXToRadian(90), 0, 0), D3DXVECTOR3(0.1, 0, 0.1), D3DXVECTOR3(0, 0, 0.1));
		appendKeyFramesRotation(animIdle.boneAnimations[4].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesRotation(animIdle.boneAnimations[5].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));

		auto& animSpinAttackReady = _animations[TEXT("SpinAttackReady")];
		initAnimationClip(animSpinAttackReady);
		appendKeyFramesLinearInterpolate(animSpinAttackReady.boneAnimations[0].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesLinearInterpolate(animSpinAttackReady.boneAnimations[1].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesLinearInterpolate(animSpinAttackReady.boneAnimations[2].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, D3DXToRadian(-90)));
		appendKeyFramesLinearInterpolate(animSpinAttackReady.boneAnimations[3].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, D3DXToRadian(90)));
		appendKeyFramesLinearInterpolate(animSpinAttackReady.boneAnimations[4].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesLinearInterpolate(animSpinAttackReady.boneAnimations[5].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));


		auto& animSpinAttackRotating = _animations[TEXT("SpinAttackRotating")];
		initAnimationClip(animSpinAttackRotating);
		appendKeyFramesLinearInterpolate(animSpinAttackRotating.boneAnimations[0].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, D3DXToRadian(360*10), 0));
		appendKeyFramesLinearInterpolate(animSpinAttackRotating.boneAnimations[1].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesLinearInterpolate(animSpinAttackRotating.boneAnimations[2].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, D3DXToRadian(-90)), D3DXVECTOR3(0, 0, D3DXToRadian(-90)));
		appendKeyFramesLinearInterpolate(animSpinAttackRotating.boneAnimations[3].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, D3DXToRadian(90)), D3DXVECTOR3(0, 0, D3DXToRadian(90)));
		appendKeyFramesLinearInterpolate(animSpinAttackRotating.boneAnimations[4].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesLinearInterpolate(animSpinAttackRotating.boneAnimations[5].keyFrames, 0, 1, 0.03, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));

		auto& animKickAttack = _animations[TEXT("KickAttack")];
		initAnimationClip(animKickAttack);
		appendKeyFramesLinearInterpolate(animKickAttack.boneAnimations[0].keyFrames, 0, 0.2, 0.01, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesLinearInterpolate(animKickAttack.boneAnimations[1].keyFrames, 0, 0.2, 0.01, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesLinearInterpolate(animKickAttack.boneAnimations[2].keyFrames, 0, 0.2, 0.01, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesLinearInterpolate(animKickAttack.boneAnimations[3].keyFrames, 0, 0.2, 0.01, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
		appendKeyFramesLinearInterpolate(animKickAttack.boneAnimations[4].keyFrames, 0, 0.2, 0.01, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(D3DXToRadian(-90), 0, 0));
		appendKeyFramesLinearInterpolate(animKickAttack.boneAnimations[5].keyFrames, 0, 0.2, 0.01, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
	}


	void MinecraftCharacter::initAnimationClip(AnimationClip & animClip)
	{
		animClip.boneAnimations.resize(6);
		animClip.boneAnimations[0].boneName = TEXT("Body");
		animClip.boneAnimations[1].boneName = TEXT("Head");
		animClip.boneAnimations[2].boneName = TEXT("ArmLeft");
		animClip.boneAnimations[3].boneName = TEXT("ArmRight");
		animClip.boneAnimations[4].boneName = TEXT("LegLeft");
		animClip.boneAnimations[5].boneName = TEXT("LegRight");
	}

	void MinecraftCharacter::appendKeyFramesRotation(KeyFrames & keyFrames, float tBegin, float tEnd, float dt, D3DXVECTOR3 angularSpeed, D3DXVECTOR3 angleOffset, D3DXVECTOR3 scale, D3DXVECTOR3 magnitudeOffset)
	{
		size_t nFrames = (tEnd - tBegin) / dt + 1;
		keyFrames.reserve(keyFrames.size() + nFrames);
		float tOffset = keyFrames.empty()? 0 :keyFrames.back().time;
		for (float t = tBegin; t < tEnd; t += dt) {
			float angleX, angleY, angleZ;
			angleX = scale.x*sin(angularSpeed.x * t + angleOffset.x) + magnitudeOffset.x;
			angleY = scale.y*sin(angularSpeed.y * t + angleOffset.y) + magnitudeOffset.y;
			angleZ = scale.z*sin(angularSpeed.z * t + angleOffset.z) + magnitudeOffset.z;

			keyFrames.emplace_back(KeyFrame(tOffset+t, 0, 0, 0, angleX, angleY, angleZ));
		}
	}

	void MinecraftCharacter::appendKeyFramesLinearInterpolate(KeyFrames & keyFrames, float tBegin, float tEnd, float dt, D3DXVECTOR3 posBegin, D3DXVECTOR3 posEnd, D3DXVECTOR3 angleBegin, D3DXVECTOR3 angleEnd)
	{
		size_t nFrames = (tEnd - tBegin) / dt + 1;
		keyFrames.reserve(keyFrames.size() + nFrames);
		float tOffset = keyFrames.empty() ? 0 : keyFrames.back().time;
		for (float t = tBegin; t < tEnd; t += dt) {
			float angleX, angleY, angleZ;
			float x, y, z;
			float phase = (t - tBegin) / (tEnd - tBegin);
			D3DXVECTOR3 xyz = posBegin + (posEnd - posBegin) * phase;
			D3DXVECTOR3 angles = angleBegin + (angleEnd - angleBegin) * phase;
			D3DXQUATERNION quat;
			D3DXQuaternionRotationYawPitchRoll(&quat, angles.y, angles.x, angles.z);
			keyFrames.emplace_back(KeyFrame(tOffset+t, xyz, quat));
		}
	}
}
