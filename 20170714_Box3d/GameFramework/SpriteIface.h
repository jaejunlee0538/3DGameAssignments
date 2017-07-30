#pragma once
#include <Windows.h>
namespace SGA {
	class SpriteIface {
	public:
		virtual ~SpriteIface(){}
		virtual void release(){}
		virtual void update(){}
		virtual void render(HDC hdc, int drawPosX, int drawPosY) const= 0;

		virtual SpriteIface* clone() const = 0;
		virtual void reset(){}
		//true�� �����ϸ� ��������Ʈ�� �ǹ� ����Ʈ�� �������� �¿� �����Ǿ� ��µȴ�.
		virtual void setFlipX(bool flipped) = 0;
		virtual bool getFlipX() const = 0;
		//�ǹ� ����Ʈ�� �������� �¿� ������ ������ clip ���� �����Ѵ�.
		//��ȿ ���� : [0, 1.0]
		virtual void setClipXFactor(float factor) = 0;
		//�ǹ� ����Ʈ�� �������� �¿� ������ ������ clip ������ �����Ѵ�.
		//��ȿ ���� : [0, 1.0]
		virtual void setClipYFactor(float factor) = 0;
		//ClipXFactor�� �ݿ��� ��� ��������Ʈ�� ���� ��ȯ�Ѵ�.
		virtual int getWidth() const = 0;
		//ClipYFactor�� �ݿ��� ��� ��������Ʈ�� ���̸� ��ȯ�Ѵ�.
		virtual int getHeight() const = 0;
		//��������Ʈ�� ���� ���� �ȼ� ��ǥ�� ��ȯ�Ѵ�(�ǹ� ����Ʈ ����)
		virtual int getLeft() const = 0;
		//��������Ʈ�� ���� ������ �ȼ� ��ǥ�� ��ȯ�Ѵ�(�ǹ� ����Ʈ ����)
		virtual int getRight() const = 0;
		//��������Ʈ�� ���� �� �ȼ� ��ǥ�� ��ȯ�Ѵ�(�ǹ� ����Ʈ ����)
		virtual int getTop() const = 0;
		//��������Ʈ�� ���� �Ʒ� �ȼ� ��ǥ�� ��ȯ�Ѵ�(�ǹ� ����Ʈ ����)
		virtual int getBottom() const = 0;

	/*	virtual void render(HDC hdc, int drawPosX, int drawPosY, int w, int h) const= 0;
		virtual void render(HDC hdc, int drawPosX, int drawPosY, float scale) const =0;
		virtual void renderRot(HDC hdc, int drawPosX, int drawPosY, float angle) const = 0;*/
		//virtual void render(HDC hdc, int drawPosX, int drawPosY) const;
		//virtual void render(HDC hdc, int drawPosX, int drawPosY, int w, int h) const;
		///*
		//��������Ʈ�� scale���Ͽ� drawPosX,drawPosY�� ����Ѵ�.
		//*/
		//virtual void render(HDC hdc, int drawPosX, int drawPosY, float scale) const;
		//virtual void renderRot(HDC hdc, int drawPosX, int drawPosY, float angle) const;
	};
}