#include "Math3D.h"

cVector4 operator*(const cVector4 & vec4, const cMatrix & mat4X4)
{
	cVector4 result;
	result[0] = vec4[0] * mat4X4(0, 0) + vec4[1] * mat4X4(1, 0) + vec4[2] * mat4X4(2, 0) + vec4[3] * mat4X4(3, 0);
	result[1] = vec4[0] * mat4X4(0, 1) + vec4[1] * mat4X4(1, 1) + vec4[2] * mat4X4(2, 1) + vec4[3] * mat4X4(3, 1);
	result[2] = vec4[0] * mat4X4(0, 2) + vec4[1] * mat4X4(1, 2) + vec4[2] * mat4X4(2, 2) + vec4[3] * mat4X4(3, 2);
	result[3] = vec4[0] * mat4X4(0, 3) + vec4[1] * mat4X4(1, 3) + vec4[2] * mat4X4(2, 3) + vec4[3] * mat4X4(3, 3);
	return result;
}
