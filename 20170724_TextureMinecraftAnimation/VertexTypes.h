#pragma once

struct VertexPosDiff {
	enum {
		FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE
	};
	D3DXVECTOR3 p;
	D3DCOLOR color;
	VertexPosDiff() {

	}

	//VertexPosDiff(float x, float y, float z, BYTE r, BYTE g, BYTE b) {
	//	p.x = x;
	//	p.y = y;
	//	p.z = z;
	//	color.r = r;
	//	color.g = g;
	//	color.b = b;
	//	color.a = 0xff;
	//}

	VertexPosDiff(float x, float y, float z, D3DCOLOR rgb) {
		p.x = x;
		p.y = y;
		p.z = z;
		color = rgb;
	}
};

struct VertexPosTex {
	enum {
		FVF = D3DFVF_XYZ | D3DFVF_TEX1
	};
	D3DXVECTOR3 p;
	D3DXVECTOR2 uv;
	VertexPosTex() {}
	VertexPosTex(float x, float y, float z, float u, float v)
		:p(x, y, z), uv(u, v) {

	}
};

struct VertexPosNorm {
	enum {
		FVF = D3DFVF_XYZ | D3DFVF_NORMAL
	};

	D3DXVECTOR3 p;
	D3DXVECTOR3 n;

	VertexPosNorm(){}
	VertexPosNorm(const D3DXVECTOR3& pos, const D3DXVECTOR3& normal)
	:p(pos), n(normal){

	}
};
struct VertexPosNormTex {
	enum {
		FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
	};

	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2 uv;

	VertexPosNormTex(){}
	VertexPosNormTex(const D3DXVECTOR3& pos, const D3DXVECTOR3& normal, const D3DXVECTOR2& uv)
	:p(pos), n(normal), uv(uv){

	}
};

template <typename VecT>
D3DXVECTOR3 computeNormalFromVectors(const VecT& p1, const VecT& p2, const VecT& p3) {
	D3DXVECTOR3 normal;
	D3DXVECTOR3 v1(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z), v2(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);
	D3DXVec3Cross(&normal, &v1, &v2);
	D3DXVec3Normalize(&normal, &normal);
	return normal;
}
//
//template <typename VecT>
//D3DXVECTOR3 computeVertexNormal(const VecT& vTarget, const VecT* vAdj, size_t nAdj) {
//	std::vector<D3DXVECTOR3> surfNormals(nAdj);
//	surfNormals[0] = computeNormalFromVectors(vTarget, vAdj[0], vAdj[1]);
//}