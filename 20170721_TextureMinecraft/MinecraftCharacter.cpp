#include "stdafx.h"
#include "MinecraftCharacter.h"
#include "cBoxTextured.h"

MinecraftCharacter::MinecraftCharacter()
{
}

MinecraftCharacter::~MinecraftCharacter()
{
}

void MinecraftCharacter::Setup(const char * textureFile)
{
	D3DXCreateTextureFromFile(g_pD3DDevice, textureFile, &m_texture);
	m_head = new cBoxTextured();
	m_head->init(2, 2, 2, D3DXVECTOR2(0, 0), D3DXVECTOR3(2.0/16.0, 2.0/8.0, 2.0/8.0));
	m_body = new cBoxTextured();
	m_body->init(2, 3, 1, D3DXVECTOR2(0.25, 0.5), D3DXVECTOR3(2.0 / 16.0, 3.0 / 8.0, 1.0 / 8.0));
	m_armL = new cBoxTextured();
	m_armL->init(1, 3, 1, D3DXVECTOR2(0, 0.5), D3DXVECTOR3(1.0 / 16.0, 3.0 / 8.0, 1.0 / 8.0));
	m_armR = new cBoxTextured();
	m_armR->init(1, 3, 1, D3DXVECTOR2(0, 0.5), D3DXVECTOR3(1.0 / 16.0, 3.0 / 8.0, 1.0 / 8.0));
	m_legR = new cBoxTextured();
	m_legR->init(1, 3, 1, D3DXVECTOR2(10.0 / 16.0, 0.5), D3DXVECTOR3(1.0 / 16.0, 3.0 / 8.0, 1.0 / 8.0));
	m_legL = new cBoxTextured();
	m_legL->init(1, 3, 1, D3DXVECTOR2(10.0 / 16.0, 0.5), D3DXVECTOR3(1.0 / 16.0, 3.0 / 8.0, 1.0 / 8.0));

	m_head->SetPosition(0, 7, 0);
	m_body->SetPosition(0, 4.5, 0);
	m_armL->SetPosition(-1.5, 4.5, 0);
	m_armR->SetPosition(1.5, 4.5, 0);
	m_legL->SetPosition(-0.5, 1.5, 0);
	m_legR->SetPosition(0.5, 1.5, 0);
}

void MinecraftCharacter::Update(SGA::UserInputData& input)
{
	if (input.hasInput(SGA::LEFT_KEY_STAYDOWN)) {
		m_body->Rotate(0, 0.05f, 0);
	}
	if (input.hasInput(SGA::RIGHT_KEY_STAYDOWN)) {
		m_body->Rotate(0, -0.05f, 0);
	}
}

void MinecraftCharacter::Render()
{
	g_pD3DDevice->SetTexture(0, m_texture);
	m_head->Render();
	m_body->Render();
	m_armL->Render();
	m_armR->Render();
	m_legL->Render();
	m_legR->Render();
}

D3DXVECTOR3 MinecraftCharacter::GetPosition() const
{
	return m_head->GetPosition();
}
