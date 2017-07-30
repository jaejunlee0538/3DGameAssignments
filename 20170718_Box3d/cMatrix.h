#pragma once
#include <vector>
#include <ostream>
#include "cVector4.h"
#include <tstring.h>
class cMatrix
{
	class cRow {
	public:
		cRow() { }
		cRow(int N) {
			init(N);
		}
		cRow(int N, float val) {
			init(N, val);
		}
		void init(int N) {
			_data.resize(N);
		}
		void init(int N, float val) {
			_data.resize(N, val);
		}
		void setValues(float val) {
			for (int i = 0; i < _data.size(); ++i) {
				_data[i] = val;
			}
		}
		void multScalar(float s) {
			for (int i = 0; i < _data.size(); ++i) {
				_data[i] *= s;
			}
		}
		float& operator[](size_t idx) {
			return _data[idx];
		}
		const float& operator[](size_t idx) const{
			return _data[idx];
		}
	private:
		std::vector<float> _data;
	};
public:
	cMatrix();
	cMatrix(int dim, float initVal);
	~cMatrix();
	
	//요소의 값이 초기화되지 않은 N X N 행렬로 초기화한다.
	void init(int N);
	//모든 요소의 값이 val인 N X N 행렬로 초기화한다.
	void init(int N, float val);
	//행렬의 차원을 반환한다.
	int GetDimension() const {
		return _rows.size();
	}
	//행렬이 초기화되지 않았으면 false를 반환한다.
	bool IsInitialized() const{
		return _rows.empty()==false;
	}
	//행렬의 모든 요소의 값을 val로 바꾼다.
	void ClearValue(float val);
	//iRow, iCol 요소를 반환한다.
	float& Get(int iRow, int iCol);
	//iRow, iCol 요소를 반환한다.
	const float& Get(int iRow, int iCol) const;
	//행렬의 모든 요소에 스칼라 s를 곱한다.
	void MultScalar(float s);
	//this의 전치행렬을 반환한다.
	cMatrix Transpose();
	//this의 역행렬을 반환한다.
	cMatrix Inverse();
	//this의 역행렬을 계산할 수 있으면 true를 반환한다.
	bool IsIvertible() const;
	//this의 행렬식을 반환한다.
	float GetDeterminant() const;
	//this의 [iRow,iCol] 여인수를 반환한다.
	float GetCofactor(int iRow, int iCol) const;
	//this의 iRowSkip 행과 iColSkip 열이 삭제된 부분행렬을 반환한다.
	cMatrix GetPartialMatrix(int iRowSkip, int iColSkip) const;
	//this 행렬을 콘솔에 출력한다.
	void Print() const;
	/***************************************************************/
	//연산자 오버로딩
	bool operator==(const cMatrix& m) const;
	bool operator!=(const cMatrix& m) const;
	cMatrix operator*(float scalar) const;
	cMatrix operator*(const cMatrix& m) const;
	cMatrix operator+(const cMatrix& m) const;
	cMatrix operator-(const cMatrix& m) const;
	float & operator()(int row, int col){
		return _rows[row][col];
	}
	const float & operator()(int row, int col) const{
		return _rows[row][col];
	}
	cRow& operator[](size_t idx) {
		return _rows[idx];
	}
	const cRow& operator[](size_t idx)const {
		return _rows[idx];
	}
	/***************************************************************/

	//크기 N X N의 단위 행렬을 반환한다.
	static cMatrix Identity(int N);

	static cMatrix Translation(float x, float y, float z);
	static cMatrix Translation(const cVector3& v);
	static cMatrix RotationX(float fAngle);
	static cMatrix RotationY(float fAngle);
	static cMatrix RotationZ(float fAngle);
	static cMatrix Scaling(float x, float y, float z);
	static cMatrix CameraPosition(const cVector3& vEye, const cVector3& vLookAt, const cVector3& vUp);
	static cMatrix LookAtLH(const cVector3& vEye, const cVector3& vLookAt, const cVector3& vUp);
	static cMatrix PerspectiveFovLH(float fFovY, float fAspect, float fNearZ, float fFarZ);
	static cMatrix Viewport(float fX, float fY, float fW, float fH, float fMinZ, float fMaxZ);
	static cMatrix InverseTransformationMatrix(const cMatrix& m_);
	static cMatrix NormalizeRotationPart(const cMatrix& m);
	friend std::ostream& operator<<(std::ostream& os, const cMatrix& m);
private:
	std::vector<cRow> _rows;
};

std::ostream& operator<<(std::ostream& os, const cMatrix& m);
void LogcMatrix(const cMatrix& m, const TCHAR* name);