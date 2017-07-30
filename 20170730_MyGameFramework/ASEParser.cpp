#include "stdafx.h"
#include "ASEParser.h"
#include "StringUtils.h"
#include <fstream>
#include <sstream>
#include "Exceptions.h"
#include "MeshModel.h"
#include "MeshObject.h"
namespace SGA {
#define STR_BUF_SIZE	100
	ASEParser::ASEParser()
	{
	}

	ASEParser::~ASEParser()
	{
	}

	void ASEParser::load(const char * filePath, MeshObject** object)
	{
		FILE* file;
		errno_t err = fopen_s(&file, filePath, "r");
		if (err != 0) {
			throw ResourceFileMissing(filePath);
		}
		
		while (!feof(file)) {
			fscanf_s(file, "%s", &_buffer, STR_BUF_SIZE);
			if (!strcmp(_buffer, "*MATERIAL_LIST")) {
				nextLine(file);
				parseMtllib(file);
			}
			else if (!strcmp(_buffer, "*SCENE")) {
				nextLine(file);
				parseScene(file);
			}
			else if (!strcmp(_buffer, "*GEOMOBJECT")) {
				nextLine(file);
				MeshObject * object = new MeshObject();
				parseGeomObject(*object, file);
				insertObject(object->getTag(), object);
			}
			else {
				nextLine(file);
			}
		}
		fclose(file);
		
		*object = nullptr;
		for (auto it = _gameObjects.begin(); it != _gameObjects.end(); ++it) {
			if (it->second->hasParent() == false) {
				assert(*object == nullptr && "Root 오브젝트가 하나 이상 존재합니다.");
				*object = it->second;
			}
		}
		
	}

	bool ASEParser::isClosingBracket(const char * str)
	{
		return strchr(str, '}');
	}

	void ASEParser::parseScene(FILE * file)
	{
		while (!feof(file)) {
			fgets(_buffer, STR_BUF_SIZE, file);
			if (isClosingBracket((_buffer))) {
				break;
			}
		}
	}
	void ASEParser::parseMtllib(FILE * file)
	{
		int mtlCount;
		fscanf_s(file, "%*s %d", &mtlCount);
		_mtlLib.resize(mtlCount);

#ifdef _DEBUG
		LogDebugMessage("Material Count : %i\n", mtlCount);
#endif
		while (!feof(file)) {
			fscanf_s(file, "%s", &_buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MATERIAL")) {
				int mtlIdx;
				fgets(_buffer, BUFFER_SIZE, file);
				sscanf_s(_buffer, "%d", &mtlIdx);
#ifdef _DEBUG
				LogDebugMessage(TEXT("[ASEParser] %d Material 파싱\n"), mtlIdx);
#endif
				parseMaterial(_mtlLib[mtlIdx], file);
			}
			else {
				nextLine(file);
			}
		}
	}
	void ASEParser::parseMaterial(tagMaterial& material, FILE * file)
	{
		while (!feof(file)) {
			fscanf_s(file, "%s", &_buffer, STR_BUF_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MATERIAL_NAME")) {
				fgets(_buffer, BUFFER_SIZE, file);
				char mtlName[100];
				sscanf_s(_buffer, " \"%[^\"]\"", &mtlName, 100);
				material.mtlName = mtlName;
			}
			else if (!strcmp(_buffer, "*MATERIAL_AMBIENT")) {
				fgets(_buffer, BUFFER_SIZE, file);
				sscanf_s(_buffer, "%f %f %f", &(material.material->Ambient.r), &(material.material->Ambient.g), &(material.material->Ambient.b));
				material.material->Ambient.a = 1.0f;
			}
			else if (!strcmp(_buffer, "*MATERIAL_DIFFUSE")) {
				fgets(_buffer, BUFFER_SIZE, file);
				sscanf_s(_buffer, "%f %f %f", &(material.material->Diffuse.r), &(material.material->Diffuse.g), &(material.material->Diffuse.b));
				material.material->Diffuse.a = 1.0f;
			}
			else if (!strcmp(_buffer, "*MATERIAL_SPECULAR")) {
				fgets(_buffer, BUFFER_SIZE, file);
				sscanf_s(_buffer,"%f %f %f", &(material.material->Specular.r), &(material.material->Specular.g), &(material.material->Specular.b));
				material.material->Specular.a = 1.0f;
			}
			else if (!strcmp(_buffer, "*MAP_DIFFUSE")) {
				parseMapDiffuse(material, file);
			}
			else {
				nextLine(file);
			}
		}
#ifdef _DEBUG
		LogDebugMessage(TEXT("Diffuse : %.3f %.3f %.3f\n"), material.material->Diffuse.r, material.material->Diffuse.g, material.material->Diffuse.b);
		LogDebugMessage(TEXT("Specular : %.3f %.3f %.3f\n"), material.material->Specular.r, material.material->Specular.g, material.material->Specular.b);
		LogDebugMessage(TEXT("Ambient : %.3f %.3f %.3f\n"), material.material->Ambient.r, material.material->Ambient.g, material.material->Ambient.b);
#endif
	}

