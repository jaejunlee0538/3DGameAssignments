#include "stdafx.h"
#include "cMainGame.h"

namespace SGA {
	cMainGame::cMainGame()
		:m_pD3D(NULL), m_pD3DDevice(NULL), m_freq(1.0)
	{
		
	}

	cMainGame::~cMainGame()
	{
		SAFE_RELEASE(m_pFont);
		SAFE_RELEASE(m_pD3D);
		if (m_pD3DDevice) {
			ULONG refCount = m_pD3DDevice->Release();
			assert(refCount == 0 && "COM 객체가 모두 반환되지 않았습니다.");
		}
	}

	void cMainGame::Setup()
	{
		//https://en.wikibooks.org/wiki/DirectX/9.0/Direct3D/Initialization
		m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

		BOOL windowed = FALSE;
		UINT adapterNum = -1;
		//내꺼에서는 하나밖에 안뜨네...(내장 아니면 외장)
		UINT nAdapters = m_pD3D->GetAdapterCount();
		std::vector<D3DADAPTER_IDENTIFIER9> adapterIdentifiers(nAdapters);
		for (int i = 0; i < nAdapters; ++i) {
			m_pD3D->GetAdapterIdentifier(i, 0, &adapterIdentifiers[i]);
			LogDebugMessage("%s\n", adapterIdentifiers[i].DeviceName);
			LogDebugMessage("%s\n", adapterIdentifiers[i].Description);
		}
		assert(nAdapters > 0 && "로그와 함께 프로그램 종료 시켜야 함");

		adapterNum = 0;
	
		D3DFORMAT fmtArr[] = { D3DFMT_X8R8G8B8, D3DFMT_X1R5G5B5, D3DFMT_R5G6B5,D3DFMT_X8R8G8B8, D3DFMT_X1R5G5B5, D3DFMT_R5G6B5 };
		D3DDEVTYPE devArr[] = { D3DDEVTYPE_HAL ,D3DDEVTYPE_HAL ,D3DDEVTYPE_HAL,D3DDEVTYPE_REF,D3DDEVTYPE_REF,D3DDEVTYPE_REF };

		size_t iConfig ;
		for (iConfig = 0; iConfig < sizeof(fmtArr); ++iConfig) {
			if (SUCCEEDED(m_pD3D->CheckDeviceType(adapterNum, devArr[iConfig], fmtArr[iConfig], fmtArr[iConfig], windowed))) {
				break;
			}
		}
		assert(iConfig < sizeof(fmtArr) && "적절한 디바이스를 찾지 못했음");

		switch (devArr[iConfig]) {
		case D3DDEVTYPE_HAL:
			LogDebugMessage("DeviceType : HAL\n");
			break;
		case D3DDEVTYPE_REF:
			LogDebugMessage("DeviceType : REF");
			break;
		}
		
		switch (fmtArr[iConfig]) {
		case D3DFMT_X8R8G8B8:
			LogDebugMessage("AdapterFormat : X8R8G8B8\n");
			break;
		case D3DFMT_X1R5G5B5:
			LogDebugMessage("AdapterFormat : X1R5G5B5\n");
			break;
		case D3DFMT_R5G6B5:
			LogDebugMessage("AdapterFormat : R5G6B5\n");
			break;
		}

		D3DCAPS9 stCaps;
		int nVertexProcessing;
		
		m_pD3D->GetDeviceCaps(adapterNum, devArr[iConfig], &stCaps);

		if (stCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
			LogDebugMessage("Vertex Processing : Hardware\n");
			nVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
#ifdef NDEBUG
			if (stCaps.DevCaps & D3DDEVCAPS_PUREDEVICE) {
				//속도는 빠르지만 디버깅할 떄 안좋음.
				LogDebugMessage("Enable PureDevice\n");
				nVertexProcessing |= D3DCREATE_PUREDEVICE;
			}
#endif
		}
		else {
			LogDebugMessage("Vertex Processing : Software\n");
			nVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		//TODO : 이게 뭐지?
		int presentInterval = D3DPRESENT_INTERVAL_DEFAULT;
		//if (stCaps.PresentationIntervals & D3DPRESENT_INTERVAL_FOUR) {
		//	presentInterval = D3DPRESENT_INTERVAL_FOUR;
		//}
		
		//깊이-스텐실 버퍼 포맷 
		D3DFORMAT depthstencil = D3DFMT_D24S8;
		if (FAILED(m_pD3D->CheckDeviceFormat(adapterNum, devArr[iConfig], fmtArr[iConfig], D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, depthstencil))) {
			//게임 구현에 필요한 Depth-Stencil버퍼를 이요할 수 없기 때문에 게임을 실행할 수 없는건가?
			assert(false);
			return;
		}
		
		//멀티 샘플링
		D3DMULTISAMPLE_TYPE multiSampleType = D3DMULTISAMPLE_NONE;
		DWORD maxQuality = 0;
		for (int i = D3DMULTISAMPLE_2_SAMPLES; i < D3DMULTISAMPLE_16_SAMPLES; ++i) {
			D3DMULTISAMPLE_TYPE mst= (D3DMULTISAMPLE_TYPE)i;
			DWORD quality;
			if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(adapterNum, devArr[iConfig], fmtArr[iConfig], windowed, mst, &quality))) {
				if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(adapterNum, devArr[iConfig], depthstencil, windowed, mst, &quality))) {
					LogDebugMessage("%d Sample Supported\n", mst);
					multiSampleType = mst;
					maxQuality = quality - 1;
				}
			}
		}

		//
		D3DDISPLAYMODE bestmode;
		UINT nAdapterMode = m_pD3D->GetAdapterModeCount(adapterNum, fmtArr[iConfig]);
		assert(nAdapterMode > 0);
		m_pD3D->EnumAdapterModes(adapterNum, fmtArr[iConfig], 0, &bestmode);
		for (UINT i = 1; i < nAdapterMode; i++)
		{
			D3DDISPLAYMODE dispmode;
			m_pD3D->EnumAdapterModes(adapterNum, fmtArr[iConfig], i, &dispmode);
			if (dispmode.Width > bestmode.Width)
			{
				bestmode.Width = dispmode.Width;
				bestmode.Height = dispmode.Height;
				bestmode.RefreshRate = dispmode.RefreshRate;
				continue;
			}
			if (dispmode.Height > bestmode.Height)
			{
				bestmode.Height = dispmode.Height;
				bestmode.RefreshRate = dispmode.RefreshRate;
				continue;
			}
			if (dispmode.RefreshRate > bestmode.RefreshRate)
			{
				bestmode.RefreshRate = dispmode.RefreshRate;
				continue;
			}
		}
		LogDebugMessage("Width X Height : %u X %u\n", bestmode.Width, bestmode.Height);
		LogDebugMessage("RefreshRate : %u\n", bestmode.RefreshRate);

		D3DPRESENT_PARAMETERS stD3DPP;
		ZeroMemory(&stD3DPP, sizeof(D3DPRESENT_PARAMETERS));
		stD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
		stD3DPP.Windowed = windowed;
		stD3DPP.BackBufferFormat = fmtArr[iConfig];
		stD3DPP.EnableAutoDepthStencil = TRUE;
		stD3DPP.AutoDepthStencilFormat = depthstencil;
		stD3DPP.PresentationInterval = presentInterval;
		stD3DPP.MultiSampleType = multiSampleType;
		stD3DPP.MultiSampleQuality = maxQuality;
		if (!windowed) {
			stD3DPP.BackBufferHeight = bestmode.Height;
			stD3DPP.BackBufferWidth = bestmode.Width;
			stD3DPP.FullScreen_RefreshRateInHz = bestmode.RefreshRate;
		}
		
		HRESULT ret = m_pD3D->CreateDevice(adapterNum,
			devArr[iConfig],
			g_hWnd,
			nVertexProcessing,
			&stD3DPP,
			&m_pD3DDevice);
		if (FAILED(ret)) {
			LogDebugMessage("CreateDevice Error : %s\n", DXGetErrorDescriptionA(ret));
		}
		g_pD3DDevice = m_pD3DDevice;

		//////////////////////////////////////////////////////////////////////
		D3DXFONT_DESC fd;
		ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
		fd.Height = 50;
		fd.Width = 25;
		fd.Weight = FW_NORMAL;
		fd.Italic = false;
		fd.CharSet = DEFAULT_CHARSET;
		fd.OutputPrecision = OUT_DEFAULT_PRECIS;
		fd.PitchAndFamily = FF_DONTCARE;
		//AddFontResource("umberto.ttf");
		strcpy_s(fd.FaceName, "고딕");	//글꼴 스타일
		D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);
		//////////////////////////////////////////////////////////////////////
		RECT screenRect;
		GetClientRect(g_hWnd, &screenRect);
		m_camera.Setup(D3DXToRadian(55), screenRect.right / (float)screenRect.bottom, 1, 1000);
		m_camera.SetAngle(D3DXToRadian(45), D3DXToRadian(180), 0);
		m_camera.SetDistanceToTarget(6);
		m_camera.SetLookTargetPosition(D3DXVECTOR3(0, 0, 0));
		
		m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_camera.GetLookAtMatrix());
		m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_camera.GetProjectionMatrix());
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
		m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
		m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

		_grid.init(30, 30, 1, 1);

		m_counter.Start();
	}

	void cMainGame::Update()
	{
		auto input = InputManager::getSingleton()->GenerateUserInput();
		m_camera.Update(input);
		g_pD3DDevice->SetTransform(D3DTS_VIEW, &(m_camera.GetLookAtMatrix()));
	}

	void cMainGame::Render()
	{
		auto time = 0.001 * m_counter.GetElapsedTime();
		m_freq.update(time);

		m_pD3DDevice->Clear(NULL,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(0, 0, 0),
			1.0f, 0);

		m_pD3DDevice->BeginScene();
		_grid.render();

		char msg[100];
		sprintf_s(msg, 100, "FPS : %.1lf fps", (double)m_freq.getFPS());
		RECT rect;
		rect.left = rect.right = 10;
		rect.top = rect.bottom = 10;
		m_pFont->DrawTextA(
			NULL, msg, strlen(msg), &rect, DT_LEFT | DT_TOP | DT_NOCLIP,
			D3DCOLOR_XRGB(255, 255, 255));

		m_pD3DDevice->EndScene();
		m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	}
}

