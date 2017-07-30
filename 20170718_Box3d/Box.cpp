#include "Box.h"
#include "Camera3D.h"
#include "cMatrix.h"
#include "Math3D.h"
Box::Box()
{
}

Box::Box(float width, float height, float depth)
{
	init(width, height, depth);
}

Box::~Box()
{
}

void Box::init(float width, float height, float depth)
{
	float x = width / 2;
	float y = height / 2;
	float z = depth / 2;
	_vertices[0] = cVector3(-x, -y, -z);
	_vertices[1] = cVector3(-x, y, -z);
	_vertices[2] = cVector3(x, y, -z);
	_vertices[3] = cVector3(x, -y, -z);
	_vertices[4] = cVector3(-x, -y, z);
	_vertices[5] = cVector3(-x, y, z);
	_vertices[6] = cVector3(x, y, z);
	_vertices[7] = cVector3(x, -y, z);

	//전면
	_indices[0] = 0; _indices[1] = 1; _indices[2] = 2;
	_indices[3] = 0; _indices[4] = 2; _indices[5] = 3;

	//후면
	_indices[6] = 4; _indices[7] = 6; _indices[8] = 5;
	_indices[9] = 4; _indices[10] = 7; _indices[11] = 6;

	// 왼쪽 측변
	_indices[12] = 4; _indices[13] = 5; _indices[14]= 1;
	_indices[15] = 4; _indices[16] = 1; _indices[17] = 0;

	// 요른쪽 측연
	_indices[18] = 3; _indices[19] = 2; _indices[20] = 6;
	_indices[21] = 3; _indices[22] = 6; _indices[23] = 7;

	// 상단
	_indices[24] = 1; _indices[25] = 5; _indices[26] = 6;
	_indices[27] = 1; _indices[28] = 6; _indices[29] = 2;

	// 하단
	_indices[30] = 4; _indices[31] = 0; _indices[32] = 3;
	_indices[33] = 4; _indices[34] = 3; _indices[35] = 7;
}

void Box::Render(Camera3D & camera) const
{
	std::vector<cVector4> vertices(36);

	//LogcMatrix(GetWorldMatrix(), TEXT("World"));
	for (int i = 0; i < 36; ++i) {
		vertices[i] = cVector4(_vertices[_indices[i]], 1) * GetWorldMatrix();
	}
	camera.RenderTriangle(vertices.data(), 12);
}
