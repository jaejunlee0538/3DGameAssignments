#pragma once
#include "GameObject.h"
namespace SGA {
	class PlayerControlObject :
		public GameObject
	{
	public:
		PlayerControlObject();
		~PlayerControlObject();
		void update(bool isDirty = false) override;
	};

}