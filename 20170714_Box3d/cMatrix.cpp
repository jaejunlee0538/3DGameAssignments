#include "stdafx.h"
#include "cMatrix.h"
#include <float.h>
#include <assert.h>
#include <cmath>
#include "cVector3.h"
#include "cVector4.h"
#include <iomanip>
#define DETERMINANT_THRESHOLD	0.00001f
namespace {
	inline int IDX(int row, int col, int N) {
		return col + row * N;
	}
}

cMatrix::cMatrix()
{
}

cMatrix::cMatrix(int dim, float initVal)
{
	init(dim, initVal);
}

cMatrix::~cMatrix()
{
}

void cMatrix::init(int N)
{
	_rows.resize(N,cRow(N));
}

void cMatrix::init(int N, float val)
{
	_rows.resize(N, cRow(N, val));
}

void cMatrix::ClearValue(float val)
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	for (int i = 0; i < _rows.size(); ++i) {
		_rows[i].setValues(val);
	}
}

void cMatrix::MultScalar(float s)
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	for (int i = 0; i < _rows.size(); ++i) {
		_rows[i].multScalar(s);
	}
}

cMatrix cMatrix::Transpose()
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	cMatrix transposed;
	transposed.init(_rows.size());
	//대각성분 상단에 for문을 돈다.
	for (int iRow = 0; iRow < _rows.size(); ++iRow) {
		for (int iCol = iRow; iCol < _rows.size(); ++iCol) {
			transposed(iCol, iRow) = Get(iRow, iCol);
			if(iCol != iRow)//대각선 성분은 Skip!
				transposed(iRow, iCol) = Get(iCol, iRow);
		}
	}
	return transposed;
}

cMatrix cMatrix::Inverse()
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	//assert(IsIvertible() && "행렬식이 너무 작습니다.");
	cMatrix C;
	C.init(_rows.size());
	for (int iRow = 0; iRow < _rows.size(); ++iRow) { 
		for (int iCol = 0; iCol < _rows.size(); ++iCol) {
			C(iRow, iCol) = GetCofactor(iRow, iCol);
		}
	}
	float det = 0.0;
	for (int i = 0; i < _rows.size(); ++i) {
		det += Get(0, i)*C(0, i);
	}
	assert(std::abs(det) > DETERMINANT_THRESHOLD && "행렬식이 너무 작습니다.");
	return C.Transpose() * (1.0f / det);
}

bool cMatrix::IsIvertible() const
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	return std::abs(GetDeterminant()) > DETERMINANT_THRESHOLD;
}

float & cMatrix::Get(int iRow, int iCol)
{
	assert(IDX(iRow, iCol, _rows.size()) < _rows.size()*_rows.size() && "배열의 범위를 초과하였습니다");
	return _rows[iRow][iCol];
}

const float & cMatrix::Get(int iRow, int iCol) const
{
	assert(IDX(iRow, iCol, _rows.size()) < _rows.size()*_rows.size() && "배열의 범위를 초과하였습니다");
	return _rows[iRow][iCol];
}

float cMatrix::GetDeterminant() const
{
	assert(IsInitialized() && "행렬이 초기화되지 않았습니다.");
	switch (GetDimension()) {
	case 1:
		return Get(0,0);
	case 2:
		return Get(0, 0)*Get(1, 1) - Get(1, 0)*Get(0, 1);
	default:
	{
		float det = 0.0;
		for (int iCol = 0; iCol < _rows.size(); ++iCol) {
			det += Get(0, iCol) * GetCofactor(0, iCol);
		}
		return det;
	}
	}
}

float cMatrix::GetCofactor(int iRow, int iCol) const
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	int sgn = ((iRow + iCol) & 1) ? -1 : 1;
	return sgn * GetPartialMatrix(iRow, iCol).GetDeterminant();
}

