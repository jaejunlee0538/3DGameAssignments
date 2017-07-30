#pragma once
#include <vector>
class cMatrix
{
public:
	cMatrix();
	~cMatrix();
	
	//요소의 값이 초기화되지 않은 N X N 행렬로 초기화한다.
	void init(int N);
	//모든 요소의 값이 val인 N X N 행렬로 초기화한다.
	void init(int N, float val);
	//행렬의 차원을 반환한다.
	int GetDimension() const {
		return _N;
	}
	//행렬이 초기화되지 않았으면 false를 반환한다.
	bool IsInitialized() const{
		return _data.empty()==false;
	}
	//행렬의 모든 요소의 값을 val로 바꾼다.
	void ClearValue(float val);
	//iRow, iCol 요소를 반환한다.
	float& Get(int iRow, int iCol);
	//iRow, iCol 요소를 반환한다.
	const float& Get(int iRow, int iCol) const;
	//행렬의 모든 요소에 스칼라 s를 곱한다.
	void MultScalar(float s);
	//데이터 배열로 부터 행렬의 요소를 설정한다.
	void SetDataFrom(const float* data);
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
		return Get(row, col);
	}
	const float & operator()(int row, int col) const{
		return Get(row, col);
	}
	/***************************************************************/

	//크기 N X N의 단위 행렬을 반환한다.
	static cMatrix Identity(int N);
private:
	void ClearMemory();
private:
	std::vector<float> _data;
	int _N;
};

