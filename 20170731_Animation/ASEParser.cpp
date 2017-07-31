#include "stdafx.h"
#include "ASEParser.h"
#include "StringUtils.h"
#include <fstream>
#include <sstream>
#include "Exceptions.h"
#include "MeshModel.h"
#include "MeshObject.h"
#include "D3DSerialize.h"
namespace SGA {
#define STR_BUF_SIZE	100
	ASEParser::ASEParser()
	{
		_file = nullptr;
	}

	ASEParser::~ASEParser()
	{
	}

	void ASEParser::load(const char * filePath, MeshObject** object, AnimationClip** animationClip)
	{
		errno_t err = fopen_s(&_file, filePath, "r");
		if (err != 0) {
			throw ResourceFileMissing(filePath);
		}
		AnimationClip* animClip = new AnimationClip();
		while (!feof(_file)) {
			fscanf_s(_file, "%s", &_buffer, STR_BUF_SIZE);
			if (!strcmp(_buffer, "*MATERIAL_LIST")) {
				nextLine();
				parseMtllib();
			}
			else if (!strcmp(_buffer, "*SCENE")) {
				nextLine();
				parseScene();
			}
			else if (!strcmp(_buffer, "*GEOMOBJECT")) {
				nextLine();
				MeshObject * object = new MeshObject();
				BoneAnimation boneAnimation;
				parseGeomObject(*object, boneAnimation);
				animClip->boneAnimations.push_back(boneAnimation);
				insertObject(object->getTag(), object);
			}
			else {
				nextLine();
			}
		}
		fclose(_file);
		
		*object = nullptr;
		for (auto it = _gameObjects.begin(); it != _gameObjects.end(); ++it) {
			if (it->second->hasParent() == false) {
				assert(*object == nullptr && "Root 오브젝트가 하나 이상 존재합니다.");
				*object = it->second;
			}
		}
		*animationClip = animClip;
	}

	bool ASEParser::isClosingBracket(const char * str)
	{
		return strchr(str, '}');
	}