cMatrix cMatrix::GetPartialMatrix(int iRowSkip, int iColSkip) const
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	cMatrix m;
	m.init(_rows.size() - 1);
	int iRowOut=0, iColOut=0;
	for (int iRow = 0; iRow < _rows.size(); ++iRow) {
		if (iRow == iRowSkip) {
			continue;
		}
		iColOut = 0;
		for (int iCol = 0; iCol < _rows.size(); ++iCol) {
			if (iCol == iColSkip) {
				continue;
			}
			m(iRowOut, iColOut) = (*this)(iRow, iCol);
			iColOut++;
		}
		iRowOut++;
	}
	return m;
}

bool cMatrix::operator==(const cMatrix & m) const
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	assert(m.IsInitialized() && "두번째 행렬이 초기화되지 않았습니다.");
	if (GetDimension() != m.GetDimension())
		return false;
	for (int i = 0; i < _rows.size(); ++i) {
		for (int k = 0; k < _rows.size(); ++k) {
			if (std::abs(_rows[i][k] - m._rows[i][k]) > FLT_EPSILON)
				return false;
		}
	}
	return true;
}

bool cMatrix::operator!=(const cMatrix & m) const
{
	return !(*this == m);
}

cMatrix cMatrix::operator*(float scalar) const
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	cMatrix m = *this;
	m.MultScalar(scalar);
	return m;
}

cMatrix cMatrix::operator*(const cMatrix & m) const
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	assert(m.IsInitialized() && "두번째 행렬이 초기화되지 않았습니다.");
	assert(GetDimension() == m.GetDimension() && "곱할 수 없는 행렬입니다.");
	cMatrix m3;
	m3.init(_rows.size());
	const cMatrix& m1 = *this;
	const cMatrix& m2 = m;

	for (int iRow = 0; iRow < _rows.size(); ++iRow) {
		for (int iCol = 0; iCol < _rows.size(); ++iCol) {
			float v = 0.0f;
			for (int i = 0; i < _rows.size(); ++i) {
				v += m1(iRow, i) * m2(i, iCol);
			}
			m3(iRow, iCol) = v;
		}
	}
	return m3;
}

cMatrix cMatrix::operator+(const cMatrix & m) const
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	assert(m.IsInitialized() && "두번째 행렬이 초기화되지 않았습니다.");
	assert(GetDimension() == m.GetDimension() && "더할 수 없는 행렬입니다.");
	cMatrix m3;
	m3.init(_rows.size());
	const cMatrix& m1 = *this;
	const cMatrix& m2 = m;
	for (int iRow = 0; iRow < _rows.size(); ++iRow) {
		for (int iCol = 0; iCol < _rows.size(); ++iCol) {
			m3(iRow, iCol) = m1(iRow, iCol) + m2(iRow, iCol);
		}
	}
	return m3;
}

cMatrix cMatrix::operator-(const cMatrix & m) const
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	assert(m.IsInitialized() && "두번째 행렬이 초기화되지 않았습니다.");
	assert(GetDimension() == m.GetDimension() && "뺄 수 없는 행렬입니다.");
	cMatrix m3;
	m3.init(_rows.size());
	const cMatrix& m1 = *this;
	const cMatrix& m2 = m;
	for (int iRow = 0; iRow < _rows.size(); ++iRow) {
		for (int iCol = 0; iCol < _rows.size(); ++iCol) {
			m3(iRow, iCol) = m1(iRow, iCol) - m2(iRow, iCol);
		}
	}
	return m3;
}

cMatrix cMatrix::Identity(int N)
{
	cMatrix identity;
	identity.init(N, 0);
	for (int i = 0; i < N; ++i) {
		identity(i, i) = 1;
	}
	return identity;
}

cMatrix cMatrix::Translation(float x, float y, float z)
{
	cMatrix mat(4, 0);
	mat(3, 0) = x;
	mat(3, 1) = y;
	mat(3, 2) = z;
	mat(3, 3) = 1;
	return mat;
}

cMatrix cMatrix::Translation(const cVector3 & v)
{
	cMatrix mat = cMatrix::Identity(4);
	mat(3, 0) = v.x();
	mat(3, 1) = v.y();
	mat(3, 2) = v.z();
	return mat;
}

