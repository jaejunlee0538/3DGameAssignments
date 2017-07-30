#pragma once
#include "cMatrix.h"
#include "cVector3.h"
#include <Windows.h>
class Renderable;
class Camera3D
{
public:
	enum RenderModes {
		RENDER_MODE_SOLID,
		RENDER_MODE_WIREFRAME
	};
	enum BackfaceCullingModes {
		CULL_CW,
		CULL_CCW,
		CULL_NONE
	};
	Camera3D();
	Camera3D(HDC hdc, float yFOV, float aspect, float zNear, float zFar, float viewPortX, float viewPortY, float width, float height);
	~Camera3D();

	void init(HDC hdc, float yFOV, float aspect, float zNear, float zFar, float viewPortX, float viewPortY, float width, float height);
	void initViewportMatrix(float viewPortX, float viewPortY, float width, float height);
	void initProjectionMatrix(float yFOV, float aspect, float zNear, float zFar);

	void Render(Renderable& object);

	void SetCullMode(BackfaceCullingModes cullMode) { _cullingMode = cullMode; }
	void SetRenderMode(RenderModes mode) { _mode = mode; }
	void SetLineColor(COLORREF color);
	void SetFaceColor(COLORREF color);
	void SetTextColor(COLORREF color);
	void RenderTriangle(const cVector4 * vec, int NTriangles);
	void RenderLines(const cVector4* vertices, const int* indices, int NLines);
	void RenderText(float x, float y, float z, const TCHAR* text);
	void RenderCameraInfo(int x, int y);

	void IncrementDistanceToTarget(float dz);

	const float& GetX() const { return _matCamPos(3, 0); }
	const float& GetY() const { return _matCamPos(3, 1); }
	const float& GetZ() const { return _matCamPos(3, 2); }
	void SetPosition(const float& x, const float& y, const float& z);

	void FollowTarget(const cVector3& targetPos);

	void Translate(float dx, float dy, float dz);
	void TranslateLocal(float dx, float dy, float dz);
	
	void Rotate(float dx, float dy, float dz);

	void SetAngles(const float& x, const float& y, const float& z);
	void MakeLookAt(const cVector3& target);
private:
	float _distanceToTarget;
	cVector3 _angles;
	
	cMatrix _matCamPos;

	cMatrix _matProj;
	cMatrix _matViewport;

	BackfaceCullingModes _cullingMode;
	RenderModes _mode;
	HDC _hdc;
	COLORREF _faceColor;
	COLORREF _lineColor;
	COLORREF _textColor;
};