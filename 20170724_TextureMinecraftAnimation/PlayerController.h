#pragma once
namespace SGA {
	class MinecraftCharacter;
	class PlayerController
	{
	public:
		PlayerController();
		~PlayerController();

		void setPlayer(MinecraftCharacter* player);
		void update(UserInputData& input);

	private:
		MinecraftCharacter* _player = nullptr;

	};

}