cMatrix cMatrix::RotationX(float fAngle)
{
	float c = cos(fAngle), s = sin(fAngle);
	cMatrix mat = cMatrix::Identity(4);
	mat(1, 1) = c; mat(1, 2) = s;
	mat(2, 1) = -s; mat(2, 2) = c;
	return mat;
}

cMatrix cMatrix::RotationY(float fAngle)
{
	float c = cos(fAngle), s = sin(fAngle);
	cMatrix mat = cMatrix::Identity(4);
	mat(0, 0) = c; mat(2, 0) = -s;
	mat(2, 0) = s; mat(2, 2) = c;
	return mat;
}

cMatrix cMatrix::RotationZ(float fAngle)
{
	float c = cos(fAngle), s = sin(fAngle);
	cMatrix mat = cMatrix::Identity(4);
	mat(0, 0) = c; mat(0, 1) = s;
	mat(1, 0) = -s; mat(1, 1) = c;
	return mat;
}

cMatrix cMatrix::Scaling(float x, float y, float z)
{
	cMatrix mat = cMatrix::Identity(4);
	mat(0, 0) = x;
	mat(1, 1) = y;
	mat(2, 2) = z;
	return mat;
}

cMatrix cMatrix::LookAtLH(const cVector3 & vEye, const cVector3 & vLookAt, const cVector3 & vUp)
{
	cMatrix mat = cMatrix::Identity(4);
	cVector3 unitZ = vLookAt.Normalize();
	cVector3 unitX = cVector3::Cross(vUp, vLookAt).Normalize();
	cVector3 unitY = cVector3::Cross(unitZ, unitX).Normalize();
	mat(0, 0) = unitX[0];
	mat(1, 0) = unitX[1];
	mat(2, 0) = unitX[2];
	mat(0, 1) = unitY[0];
	mat(1, 1) = unitY[1];
	mat(2, 1) = unitY[2];
	mat(0, 2) = unitZ[0];
	mat(1, 2) = unitZ[1];
	mat(2, 2) = unitZ[2];
	mat(3, 0) = -cVector3::Dot(unitX, vEye);
	mat(3, 1) = -cVector3::Dot(unitY, vEye);
	mat(3, 2) = -cVector3::Dot(unitZ, vEye);
	return mat;
}

cMatrix cMatrix::PerspectiveFovLH(float fFovY, float fAspect, float fNearZ, float fFarZ)
{
	cMatrix mat(4, 0.0f);
	float cotFov = 1 / tan(fFovY / 2);
	mat(0, 0) = cotFov / fAspect;
	mat(1, 1) = cotFov;
	mat(2, 2) = fFarZ / (fFarZ - fNearZ);
	mat(3, 2) = -fFarZ*fNearZ / (fFarZ - fNearZ);
	mat(2, 3) = 1;
	//float cotFov = 1 / tan(fFovY / 2);
	//mat(0, 0) = cotFov / fAspect;
	//mat(1, 1) = cotFov;
	//mat(2, 2) = fFarZ / (fFarZ - fNearZ);
	//mat(2, 3) = -fFarZ*fNearZ / (fFarZ - fNearZ);
	//mat(3, 2) = 1;
	return mat;
}

cMatrix cMatrix::Viewport(float fX, float fY, float fW, float fH, float fMinZ, float fMaxZ)
{
	cMatrix mat = cMatrix::Identity(4);
	mat(0, 0) = fW / 2;
	mat(1, 1) = -fH / 2;
	mat(2, 2) = fMaxZ - fMinZ;
	mat(3, 0) = fX + fW / 2;
	mat(3, 1) = fY + fH / 2;
	mat(3, 2) = fMinZ;
	return mat;
}

std::ostream & operator<<(std::ostream & os, const cMatrix & m)
{
	for (int iRow = 0; iRow < m.GetDimension(); ++iRow) {
		os << " ";
		for (int iCol = 0; iCol < m.GetDimension(); ++iCol) {
			os << std::fixed << std::setw(9) << std::setprecision(4) << m(iRow, iCol)<< " ";
		}
		os << std::endl;
	}
	return os;
}
