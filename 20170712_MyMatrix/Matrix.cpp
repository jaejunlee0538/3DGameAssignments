#include "stdafx.h"
#include "Matrix.h"
#include <float.h>
#include <assert.h>
#include <cmath>
#define DETERMINANT_THRESHOLD	0.00001f
namespace {
	inline int IDX(int row, int col, int N) {
		return col + row * N;
	}
}

cMatrix::cMatrix()
	:_N(-1)
{
}

cMatrix::~cMatrix()
{
	ClearMemory();
}

void cMatrix::init(int N)
{
	ClearMemory();
	_N = N;
	_data.resize(N*N);
}

void cMatrix::init(int N, float val)
{
	ClearMemory();
	_N = N;
	_data.resize(N*N);
	ClearValue(val);
}

void cMatrix::ClearValue(float val)
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	for (int i = 0; i < _N*_N; ++i) {
		_data[i] = val;
	}
}

void cMatrix::MultScalar(float s)
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	for (int i = 0; i < _N*_N; ++i) {
		_data[i] *= s;
	}
}

void cMatrix::SetDataFrom(const float * data)
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	memcpy(_data.data(), data, sizeof(float)*_N*_N);
}

cMatrix cMatrix::Transpose()
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	cMatrix transposed;
	transposed.init(_N);
	//대각성분 상단에 for문을 돈다.
	for (int iRow = 0; iRow < _N; ++iRow) {
		for (int iCol = iRow; iCol < _N; ++iCol) {
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
	C.init(_N);
	for (int iRow = 0; iRow < _N; ++iRow) { 
		for (int iCol = 0; iCol < _N; ++iCol) {
			C(iRow, iCol) = GetCofactor(iRow, iCol);
		}
	}
	float det = 0.0;
	for (int i = 0; i < _N; ++i) {
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
	assert(IDX(iRow, iCol, _N) < _N*_N && "배열의 범위를 초과하였습니다");
	return _data[IDX(iRow, iCol, _N)];
}

const float & cMatrix::Get(int iRow, int iCol) const
{
	assert(IDX(iRow, iCol, _N) < _N*_N && "배열의 범위를 초과하였습니다");
	return _data[IDX(iRow, iCol, _N)];
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
		for (int iCol = 0; iCol < _N; ++iCol) {
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
	m.init(_N - 1);
	int iRowOut=0, iColOut=0;
	for (int iRow = 0; iRow < _N; ++iRow) {
		if (iRow == iRowSkip) {
			continue;
		}
		iColOut = 0;
		for (int iCol = 0; iCol < _N; ++iCol) {
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

void cMatrix::Print() const
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	for (int iRow = 0; iRow < _N; ++iRow) {
		std::cout << " ";
		for (int iCol = 0; iCol < _N; ++iCol) {
			std::cout <<std::fixed<<std::setw(7)<<std::setprecision(2)<< _data[IDX(iRow, iCol, _N)] << " ";
		}
		std::cout << std::endl;
	}
}

bool cMatrix::operator==(const cMatrix & m) const
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	assert(m.IsInitialized() && "두번째 행렬이 초기화되지 않았습니다.");
	if (GetDimension() != m.GetDimension())
		return false;
	int NEle = _N*_N;
	for (int i = 0; i < NEle; ++i) {
		if (std::abs(_data[i] - m._data[i]) > FLT_EPSILON)
			return false;
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
	cMatrix m;
	m.init(_N);
	m.SetDataFrom(_data.data());
	m.MultScalar(scalar);
	return m;
}

cMatrix cMatrix::operator*(const cMatrix & m) const
{
	assert(IsInitialized() && "행렬이 초기화 되지 않았습니다.");
	assert(m.IsInitialized() && "두번째 행렬이 초기화되지 않았습니다.");
	assert(GetDimension() == m.GetDimension() && "곱할 수 없는 행렬입니다.");
	cMatrix m3;
	m3.init(_N);
	const cMatrix& m1 = *this;
	const cMatrix& m2 = m;

	for (int iRow = 0; iRow < _N; ++iRow) {
		for (int iCol = 0; iCol < _N; ++iCol) {
			float v = 0.0f;
			for (int i = 0; i < _N; ++i) {
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
	m3.init(_N);
	const cMatrix& m1 = *this;
	const cMatrix& m2 = m;
	for (int iRow = 0; iRow < _N; ++iRow) {
		for (int iCol = 0; iCol < _N; ++iCol) {
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
	m3.init(_N);
	const cMatrix& m1 = *this;
	const cMatrix& m2 = m;
	for (int iRow = 0; iRow < _N; ++iRow) {
		for (int iCol = 0; iCol < _N; ++iCol) {
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

void cMatrix::ClearMemory()
{
	_data.clear();
}
