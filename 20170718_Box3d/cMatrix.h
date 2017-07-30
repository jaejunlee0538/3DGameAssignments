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
	
	//����� ���� �ʱ�ȭ���� ���� N X N ��ķ� �ʱ�ȭ�Ѵ�.
	void init(int N);
	//��� ����� ���� val�� N X N ��ķ� �ʱ�ȭ�Ѵ�.
	void init(int N, float val);
	//����� ������ ��ȯ�Ѵ�.
	int GetDimension() const {
		return _rows.size();
	}
	//����� �ʱ�ȭ���� �ʾ����� false�� ��ȯ�Ѵ�.
	bool IsInitialized() const{
		return _rows.empty()==false;
	}
	//����� ��� ����� ���� val�� �ٲ۴�.
	void ClearValue(float val);
	//iRow, iCol ��Ҹ� ��ȯ�Ѵ�.
	float& Get(int iRow, int iCol);
	//iRow, iCol ��Ҹ� ��ȯ�Ѵ�.
	const float& Get(int iRow, int iCol) const;
	//����� ��� ��ҿ� ��Į�� s�� ���Ѵ�.
	void MultScalar(float s);
	//this�� ��ġ����� ��ȯ�Ѵ�.
	cMatrix Transpose();
	//this�� ������� ��ȯ�Ѵ�.
	cMatrix Inverse();
	//this�� ������� ����� �� ������ true�� ��ȯ�Ѵ�.
	bool IsIvertible() const;
	//this�� ��Ľ��� ��ȯ�Ѵ�.
	float GetDeterminant() const;
	//this�� [iRow,iCol] ���μ��� ��ȯ�Ѵ�.
	float GetCofactor(int iRow, int iCol) const;
	//this�� iRowSkip ��� iColSkip ���� ������ �κ������ ��ȯ�Ѵ�.
	cMatrix GetPartialMatrix(int iRowSkip, int iColSkip) const;
	//this ����� �ֿܼ� ����Ѵ�.
	void Print() const;
	/***************************************************************/
	//������ �����ε�
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

	//ũ�� N X N�� ���� ����� ��ȯ�Ѵ�.
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