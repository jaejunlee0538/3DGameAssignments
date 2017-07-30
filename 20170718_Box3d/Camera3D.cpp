#include "Camera3D.h"
#include "cVector4.h"
#include "Renderable.h"
#include "Math3D.h"

#include <Utils.h>
#include <MathUtils.h>

namespace {
	const float MAX_X_ANGLE = PI * 0.48;
}
Camera3D::Camera3D()
{
	_distanceToTarget = 10;
	_angles = cVector3(0, 0, 0);
	_mode = RENDER_MODE_SOLID;
	_cullingMode = CULL_CCW;
	_faceColor = RGB(255, 255, 255);
	_lineColor = RGB(0, 0, 0);
	_textColor = RGB(0, 0, 0);
}

Camera3D::Camera3D(HDC hdc, float yFOV, float aspect, float zNear, float zFar, float viewPortX, float viewPortY, float width, float height)
{
	_distanceToTarget = 10;
	_angles = cVector3(0, 0, 0);
	init(hdc, yFOV, aspect, zNear, zFar, viewPortX, viewPortY, width, height);
	_mode = RENDER_MODE_SOLID;
	_cullingMode = CULL_CCW;
	_faceColor = RGB(255, 255, 255);
	_lineColor = RGB(0, 0, 0);
	_textColor = RGB(0, 0, 0);
}

Camera3D::~Camera3D()
{
}


void Camera3D::init(HDC hdc, float yFOV, float aspect, float zNear, float zFar, float viewPortX, float viewPortY, float width, float height)
{
	_hdc = hdc;
	_matCamPos = cMatrix::Identity(4);
	initViewportMatrix(viewPortX, viewPortY, width, height);
	initProjectionMatrix(yFOV, aspect, zNear, zFar);
}

void Camera3D::initViewportMatrix(float viewPortX, float viewPortY, float width, float height)
{
	_matViewport = cMatrix::Viewport(viewPortX, viewPortY, width, height, 0, 1);
}

void Camera3D::initProjectionMatrix(float yFOV, float aspect, float zNear, float zFar)
{
	_matProj = cMatrix::PerspectiveFovLH(yFOV, aspect, zNear, zFar);
}

void Camera3D::SetLineColor(COLORREF color)
{
	_lineColor = color;
}

void Camera3D::SetFaceColor(COLORREF color)
{
	_faceColor = color;
}

void Camera3D::SetTextColor(COLORREF color)
{
	_textColor = color;
}

void Camera3D::Render(Renderable & object)
{
	object.Render(*this);
}