	void ASEParser::parseMapDiffuse(tagMaterial & material, FILE * file)
	{
		while (!feof(file)) {
			fscanf_s(file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*BITMAP")) {
				fgets(_buffer, BUFFER_SIZE, file);
				char textureName[100];
				sscanf_s(_buffer, " \"%[^\"]\"", &textureName, 100);
#ifdef _DEBUG
				LogDebugMessage(TEXT("텍스쳐 로딩 : %s\n"), textureName);
#endif // _DEBUG
				material.texture = GET_TEXTURE_MGR()->createTexture(textureName);
			}
			else {
				nextLine(file);
			}
		}
	}

	void ASEParser::parseGeomObject(MeshObject& object, FILE * file)
	{
		_faces.clear();
		_uv.clear();
		_vertices.clear();
		_materialIndex = -1;
		while (!feof(file)) {
			fscanf_s(file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*NODE_NAME")) {
				fgets(_buffer, BUFFER_SIZE, file);
				char nodeName[100];
				sscanf_s(_buffer, " \"%[^\"]\"", &nodeName, 100);
				object.setTag(nodeName);
#ifdef _DEBUG
				LogDebugMessage(TEXT("---------------NodeName : %s-------------\n"), object.getTag().c_str());
#endif // _DEBUG
			}
			else if (!strcmp(_buffer, "*NODE_PARENT")) {
				fgets(_buffer, BUFFER_SIZE, file);
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
#ifdef _DEBUG
				LogDebugMessage(TEXT("NODE_TM\n"));
#endif // _DEBUG
				nextLine(file);
				parseNodeTM(object, file);
			}
			else if (!strcmp(_buffer, "*MESH")) {
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH\n"));
#endif // _DEBUG
				parseMesh(file);
			}
			else if (!strcmp(_buffer, "*MATERIAL_REF")) {
#ifdef _DEBUG
				LogDebugMessage(TEXT("MATERIAL_REF\n"));
#endif // _DEBUG
				fgets(_buffer, BUFFER_SIZE, file);
				sscanf_s(_buffer, "%d", &_materialIndex);
			}
			else if(!strcmp(_buffer, "*TM_ANIMATION")){
#ifdef _DEBUG
				LogDebugMessage(TEXT("TM_ANIMATION\n"));
#endif // _DEBUG
				nextLine(file);
				parseTMAnimation(file);
			}
			else {
				nextLine(file);
			}
		}
		MeshGroup::Ptr meshGroup(new MeshGroup);
		if (_materialIndex >= 0) {
			meshGroup->material = _mtlLib[_materialIndex].material;
			meshGroup->texture = _mtlLib[_materialIndex].texture;
		}
		meshGroup->name = object.getTag();
		meshGroup->vertices.resize(_faces.size() * 3);
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

	void ASEParser::parseNodeTM(MeshObject & object, FILE * file)
	{
		while (!feof(file)) {
			fscanf_s(file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (true) {
				nextLine(file);
			}
		}
	}

	void ASEParser::parseVertexList(FILE * file)
	{
		while (!feof(file)) {
			fscanf_s(file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MESH_VERTEX")) {
				int idx;
				float x, y, z;
				fgets(_buffer, BUFFER_SIZE, file);
				sscanf_s(_buffer, "%d %f %f %f", &idx, &x, &y, &z);
				_vertices[idx].x = x;
				_vertices[idx].y = z;
				_vertices[idx].z = y;
			}
			else {
				nextLine(file);
			}
		}
	}

	void ASEParser::parseFaceList(FILE * file)
	{
		while (!feof(file)) {
			fscanf_s(file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MESH_FACE")) {
				int faceIdx;
				int vertexIdx[3];
				fgets(_buffer, BUFFER_SIZE, file);
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

	void ASEParser::parseTVertexList(FILE * file)
	{
		while (!feof(file)) {
			fscanf_s(file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MESH_TVERT")) {
				int idx;
				float x,y,z;
				fgets(_buffer, BUFFER_SIZE, file);
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

	void ASEParser::parseTFaceList(FILE * file)
	{
		while (!feof(file)) {
			fscanf_s(file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MESH_TFACE")) {
				int idx;
				int x, y, z;
				fgets(_buffer, BUFFER_SIZE, file);
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

	void ASEParser::parseNormals(FILE * file)
	{
		int faceIdx;
		int vertexCount = 0;
		while (!feof(file)) {
			fscanf_s(file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MESH_VERTEXNORMAL")) {
				int idx;
				float x, y, z;
				fgets(_buffer, BUFFER_SIZE, file);
				sscanf_s(_buffer, "%d %f %f %f", &idx, &x, &y, &z);
				_faces[faceIdx].normals[vertexCount].x = x;
				_faces[faceIdx].normals[vertexCount].y = y;
				_faces[faceIdx].normals[vertexCount].z = z;
				vertexCount++;
			}
			else if (!strcmp(_buffer, "*MESH_FACENORMAL")) {
				fgets(_buffer, BUFFER_SIZE, file);
				sscanf_s(_buffer, "%d %*f %*f %*f", &faceIdx);
				vertexCount = 0;
			}
			else {
				assert(false && "존재할 수 없는 Key값입니다.");
			}
		}
	}

	void ASEParser::parseMesh(FILE * file)
	{
		while (!feof(file)) {
			fscanf_s(file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*MESH_NUMVERTEX")) {
				int nVertices;
				fgets(_buffer, BUFFER_SIZE, file);
				sscanf_s(_buffer, "%d", &nVertices);
				_vertices.resize(nVertices);
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH_NUMVERTEX : %d\n"), nVertices);
#endif // _DEBUG
			}
			else if (!strcmp(_buffer, "*MESH_NUMFACES")) {
				int nFaces;
				fgets(_buffer, BUFFER_SIZE, file);
				sscanf_s(_buffer, "%d", &nFaces);
				_faces.resize(nFaces);
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH_NUMFACES : %d\n"), nFaces);
#endif // _DEBUG
			}
			else if (!strcmp(_buffer, "*MESH_NUMTVERTEX")) {
				int nTex;
				fgets(_buffer, BUFFER_SIZE, file);
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
				nextLine(file);
				parseVertexList(file);
			}
			else if (!strcmp(_buffer, "*MESH_FACE_LIST")) {
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH_FACE_LIST\n"));
#endif // _DEBUG
				nextLine(file);
				parseFaceList(file);
			}
			else if (!strcmp(_buffer, "*MESH_NORMALS")) {
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH_NORMALS\n"));
#endif // _DEBUG
				nextLine(file);
				parseNormals(file);
			}
			else if (!strcmp(_buffer, "*MESH_TVERTLIST")) {
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH_TVERTLIST\n"));
#endif // _DEBUG
				nextLine(file);
				parseTVertexList(file);
			}
			else if (!strcmp(_buffer, "*MESH_TFACELIST")) {
#ifdef _DEBUG
				LogDebugMessage(TEXT("MESH_TFACELIST\n"));
#endif // _DEBUG
				nextLine(file);
				parseTFaceList(file);
			}
			else {
				nextLine(file);
			}
		}
	}

	void ASEParser::parseTMAnimation(FILE * file)
	{
		while (!feof(file)) {
			fscanf_s(file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*NODE_NAME")) {
				nextLine(file);
			}
			else if (!strcmp(_buffer, "*CONTROL_POS_TRACK")) {
				nextLine(file);
				parsePosTrack(file);
			}
			else if (!strcmp(_buffer, "*CONTROL_ROT_TRACK")) {
				nextLine(file);
				parseRotTrack(file);
			}
			else if (!strcmp(_buffer, "*CONTROL_ROT_TCB")) {
				nextLine(file);
				parseCtrlRotTCB(file);
			}
			else {
				assert(false);
			}
		}
	}

	void ASEParser::parsePosTrack(FILE * file)
	{
		while (!feof(file)) {
			fscanf_s(file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*CONTROL_POS_SAMPLE")) {
				nextLine(file);
			}
			else {
				assert(false);
			}
		}
	}

	void ASEParser::parseRotTrack(FILE * file)
	{
		while (!feof(file)) {
			fscanf_s(file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*CONTROL_ROT_SAMPLE")) {
				nextLine(file);
			}
			else {
				assert(false);
			}
		}
	}

	void ASEParser::parseCtrlRotTCB(FILE * file)
	{
		while (!feof(file)) {
			fscanf_s(file, "%s", _buffer, BUFFER_SIZE);
			if (isClosingBracket(_buffer)) {
				break;
			}
			if (!strcmp(_buffer, "*CONTROL_TCB_ROT_KEY")) {
				nextLine(file);
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

	void ASEParser::nextLine(FILE * file)
	{
		char buffer[STR_BUF_SIZE];
		fgets(buffer, STR_BUF_SIZE, file);
	}
}