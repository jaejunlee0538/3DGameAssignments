#include "StdAfx.h"
#include "cDeviceManager.h"

cDeviceManager::~cDeviceManager(void)
{
}

cDeviceManager::cDeviceManager(void)
	: m_pD3D(NULL)
	, m_pD3DDevice(NULL)
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9	stCaps;
	int			nVertexProcessing;
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &stCaps);
	if(stCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		nVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		nVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	//////////////////////////////////////////////////////////////////////
	
	//stCaps.
	//if(FAILED(m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, )))

	D3DPRESENT_PARAMETERS stD3DPP;
	ZeroMemory(&stD3DPP, sizeof(D3DPRESENT_PARAMETERS));
	stD3DPP.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	stD3DPP.Windowed				= FALSE;
	stD3DPP.BackBufferFormat		= D3DFMT_A8R8G8B8;
	stD3DPP.EnableAutoDepthStencil	= TRUE;
	stD3DPP.AutoDepthStencilFormat	= D3DFMT_D16;
	stD3DPP.hDeviceWindow = g_hWnd;
	stD3DPP.BackBufferWidth = 1920;
	stD3DPP.BackBufferHeight = 1080;

	DWORD quality;
	//if (m_pD3D->CheckDeviceMultiSampleType(stCaps.AdapterOrdinal, stCaps.DeviceType, D3DFMT_UNKNOWN, TRUE, D3DMULTISAMPLE_4_SAMPLES, &quality)
	//	&& m_pD3D->CheckDeviceMultiSampleType(stCaps.AdapterOrdinal, stCaps.DeviceType, D3DFMT_D16, TRUE, D3DMULTISAMPLE_4_SAMPLES, &quality)) {
	//	stD3DPP.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
	//	stD3DPP.MultiSampleQuality = quality;
	//	LogDebugMessage("Multisampling 지원\n");
	//}
	//////////////////////////////////////////////////////////////////////
	m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		g_hWnd,
		nVertexProcessing,
		&stD3DPP,
		&m_pD3DDevice);
}




LPDIRECT3DDEVICE9 cDeviceManager::GetDevice()
{
	return m_pD3DDevice;
}

void cDeviceManager::Destroy()
{
	SAFE_RELEASE(m_pD3D);
	ULONG ul = m_pD3DDevice->Release();
	assert(ul == 0 && "삭제되지 않은 COM 객체가 있습니다.");
}