void Camera3D::RenderTriangle(const cVector4 * vec, int NTriangles)
{
	//렌더링 설정
	HGDIOBJ oldBrush;
	COLORREF oldBrushColor;
	COLORREF oldPenColor;
	oldPenColor = SetDCPenColor(_hdc, _lineColor);
	switch (_mode) {
	case RENDER_MODE_SOLID:
		oldBrushColor = SetDCBrushColor(_hdc, _faceColor);
		break;
	case RENDER_MODE_WIREFRAME:
		oldBrush = SelectObject(_hdc, GetStockObject(NULL_BRUSH));
		break;
	}
	//렌더링 시작
	cVector4 tmp[3];
	POINT pts[3];
	cMatrix ViewProj = cMatrix::InverseTransformationMatrix(_matCamPos) * _matProj;
	//LogcMatrix(cMatrix::InverseTransformationMatrix(_matCamPos), TEXT("View"));
	switch (_cullingMode) {
	case CULL_NONE:
		for (int i = 0; i < NTriangles; ++i) {
			int idx = i * 3;
			tmp[0] = vec[idx + 0] * ViewProj;
			tmp[1] = vec[idx + 1] * ViewProj;
			tmp[2] = vec[idx + 2] * ViewProj;
			tmp[0].DivideByW();
			tmp[1].DivideByW();
			tmp[2].DivideByW();
			tmp[0] = tmp[0] * _matViewport;
			tmp[1] = tmp[1] * _matViewport;
			tmp[2] = tmp[2] * _matViewport;
			pts[0].x = tmp[0].x(); pts[0].y = tmp[0].y();
			pts[1].x = tmp[1].x(); pts[1].y = tmp[1].y();
			pts[2].x = tmp[2].x(); pts[2].y = tmp[2].y();
			Polygon(_hdc, pts, 3);
		}
		break;
	case CULL_CW:
		for (int i = 0; i < NTriangles; ++i) {
			int idx = i * 3;
			tmp[0] = vec[idx + 0] * ViewProj;
			tmp[1] = vec[idx + 1] * ViewProj;
			tmp[2] = vec[idx + 2] * ViewProj;
			tmp[0].DivideByW();
			tmp[1].DivideByW();
			tmp[2].DivideByW();
			if(cVector4::Cross(tmp[1] - tmp[0], tmp[2] - tmp[0]).z() <= 0)
				continue;
			tmp[0] = tmp[0] * _matViewport;
			tmp[1] = tmp[1] * _matViewport;
			tmp[2] = tmp[2] * _matViewport;
			pts[0].x = tmp[0].x(); pts[0].y = tmp[0].y();
			pts[1].x = tmp[1].x(); pts[1].y = tmp[1].y();
			pts[2].x = tmp[2].x(); pts[2].y = tmp[2].y();
			Polygon(_hdc, pts, 3);
		}
		break;
	case CULL_CCW:
		for (int i = 0; i < NTriangles; ++i) {
			int idx = i * 3;
			tmp[0] = vec[idx + 0] * ViewProj;
			tmp[1] = vec[idx + 1] * ViewProj;
			tmp[2] = vec[idx + 2] * ViewProj;
			tmp[0].DivideByW();
			tmp[1].DivideByW();
			tmp[2].DivideByW();
			if (cVector4::Cross(tmp[1] - tmp[0], tmp[2] - tmp[0]).z() >= 0)
				continue;
			tmp[0] = tmp[0] * _matViewport;
			tmp[1] = tmp[1] * _matViewport;
			tmp[2] = tmp[2] * _matViewport;
			pts[0].x = tmp[0].x(); pts[0].y = tmp[0].y();
			pts[1].x = tmp[1].x(); pts[1].y = tmp[1].y();
			pts[2].x = tmp[2].x(); pts[2].y = tmp[2].y();
			Polygon(_hdc, pts, 3);
		}
		break;
	}

	//렌더링 설정 초기화
	SetDCPenColor(_hdc, oldPenColor);
	switch (_mode) {
	case RENDER_MODE_SOLID:
		SetDCBrushColor(_hdc, oldBrushColor);
		break;
	case RENDER_MODE_WIREFRAME:
		SelectObject(_hdc, oldBrush);
		break;
	}
}

void Camera3D::RenderLines(const cVector4 * vertices, const int * indices, int NLines)
{
	COLORREF oldPenColor;
	oldPenColor = SetDCPenColor(_hdc, _lineColor);

	cVector4 v[2];
	cMatrix matMVPVp = cMatrix::InverseTransformationMatrix(_matCamPos) * _matProj * _matViewport;
	for (int i = 0; i < NLines; ++i) {
		v[0] = vertices[indices[i * 2]] * matMVPVp;
		v[1] = vertices[indices[i * 2 + 1]] * matMVPVp;
		v[0].DivideByW();
		v[1].DivideByW();
		MoveToEx(_hdc, v[0].x(), v[0].y(), NULL);
		LineTo(_hdc, v[1].x(), v[1].y());
	}

	SetDCPenColor(_hdc, oldPenColor);
}

void Camera3D::RenderText(float x, float y, float z, const TCHAR * text)
{
	cVector4 textPos(x, y, z, 1);
	textPos = textPos *  cMatrix::InverseTransformationMatrix(_matCamPos) * _matProj * _matViewport;
	textPos.DivideByW();
	COLORREF oldColor = ::SetTextColor(_hdc, _textColor);
	TextOut(_hdc, textPos.x(), textPos.y(), text, _tcslen(text));
	::SetTextColor(_hdc, oldColor);
}

