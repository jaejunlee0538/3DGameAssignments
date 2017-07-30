#pragma once
#include <vector>
class cMatrix
{
public:
	cMatrix();
	~cMatrix();
	
	//����� ���� �ʱ�ȭ���� ���� N X N ��ķ� �ʱ�ȭ�Ѵ�.
	void init(int N);
	//��� ����� ���� val�� N X N ��ķ� �ʱ�ȭ�Ѵ�.
	void init(int N, float val);
	//����� ������ ��ȯ�Ѵ�.
	int GetDimension() const {
		return _N;
	}
	//����� �ʱ�ȭ���� �ʾ����� false�� ��ȯ�Ѵ�.
	bool IsInitialized() const{
		return _data.empty()==false;
	}
	//����� ��� ����� ���� val�� �ٲ۴�.
	void ClearValue(float val);
	//iRow, iCol ��Ҹ� ��ȯ�Ѵ�.
	float& Get(int iRow, int iCol);
	//iRow, iCol ��Ҹ� ��ȯ�Ѵ�.
	const float& Get(int iRow, int iCol) const;
	//����� ��� ��ҿ� ��Į�� s�� ���Ѵ�.
	void MultScalar(float s);
	//������ �迭�� ���� ����� ��Ҹ� �����Ѵ�.
	void SetDataFrom(const float* data);
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
		return Get(row, col);
	}
	const float & operator()(int row, int col) const{
		return Get(row, col);
	}
	/***************************************************************/

	//ũ�� N X N�� ���� ����� ��ȯ�Ѵ�.
	static cMatrix Identity(int N);
private:
	void ClearMemory();
private:
	std::vector<float> _data;
	int _N;
};

