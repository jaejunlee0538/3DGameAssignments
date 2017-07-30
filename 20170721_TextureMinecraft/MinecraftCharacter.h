#pragma once
class cBoxTextured;
class MinecraftCharacter
{
public:
	MinecraftCharacter();
	~MinecraftCharacter();
	void Setup(const char* textureFile);
	void Update(SGA::UserInputData& input);
	void Render();
	D3DXVECTOR3 GetPosition() const;
private:
	IDirect3DTexture9 *m_texture;
	cBoxTextured * m_head = nullptr;
	cBoxTextured * m_body = nullptr;
	cBoxTextured * m_armL = nullptr;
	cBoxTextured * m_armR = nullptr;

	cBoxTextured * m_legL = nullptr;
	cBoxTextured * m_legR= nullptr;

};

