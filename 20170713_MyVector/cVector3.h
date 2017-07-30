#pragma once
class cVector3
{
public:
	cVector3();
	cVector3(float x, float y, float z);
	~cVector3();

	float Length() const;
	float LengthSq() const;
	cVector3 Normalize() const;
	void Print();

	float& X();
	const float& X() const;
	float& Y();
	const float& Y() const;
	float& Z();
	const float& Z() const;

	float& operator[](size_t idx);
	const float& operator[](size_t idx)const;
	bool operator==(const cVector3& vec) const;
	bool operator!=(const cVector3& vec) const;
	cVector3 operator+(const cVector3& vec) const;
	cVector3 operator-(const cVector3& vec) const;
	cVector3 operator*(float s) const;

	static float Dot(const cVector3& v1, const cVector3& v2);
	static cVector3 Cross(const cVector3& v1, const cVector3& v2);
private:
	float _xyz[3];
};