	void ASEParser::parseScene()
	{
		while (!feof(_file)) {
			fgets(_buffer, STR_BUF_SIZE, _file);
			if (isClosingBracket((_buffer))) {
				break;
			}
		}
	}
	void ASEParser::parseMtllib()
	{
		int mtlCount;
		fscanf_s(_file, "%*s %d", &mtlCount);
		_mtlLib.resize(mtlCount);

#ifdef _DEBUG
		LogDebugMessage("Material Count : %i\n", mtlCount);
#endif
		while (!feof(_file)) {
			fscanf_s(_file, "%s", &_buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MATERIAL")) {
				int mtlIdx;
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%d", &mtlIdx);
#ifdef _DEBUG
				LogDebugMessage(TEXT("[ASEParser] %d Material 파싱\n"), mtlIdx);
#endif
				parseMaterial(_mtlLib[mtlIdx]);
			}
			else {
				nextLine();
			}
		}
	}
	void ASEParser::parseMaterial(tagMaterial& material)
	{
		while (!feof(_file)) {
			fscanf_s(_file, "%s", &_buffer, STR_BUF_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MATERIAL_NAME")) {
				fgets(_buffer, BUFFER_SIZE, _file);
				char mtlName[100];
				sscanf_s(_buffer, " \"%[^\"]\"", &mtlName, 100);
				material.mtlName = mtlName;
			}
			else if (!strcmp(_buffer, "*MATERIAL_AMBIENT")) {
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%f %f %f", &(material.material->Ambient.r), &(material.material->Ambient.g), &(material.material->Ambient.b));
				material.material->Ambient.a = 1.0f;
			}
			else if (!strcmp(_buffer, "*MATERIAL_DIFFUSE")) {
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%f %f %f", &(material.material->Diffuse.r), &(material.material->Diffuse.g), &(material.material->Diffuse.b));
				material.material->Diffuse.a = 1.0f;
			}
			else if (!strcmp(_buffer, "*MATERIAL_SPECULAR")) {
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer,"%f %f %f", &(material.material->Specular.r), &(material.material->Specular.g), &(material.material->Specular.b));
				material.material->Specular.a = 1.0f;
			}
			else if (!strcmp(_buffer, "*MAP_DIFFUSE")) {
				parseMapDiffuse(material);
			}
			else {
				nextLine();
			}
		}
#ifdef _DEBUG
		LogDebugMessage(TEXT("Diffuse : %.3f %.3f %.3f\n"), material.material->Diffuse.r, material.material->Diffuse.g, material.material->Diffuse.b);
		LogDebugMessage(TEXT("Specular : %.3f %.3f %.3f\n"), material.material->Specular.r, material.material->Specular.g, material.material->Specular.b);
		LogDebugMessage(TEXT("Ambient : %.3f %.3f %.3f\n"), material.material->Ambient.r, material.material->Ambient.g, material.material->Ambient.b);
#endif
	}

	void ASEParser::parseMapDiffuse(tagMaterial & material)
	{
		while (!feof(_file)) {
			fscanf_s(_file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*BITMAP")) {
				fgets(_buffer, BUFFER_SIZE, _file);
				char textureName[100];
				sscanf_s(_buffer, " \"%[^\"]\"", &textureName, 100);
#ifdef _DEBUG
				LogDebugMessage(TEXT("텍스쳐 로딩 : %s\n"), textureName);
#endif // _DEBUG
				material.texture = GET_TEXTURE_MGR()->createTexture(textureName);
			}
			else {
				nextLine();
			}
		}
	}

	void ASEParser::parseGeomObject(MeshObject& object, BoneAnimation& animation)
	{
		_faces.clear();
		_uv.clear();
		_vertices.clear();
		_materialIndex = -1;
		while (!feof(_file)) {
			fscanf_s(_file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*NODE_NAME")) {
				fgets(_buffer, BUFFER_SIZE, _file);
				char nodeName[100];
				sscanf_s(_buffer, " \"%[^\"]\"", &nodeName, 100);
				object.setTag(nodeName);
#ifdef _DEBUG
				LogDebugMessage(TEXT("---------------NodeName : %s-------------\n"), object.getTag().c_str());
#endif // _DEBUG
			}
			else if (!strcmp(_buffer, "*NODE_PARENT")) {
				fgets(_buffer, BUFFER_SIZE, _file);
				char parentName[100];
				sscanf_s(_buffer, " \"%[^\"]\"", &parentName, 100);
				auto it = _gameObjects.find(parentName);
				assert(it != _gameObjects.end() && "부모 노드가 존재하지 않습니다.");
				//object.setParent(it->second);
				it->second->insertChild(&object);
#ifdef _DEBUG
				LogDebugMessage(TEXT("Parent : %s\n"), it->second->getTag().c_str());
#endif // _DEBUG
			}
			else if (!strcmp(_buffer, "*NODE_TM")) {
				nextLine();
				D3DXMATRIX worldTM;
				parseNodeTM(worldTM);
#ifdef _DEBUG
				LogDebugMessage(TEXT("NODE_TM\n"));
				std::ostringstream oss;
				oss << "Matrix = " << std::endl << worldTM;
				LogDebugMessage(TEXT("%s\n"), oss.str().c_str());
#endif // _DEBUG
				_objectTransforms[object.getTag()] = worldTM;

				if (object.hasParent()) {
					//로컬 TM으로 변환
					D3DXMATRIX parentTM = _objectTransforms[object.getParent()->getTag()];
					D3DXMatrixInverse(&parentTM, NULL, &parentTM);
					worldTM = worldTM * parentTM;
				}
				object.setTransform(&worldTM);
			}
			else if (!strcmp(_buffer, "*MESH")) {
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH\n"));
#endif // _DEBUG
				parseMesh();
			}
			else if (!strcmp(_buffer, "*MATERIAL_REF")) {
#ifdef _DEBUG
				LogDebugMessage(TEXT("MATERIAL_REF\n"));
#endif // _DEBUG
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%d", &_materialIndex);
			}
			else if(!strcmp(_buffer, "*TM_ANIMATION")){
#ifdef _DEBUG
				LogDebugMessage(TEXT("TM_ANIMATION\n"));
#endif // _DEBUG
				nextLine();
				parseTMAnimation( animation, object );
			}
			else {
				nextLine();
			}
		}

		MeshGroup::Ptr meshGroup(new MeshGroup);
		if (_materialIndex >= 0) {
			meshGroup->material = _mtlLib[_materialIndex].material;
			meshGroup->texture = _mtlLib[_materialIndex].texture;
		}
		meshGroup->name = object.getTag();
		meshGroup->vertices.resize(_faces.size() * 3);

		//오브젝트 월드 행렬의 역행렬을 계산한다.
		D3DXMATRIX matWorldInv = _objectTransforms[object.getTag()];
		D3DXMatrixInverse(&matWorldInv, nullptr, &matWorldInv);
		//버텍스의 좌표를 로컬좌표계로 변환한다.
		for (size_t i = 0; i < _vertices.size(); ++i) {
			D3DXVec3TransformCoord(&_vertices[i], &_vertices[i], &matWorldInv);
		}
		//법선 벡터를 로컬좌표계로 변환한다.
		for (size_t iFace = 0; iFace < _faces.size(); ++iFace) {
			D3DXVec3TransformNormal(&_faces[iFace].normals[0], &_faces[iFace].normals[0], &matWorldInv);
			D3DXVec3TransformNormal(&_faces[iFace].normals[1], &_faces[iFace].normals[1], &matWorldInv);
			D3DXVec3TransformNormal(&_faces[iFace].normals[2], &_faces[iFace].normals[2], &matWorldInv);
		}

		//오브젝트의 버텍스를 완성한다.
		for (size_t iFace = 0; iFace < _faces.size(); ++iFace) {
			meshGroup->vertices[iFace * 3 + 0].n = _faces[iFace].normals[0];
			meshGroup->vertices[iFace * 3 + 2].n = _faces[iFace].normals[1];
			meshGroup->vertices[iFace * 3 + 1].n = _faces[iFace].normals[2];

			meshGroup->vertices[iFace * 3 + 0].p = _vertices[_faces[iFace].vertexIndices[0]];
			meshGroup->vertices[iFace * 3 + 2].p = _vertices[_faces[iFace].vertexIndices[1]];
			meshGroup->vertices[iFace * 3 + 1].p = _vertices[_faces[iFace].vertexIndices[2]];

			if (_materialIndex >= 0) {
				meshGroup->vertices[iFace * 3 + 0].uv = D3DXVECTOR2(_uv[_faces[iFace].tVertexIndices[0]].x, 1.0f - _uv[_faces[iFace].tVertexIndices[0]].y);
				meshGroup->vertices[iFace * 3 + 2].uv = D3DXVECTOR2(_uv[_faces[iFace].tVertexIndices[1]].x, 1.0f - _uv[_faces[iFace].tVertexIndices[1]].y);
				meshGroup->vertices[iFace * 3 + 1].uv = D3DXVECTOR2(_uv[_faces[iFace].tVertexIndices[2]].x, 1.0f - _uv[_faces[iFace].tVertexIndices[2]].y);
			}
		}

		MeshModel * mesh = new MeshModel();
		mesh->insertGroup(meshGroup->name, meshGroup);
		object.init(mesh);
	}

	void ASEParser::parseNodeTM(D3DXMATRIX& mat)
	{
		D3DXMATRIX matTMP;
		D3DXMatrixIdentity(&matTMP);
		while (!feof(_file)) {
			fscanf_s(_file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*TM_ROW0")) {
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%f %f %f", &matTMP(0, 0), &matTMP(0, 1), &matTMP(0, 2));
			}
			else if (!strcmp(_buffer, "*TM_ROW1")) {
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%f %f %f", &matTMP(1, 0), &matTMP(1, 1), &matTMP(1, 2));
			}
			else if (!strcmp(_buffer, "*TM_ROW2")) {
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%f %f %f", &matTMP(2, 0), &matTMP(2, 1), &matTMP(2, 2));
			}
			else if (!strcmp(_buffer, "*TM_ROW3")) {
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%f %f %f", &matTMP(3, 0), &matTMP(3, 1), &matTMP(3, 2));
			}
			else {
				nextLine();
			}
		}
		int permute[] = { 0,2,1,3 };
		for (int iRow = 0; iRow < 4; ++iRow) {
			for (int iCol = 0; iCol < 4; ++iCol) {
				mat(iRow, iCol) = matTMP(permute[iRow], permute[iCol]);
			}
		}
	}

	void ASEParser::parseVertexList()
	{
		while (!feof(_file)) {
			fscanf_s(_file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MESH_VERTEX")) {
				int idx;
				float x, y, z;
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%d %f %f %f", &idx, &x, &y, &z);
				_vertices[idx].x = x;
				_vertices[idx].y = z;
				_vertices[idx].z = y;
			}
			else {
				nextLine();
			}
		}
	}

	void ASEParser::parseFaceList()
	{
		while (!feof(_file)) {
			fscanf_s(_file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MESH_FACE")) {
				int faceIdx;
				int vertexIdx[3];
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%d: A: %d B: %d C: %d AB: %*d BC: %*d CA: %*d *MESH_SMOOTHING %*d *MESH_MTLID %*d", &faceIdx, &vertexIdx[0], &vertexIdx[1], &vertexIdx[2]);
				_faces[faceIdx].vertexIndices[0] = vertexIdx[0];
				_faces[faceIdx].vertexIndices[1] = vertexIdx[1];
				_faces[faceIdx].vertexIndices[2] = vertexIdx[2];
			}
			else {
				assert(false && "MESH_FACE외의 Key값이 나올리 없습니다.");
			}
		}
	}

	void ASEParser::parseTVertexList()
	{
		while (!feof(_file)) {
			fscanf_s(_file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MESH_TVERT")) {
				int idx;
				float x,y,z;
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%d %f %f %f", &idx, &x, &y, &z);
				_uv[idx].x = x;
				_uv[idx].y = y;
				_uv[idx].z = z;
			}
			else {
				assert(false && "MESH_TVERT외의 Key값이 나올리 없습니다.");
			}
		}
	}

	void ASEParser::parseTFaceList()
	{
		while (!feof(_file)) {
			fscanf_s(_file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MESH_TFACE")) {
				int idx;
				int x, y, z;
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%d %d %d %d", &idx, &x, &y, &z);
				_faces[idx].tVertexIndices[0] = x;
				_faces[idx].tVertexIndices[1] = y;
				_faces[idx].tVertexIndices[2] = z;
			}
			else {
				assert(false && "MESH_TFACE외의 Key값이 나올리 없습니다.");
			}
		}
	}

	void ASEParser::parseNormals()
	{
		int faceIdx;
		int vertexCount = 0;
		while (!feof(_file)) {
			fscanf_s(_file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MESH_VERTEXNORMAL")) {
				int idx;
				float x, y, z;
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%d %f %f %f", &idx, &x, &y, &z);
				_faces[faceIdx].normals[vertexCount].x = x;
				_faces[faceIdx].normals[vertexCount].y = y;
				_faces[faceIdx].normals[vertexCount].z = z;
				vertexCount++;
			}
			else if (!strcmp(_buffer, "*MESH_FACENORMAL")) {
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%d %*f %*f %*f", &faceIdx);
				vertexCount = 0;
			}
			else {
				assert(false && "존재할 수 없는 Key값입니다.");
			}
		}
	}

	void ASEParser::parseMesh()
	{
		while (!feof(_file)) {
			fscanf_s(_file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MESH_NUMVERTEX")) {
				int nVertices;
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%d", &nVertices);
				_vertices.resize(nVertices);
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH_NUMVERTEX : %d\n"), nVertices);
#endif // _DEBUG
			}
			else if (!strcmp(_buffer, "*MESH_NUMFACES")) {
				int nFaces;
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%d", &nFaces);
				_faces.resize(nFaces);
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH_NUMFACES : %d\n"), nFaces);
#endif // _DEBUG
			}
			else if (!strcmp(_buffer, "*MESH_NUMTVERTEX")) {
				int nTex;
				fgets(_buffer, BUFFER_SIZE, _file);
				sscanf_s(_buffer, "%d", &nTex);
				_uv.resize(nTex);
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH_NUMTVERTEX : %d\n"), nTex);
#endif // _DEBUG
			}
			else if (!strcmp(_buffer, "*MESH_VERTEX_LIST")) {
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH_VERTEX_LIST\n"));
#endif // _DEBUG
				nextLine();
				parseVertexList();
			}
			else if (!strcmp(_buffer, "*MESH_FACE_LIST")) {
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH_FACE_LIST\n"));
#endif // _DEBUG
				nextLine();
				parseFaceList();
			}
			else if (!strcmp(_buffer, "*MESH_NORMALS")) {
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH_NORMALS\n"));
#endif // _DEBUG
				nextLine();
				parseNormals();
			}
			else if (!strcmp(_buffer, "*MESH_TVERTLIST")) {
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH_TVERTLIST\n"));
#endif // _DEBUG
				nextLine();
				parseTVertexList();
			}
			else if (!strcmp(_buffer, "*MESH_TFACELIST")) {
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH_TFACELIST\n"));
#endif // _DEBUG
				nextLine();
				parseTFaceList();
			}
			else {
				nextLine();
			}
		}
	}

	void ASEParser::parseTMAnimation(BoneAnimation& animation, MeshObject& object)
	{
		while (!feof(_file)) {
			fscanf_s(_file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*NODE_NAME")) {
				fgets(_buffer, BUFFER_SIZE, _file);
				char nodeName[100];
				sscanf_s(_buffer, " \"%[^\"]\"", &nodeName, 100);
				animation.setBoneName(nodeName);
			}
			else if (!strcmp(_buffer, "*CONTROL_POS_TRACK")) {
				nextLine();
				parsePosTrack(animation, object);
			}
			else if (!strcmp(_buffer, "*CONTROL_ROT_TRACK")) {
				nextLine();
				parseRotTrack(animation, object);
			}
			else if (!strcmp(_buffer, "*CONTROL_ROT_TCB")) {
				nextLine();
				parseCtrlRotTCB(animation, object);
			}
			else {
				assert(false);
			}
		}
	}

	void ASEParser::parsePosTrack(BoneAnimation& animation, MeshObject& object)
	{
		D3DXMATRIX worldInv = Identity4X4;
		if (object.hasParent()) {
			getLocalTransform(object.getTag(), object.getParent()->getTag(), worldInv);
		}
	
		while (!feof(_file)) {
			fscanf_s(_file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*CONTROL_POS_SAMPLE")) {
				fgets(_buffer, BUFFER_SIZE, _file);
				int time;
				D3DXVECTOR3 pos;
				sscanf_s(_buffer, "%d %f %f %f", &time, &pos.x, &pos.z, &pos.y);
				D3DXVec3TransformCoord(&pos, &pos, &worldInv);
				animation.insertTranslation(time, pos);
			}
			else {
				assert(false);
			}
		}
	}

	void ASEParser::parseRotTrack(BoneAnimation& animation, MeshObject& object)
	{
		D3DXMATRIX worldInv = Identity4X4;
		if (object.hasParent()) {
			getLocalTransform(object.getTag() , object.getParent()->getTag(), worldInv);
		}

		D3DXQUATERNION quatInvWorld;
		D3DXQuaternionRotationMatrix(&quatInvWorld, &worldInv);
		D3DXQUATERNION quat;
		bool firstRotation = true;
		while (!feof(_file)) {
			fscanf_s(_file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*CONTROL_ROT_SAMPLE")) {
				fgets(_buffer, BUFFER_SIZE, _file);
				int time;
				D3DXVECTOR3 axis;
				float angle;
				sscanf_s(_buffer, "%d %f %f %f %f", &time, &axis.x, &axis.z, &axis.y, &angle);
				if (firstRotation) {
					D3DXQuaternionRotationAxis(&quat, &axis, angle);
					firstRotation = false;
					animation.insertRotationQuaternion(time, quat*quatInvWorld);
				}
				else {
					D3DXQUATERNION quatIncrement;
					D3DXQuaternionRotationAxis(&quatIncrement, &axis, angle);
					quat = quatIncrement * quat;
					animation.insertRotationQuaternion(time, quat*quatInvWorld);
				}
			}
			else {
				assert(false);
			}
		}
	}

	void ASEParser::parseCtrlRotTCB(BoneAnimation& animation, MeshObject& object)
	{
		while (!feof(_file)) {
			fscanf_s(_file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*CONTROL_TCB_ROT_KEY")) {
				nextLine();
			}
			else {
				assert(false);
			}
		}
	}

	void ASEParser::insertObject(std::string objectName, MeshObject * gameObject)
	{
		assert(objectName.empty() == false);
		auto it = _gameObjects.find(objectName);
		assert(it == _gameObjects.end() && "이미 존재하는 오브젝트입니다");
		_gameObjects.insert(std::make_pair(objectName, gameObject));
	}

	void ASEParser::nextLine()
	{
		char buffer[STR_BUF_SIZE];
		fgets(buffer, STR_BUF_SIZE, _file);
	}
	void ASEParser::getLocalTransform(std::string objFrom, std::string objTo, D3DXMATRIX & transform)
	{
		D3DXMATRIX tmp = _objectTransforms[objFrom];
		D3DXMatrixInverse(&tmp, NULL, &tmp);
		transform = _objectTransforms[objTo] * tmp;
	}
}