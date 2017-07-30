#pragma once
namespace SGA {
	class GameObject
	{
	public:
		GameObject();
		virtual ~GameObject();

		virtual void update() = 0;
		virtual void render() = 0;
		virtual void release() {}
		virtual float getPositionX() const;
		virtual float getPositionY() const;
		virtual void setPositionX(float x);
		virtual void setPositionY(float y);
	private:
		float _x, _y;
	};
}