void Camera3D::RenderCameraInfo(int x, int y)
{
	TCHAR buffer[1024];
	COLORREF oldColor = ::SetTextColor(_hdc, _textColor);
	_stprintf_s(buffer, 1024, TEXT("위치 : %.3f %.3f %.3f"), GetX(), GetY(), GetZ());
	TextOut(_hdc, x, y, buffer, _tcslen(buffer));
	_stprintf_s(buffer, 1024, TEXT("각도(도) : %.1f %.1f %.1f"), SGA::rad2deg(_angles.x()), SGA::rad2deg(_angles.y()), SGA::rad2deg(_angles.z()));
	TextOut(_hdc, x, y+20, buffer, _tcslen(buffer));
	::SetTextColor(_hdc, oldColor);
}

void Camera3D::IncrementDistanceToTarget(float dz)
{
	_distanceToTarget += dz;
	if (_distanceToTarget < 3) {
		_distanceToTarget = 3;
	}
}

void Camera3D::SetPosition(const float & x, const float & y, const float & z)
{
	_matCamPos(3, 0) = x;
	_matCamPos(3, 1) = y;
	_matCamPos(3, 2) = z;
}

void Camera3D::FollowTarget(const cVector3 & targetPos)
{
	cVector4 lookVec(0, 0, _distanceToTarget, 0);
	cMatrix rot = cMatrix::RotationX(_angles.x()) * cMatrix::RotationY(_angles.y()) * cMatrix::RotationZ(_angles.z());
	lookVec = lookVec * rot;
	SetPosition(targetPos.x() - lookVec.x(), targetPos.y() - lookVec.y(), targetPos.z() - lookVec.z());
}

void Camera3D::Translate(float dx, float dy, float dz)
{
	_matCamPos = _matCamPos * cMatrix::Translation(dx, dy, dz);
	//LogcMatrix(_matWorld, TEXT("World"));
}

void Camera3D::TranslateLocal(float dx, float dy, float dz)
{
	cVector4 translation = cVector4(dx, dy, dz, 0) * _matCamPos;
	Translate(translation.x(), translation.y(), translation.z());
}

void Camera3D::Rotate(float dx, float dy, float dz)
{
	_angles.x() += dx;
	//if (_angles.x() > MAX_X_ANGLE)
	//	_angles.x() = MAX_X_ANGLE;
	//if (_angles.x() < -MAX_X_ANGLE)
	//	_angles.x() = -MAX_X_ANGLE;

	_angles.y() += dy;
	_angles.z() += dz;
	_angles.y() = SGA::normalizeAngle(_angles.y());
	_angles.z() = SGA::normalizeAngle(_angles.z());
	
	cMatrix rot = cMatrix::RotationX(_angles.x()) * cMatrix::RotationY(_angles.y()) * cMatrix::RotationZ(_angles.z());
	//cMatrix rot = cMatrix::RotationZ(_angles.z()) * cMatrix::RotationY(_angles.y())*  cMatrix::RotationX(_angles.x());

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; j++) {
			_matCamPos(i, j) = rot(i, j);
		}
	}
}

void Camera3D::SetAngles(const float & x, const float & y, const float & z)
{
	_angles = cVector3(x, y, z);
	cMatrix rot = cMatrix::RotationZ(z) * cMatrix::RotationY(y) * cMatrix::RotationX(x);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			_matCamPos(i, j) = rot(i, j);
		}
	}
}

void Camera3D::MakeLookAt(const cVector3 & target)
{
	cVector3 position(_matCamPos(3, 0), _matCamPos(3, 1), _matCamPos(3, 2));
	_matCamPos = cMatrix::CameraPosition(position, target - position, cVector3(0, 1, 0));
	LogcMatrix(_matCamPos, TEXT("Camera"));
}

