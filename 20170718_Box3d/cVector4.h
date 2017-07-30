#pragma once
class cVector3;
class cVector4
{
public:
	cVector4();
	cVector4(float x, float y, float z, float w);
	cVector4(const cVector3& vec, float w);
	~cVector4();

	float Length() const;
	float LengthSq() const;
	cVector4 Normalize() const;
	void DivideByW();

	float& x() { return _xyzw[0]; }
	const float& x() const { return _xyzw[0]; }
	float& y() { return _xyzw[1]; }
	const float& y() const { return _xyzw[1]; }
	float& z() { return _xyzw[2]; }
	const float& z() const { return _xyzw[2]; }
	float& w() { return _xyzw[3]; }
	const float& w() const { return _xyzw[3]; }

	float& operator[](size_t idx);
	const float& operator[](size_t idx)const;
	bool operator==(const cVector4& vec) const;
	bool operator!=(const cVector4& vec) const;
	cVector4 operator+(const cVector4& vec) const;
	cVector4 operator-(const cVector4& vec) const;
	cVector4 operator*(float s) const;

	static float Dot(const cVector4& v1, const cVector4& v2);
	static cVector4 Cross(const cVector4& v1, const cVector4& v2);
private:
	float _xyzw[4];
};
