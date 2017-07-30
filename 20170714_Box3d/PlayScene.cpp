#include "stdafx.h"
#include "PlayScene.h"
#include <KeyManager.h>
#include <functional>
#include <MathUtils.h>
#include <Utils.h>
#include <sstream>
#include <tstring.h>
#include <CommonMacroHeader.h>
PlayScene::PlayScene()
{

}

PlayScene::~PlayScene()
{
}

HRESULT PlayScene::init()
{
	SGA::GameNode::init();
	//m_BoxPoints = { cVector3(1,1,1),cVector3(1,1,-1),cVector3(1,-1,-1),
	//	cVector3(1,1,1),cVector3(1,-1,-1),cVector3(1,-1,1)};
	m_BoxPoints = { cVector3(1,1,1),cVector3(1,1,-1),cVector3(1,-1,-1),
		cVector3(1,1,1),cVector3(1,-1,-1),cVector3(1,-1,1),
		cVector3(1,1,1),cVector3(1,-1,1),cVector3(-1,-1,1),
		cVector3(1,1,1),cVector3(-1,-1,1),cVector3(-1,1,1),
		cVector3(1,1,1),cVector3(-1,1,-1),cVector3(1,1,-1),
		cVector3(1,1,1),cVector3(-1,1,1),cVector3(-1,1,-1),
		cVector3(-1,-1,-1),cVector3(1,-1,1),cVector3(1,-1,-1),
		cVector3(-1,-1,-1),cVector3(-1,-1,1),cVector3(1,-1,1),
		cVector3(-1,-1,-1),cVector3(1,-1,-1),cVector3(1,1,-1),
		cVector3(-1,-1,-1),cVector3(1,1,-1),cVector3(-1,1,-1),
		cVector3(-1,-1,-1),cVector3(-1,1,-1),cVector3(-1,1,1),
		cVector3(-1,-1,-1),cVector3(-1,1,1),cVector3(-1,-1,1) };
	m_BoxPos = cVector3(0, 0, 0);
	m_CamPos = cVector3(0, 0, 5 );

	m_Pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	return S_OK;
}

void PlayScene::update(void)
{
	GameNode::update();
	m_LookDir = (m_BoxPos - m_CamPos).Normalize();
	cVector3 camRight = cVector3::Cross(cVector3(0,1,0),m_LookDir);
	if (GET_KEY_MANAGER()->isStayKeyDown(VK_UP)) {
		m_CamPos = m_CamPos + m_LookDir*0.03f;
	}
	if (GET_KEY_MANAGER()->isStayKeyDown(VK_DOWN)) {
		m_CamPos = m_CamPos - m_LookDir*0.03f;
	}
	if (GET_KEY_MANAGER()->isStayKeyDown(VK_LEFT)) {
		m_CamPos = m_CamPos + camRight*0.03f;
	}
	if (GET_KEY_MANAGER()->isStayKeyDown(VK_RIGHT)) {
		m_CamPos = m_CamPos - camRight*0.03f;
	}
	if (GET_KEY_MANAGER()->isStayKeyDown('A')) {
		m_CamPos = m_CamPos + cVector3(0.3f, 0, 0);
	}
	if (GET_KEY_MANAGER()->isStayKeyDown('D')) {
		m_CamPos = m_CamPos - cVector3(0.3f, 0, 0);
	}
	if (GET_KEY_MANAGER()->isStayKeyDown('W')) {
		m_BoxPos.y() += 0.3f;
	}
	if (GET_KEY_MANAGER()->isStayKeyDown('S')) {
		m_BoxPos.y() -= 0.3f;
	}
	m_LookDir = (m_BoxPos - m_CamPos).Normalize();

	//SGA::LogDebugMessage(TEXT("CamPos : %.4f %.4f %.4f\n"), m_CamPos.x(), m_CamPos.y(), m_CamPos.z());
}

void PlayScene::release(void)
{
	
}

void LogMatrices(const cMatrix& world, const cMatrix& view, const cMatrix& proj, const cMatrix& viewport) {
	std::ostringstream oss;
	oss << "World : \n"<<world<<"================"<<std::endl;
	oss << "View : \n" << view << "================" << std::endl;
	oss << "Proj : \n" << proj << "================" << std::endl;
	oss << "Viewport : \n" << viewport << "================" << std::endl;
	oss << "AllInOne : \n" << world*view*proj*viewport << "================" << std::endl;
	SGA::LogDebugMessage(TEXT("%s"), SGA::convertToTString(oss.str()).c_str());
}
void PlayScene::render(void)
{
	static float yAngle = 0;
	//yAngle += 0.003f;
	SGA::GameNode::clearScreen(RGB(0, 0, 0));
	int NTriangles = m_BoxPoints.size() / 3;
	cMatrix MatWorld = cMatrix::Scaling(1,1,1) * cMatrix::RotationZ(yAngle)*cMatrix::RotationY(0)*cMatrix::RotationX(0)*cMatrix::Translation(m_BoxPos);
	cMatrix MatView = cMatrix::LookAtLH(m_CamPos, (m_BoxPos - m_CamPos).Normalize(), cVector3(0, 1, 0));
	cMatrix MatProj = cMatrix::PerspectiveFovLH(SGA::deg2rad(55.0f), ((float)WIN_SIZE_X)/WIN_SIZE_Y, 1, 1000);
	cMatrix MatViewport = cMatrix::Viewport(0, 0, WIN_SIZE_X, WIN_SIZE_Y, 0, 1.0f);
	cMatrix MatAllInOne = MatWorld * MatView * MatProj;
	LogMatrices(MatWorld, MatView, MatProj, MatViewport);
	auto oldPen = SelectObject(getMemDC(), m_Pen);
	auto oldBrush = SelectObject(getMemDC(), GetStockObject(NULL_BRUSH));
	for (int i = 0; i < NTriangles; ++i) {
		cVector4 vp1 = cVector4(m_BoxPoints[i * 3], 1) * MatAllInOne;
		cVector4 vp2 = cVector4(m_BoxPoints[i * 3 + 1], 1) * MatAllInOne;
		cVector4 vp3 = cVector4(m_BoxPoints[i * 3 + 2], 1) * MatAllInOne;

		vp1 = vp1 * MatViewport;
		vp2 = vp2 * MatViewport;
		vp3 = vp3 * MatViewport;
		
		vp1.DivideByW();
		vp2.DivideByW();
		vp3.DivideByW();

		POINT pts[3];
		pts[0].x = vp1.x(); pts[0].y = vp1.y();
		pts[1].x = vp2.x(); pts[1].y = vp2.y();
		pts[2].x = vp3.x(); pts[2].y = vp3.y();
	
		Polygon(getMemDC(), pts, 3);
		
		//MoveToEx(getMemDC(), vp1.x(), vp1.y(), NULL);
		//LineTo(getMemDC(), vp2.x(), vp2.y());
		//LineTo(getMemDC(), vp3.x(), vp3.y());
		//LineTo(getMemDC(), vp1.x(), vp1.y());
	}
	SelectObject(getMemDC(), oldPen);
	SelectObject(getMemDC(), oldBrush);
}
