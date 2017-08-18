#pragma once
#include "cMtlTex.h"
struct Bone : public D3DXFRAME {
	D3DXMATRIX combinedTransformationMatrix;

	void calculateCombinedTransformationMatrix(Bone* parent);
};

struct BoneMesh :public D3DXMESHCONTAINER {
	std::vector<D3DMATERIAL9> materials;
	std::vector<LPDIRECT3DTEXTURE9> textures;

	ID3DXMesh*	originalMesh;
	DWORD numAttrGroups;
	D3DXATTRIBUTERANGE* attrTable;
	std::vector<D3DXMATRIX*> boneMatrixPtrs;
	std::vector<D3DXMATRIX> boneOffsetMatrices;
	std::vector<D3DXMATRIX> currentBoneMatrices;
};
class cFrameHierarchyAllocator : public ID3DXAllocateHierarchy
{
public:
	cFrameHierarchyAllocator();
	~cFrameHierarchyAllocator();

	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,
		LPD3DXFRAME *ppNewFrame) override;

	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer) override;

	
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;

	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;
};

