#include "FBXLibrary_Internal.h"
#include "FBXLibrary/FBXParserData.h"
#include <string>

FBXLibrary_Internal::FBXLibrary_Internal()
	: m_pFbxManager(nullptr), m_pFbxImporter(nullptr), m_pFbxScene(nullptr)
{
}

FBXLibrary_Internal::~FBXLibrary_Internal()
{
}

bool FBXLibrary_Internal::Init()
{
	///TODO:: 예제 코드에 Commom.h InitializeSdkObjects() 함수를 참고해서 다듬자. 

	// FBX SDK 준비 
	m_pFbxManager = FbxManager::Create();

	///TODO:: FbxIOSettings 클래스가 정확히 하는 일이 뭔지 공부하기.  
	FbxIOSettings* _pFbxIOSettings = FbxIOSettings::Create(m_pFbxManager, IOSROOT);
	m_pFbxManager->SetIOSettings(_pFbxIOSettings);

	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
	m_pFbxScene = FbxScene::Create(m_pFbxManager, ""); // 씬에 모든 데이터가 담겨 있음. 

	return  true;
}

bool FBXLibrary_Internal::Release()
{
	if (m_pObjMeshList.size() != 0)
	{
		for (auto _mesh : m_pObjMeshList)
		{
			//_mesh->m_pFBXMyNode->Destroy();
			//
			//if (_mesh->m_pFBXParentNode != nullptr)
			//{
			//	_mesh->m_pFBXParentNode->Destroy();
			//}

			if (_mesh->m_pMeshInfo != nullptr)
			{
				size_t _faceCount = _mesh->m_pMeshInfo->optimizeFace.size();
				for (int i = 0; i < _faceCount; ++i)
				{
					for (int j = 0; j < 3; j++)
					{
						_mesh->m_pMeshInfo->optimizeFace.at(i).index[j] = 0; 
					}
				}
				_mesh->m_pMeshInfo->optimizeFace.clear();
				_mesh->m_pMeshInfo->optimizeFace.shrink_to_fit(); // capacity를 0으로 만들어줌. 

				size_t _vertexCount = _mesh->m_pMeshInfo->optimizeVertex.size();
				for (int i = 0; i < _vertexCount; ++i)
				{
					_mesh->m_pMeshInfo->optimizeVertex.at(i).pos[0] = 0;
					_mesh->m_pMeshInfo->optimizeVertex.at(i).pos[1] = 0;
					_mesh->m_pMeshInfo->optimizeVertex.at(i).pos[2] = 0;
															
					_mesh->m_pMeshInfo->optimizeVertex.at(i).normal[0] = 0;
					_mesh->m_pMeshInfo->optimizeVertex.at(i).normal[1] = 0;
					_mesh->m_pMeshInfo->optimizeVertex.at(i).normal[2] = 0;
															
					_mesh->m_pMeshInfo->optimizeVertex.at(i).uv[0] = 0;
					_mesh->m_pMeshInfo->optimizeVertex.at(i).uv[1] = 0;
				}
				_mesh->m_pMeshInfo->optimizeVertex.clear();
				_mesh->m_pMeshInfo->optimizeVertex.shrink_to_fit();

				delete _mesh->m_pMeshInfo;
			}
		}
		m_pObjMeshList.clear();
		m_pObjMeshList.shrink_to_fit();
	}

	m_pFbxScene->Destroy();
	m_pFbxImporter->Destroy();
	m_pFbxManager->Destroy();

	return true;
}

bool FBXLibrary_Internal::Load(tstring filename)
{
	std::string _temp;
	_temp.assign(filename.begin(), filename.end());

	bool _bResulrt = m_pFbxImporter->Initialize(_temp.c_str(), -1, m_pFbxManager->GetIOSettings());
	_bResulrt = m_pFbxImporter->Import(m_pFbxScene);

	// Axis를 DirectX에 맞게 변환
	//m_pFbxScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::DirectX);

	FbxGeometryConverter* geometryConverter = new FbxGeometryConverter(m_pFbxManager);

	geometryConverter->Triangulate(m_pFbxScene, true, true);

	if (_bResulrt)
	{
		// FBX는 트리구조
		// 재귀호출로 순회 가능, N 트리여서 자식 수를 알아야 함.
		FbxNode* _pRootNode = m_pFbxScene->GetRootNode();

		// 씬에 저장 된 트리구조를 순회해서 오브젝트를 찾아옴.
		ProcessNode(nullptr, _pRootNode);
		ProcessSkeleton(nullptr, _pRootNode);
	}

	return true;
}

void FBXLibrary_Internal::ProcessNode(FbxNode* pParentNode, FbxNode* pNode)
{
	// mesh가 아니라면 return
	if (pNode->GetCamera() || pNode->GetLight()) return;

	if (pNode->GetMesh())
	{
		FbxMesh* _pMesh = pNode->GetMesh();

		// FBX Node가 트리구조에서 가져와 저장해 놓자. 
		FBXNodePointer* _pNode = new FBXNodePointer();

		_pNode->m_pFBXParentNode = pParentNode;
		_pNode->m_pFBXMyNode = pNode;

		m_FBXNodeHierarchy.emplace_back(_pNode);

		// 미리 저장 공간 생성. 
		TL_FBXLibrary::FBXNodeInfo* fbx = new TL_FBXLibrary::FBXNodeInfo();
		m_pObjMeshList.emplace_back(fbx);
	}

	const unsigned int _numChildNode = pNode->GetChildCount();

	for (unsigned int i = 0; i < _numChildNode; ++i)
	{
		FbxNode* _child = pNode->GetChild(i);
		ProcessNode(pNode, _child);
	}
}

void FBXLibrary_Internal::ProcessSkeleton(FbxNode* pParentNode, FbxNode* pNode)
{
	// 뼈를 가져온다. 
	if (pNode->GetSkeleton())
	{
		// FBX Node가 트리구조에서 가져와 저장해 놓자. 
		FBXNodePointer* _pNode = new FBXNodePointer();
		_pNode->m_pFBXParentNode = pParentNode;
		_pNode->m_pFBXMyNode = pNode;
		m_FBXNodeHierarchy.emplace_back(_pNode);

		// 뼈에 대한 정보 저장. 
		TL_FBXLibrary::FBXNodeInfo* fbx = new TL_FBXLibrary::FBXNodeInfo();

		fbx->m_pSkeleton = new TL_FBXLibrary::MySkeleton();
		fbx->m_pSkeleton->parentBoneIndex = -1;

		if (pParentNode != nullptr)
		{
			std::string _parentName = pParentNode->GetName();
			//fbx->m_pSkeleton->parentboneName.assign(_parentName.begin(), _parentName.end());;
			tstring _parentNameCopy; 
			_parentNameCopy.assign(_parentName.begin(), _parentName.end());

			fbx->m_pSkeleton->parentBoneIndex = FindBoneIndex(_parentNameCopy);
		}

		std::string _myName = pNode->GetName();
		fbx->m_pSkeleton->boneName.assign(_myName.begin(), _myName.end());
		m_pBoneList.emplace_back(fbx->m_pSkeleton);
	}

	const unsigned int _numChildNode = pNode->GetChildCount();

	for (int i = 0; i < _numChildNode; ++i)
	{
		FbxNode* _child = pNode->GetChild(i);
		ProcessSkeleton(pNode, _child);
	}
}

int FBXLibrary_Internal::FindBoneIndex(tstring parentName)
{
	if (m_pBoneList.size() == 0) return -1;

	for (int i = 0; i < m_pBoneList.size(); ++i)
	{
		if (m_pBoneList[i]->boneName == parentName)
		{
			return i;
		}
	}
}

void FBXLibrary_Internal::AllConvertOptimize()
{
	if (m_pObjMeshList.size() == 0) return;

	for (int i = 0; i < m_pObjMeshList.size(); ++i)
	{
		Convert_Optimize(i);
	}
}

void FBXLibrary_Internal::Convert_Optimize(int index)
{
	// TODO:: 애니메이션 고려 안함.
	//FbxMesh* _pMesh = pNodeInfo->m_pFBXMyNode->GetMesh();
	FbxMesh* _pMesh = m_FBXNodeHierarchy.at(index)->m_pFBXMyNode->GetMesh();

	//pNodeInfo->m_pMeshInfo = new hyobin_FBX::MeshInfo();
	//auto* _pInfo = pNodeInfo->m_pMeshInfo;
	m_pObjMeshList.at(index)->m_pMeshInfo = new TL_FBXLibrary::MeshInfo();
	auto* _pInfo = m_pObjMeshList.at(index)->m_pMeshInfo;

	unsigned int triCount = _pMesh->GetPolygonCount(); // 메쉬의 삼각형 개수를 가져온다.

	// cotrol point는 vertex의 물리적 정보
	FbxVector4* _vertices = _pMesh->GetControlPoints();

	for (unsigned int i = 0; i < triCount; ++i) // 삼각형의 개수
	{
		TL_FBXLibrary::MyFace _pFace;
		_pInfo->optimizeFace.emplace_back(_pFace);

		for (unsigned int j = 0; j < 3; ++j) // 삼각형은 세 개의 정점으로 구성
		{
			int controlPointIndex = _pMesh->GetPolygonVertex(i, j); // 제어점 인덱스를 가져온다.

			FbxVector4 _fbxNormal;
			_pMesh->GetPolygonVertexNormal(i, j, _fbxNormal);

			TL_FBXLibrary::MyVertex _pVertex;

			_pVertex.pos[0] = _vertices[controlPointIndex].mData[0];
			_pVertex.pos[1] = _vertices[controlPointIndex].mData[2];
			_pVertex.pos[2] = _vertices[controlPointIndex].mData[1];

			_pVertex.normal[0] = _fbxNormal.mData[0];
			_pVertex.normal[1] = _fbxNormal.mData[2];
			_pVertex.normal[2] = _fbxNormal.mData[1];

			int _uvIndex = _pMesh->GetTextureUVIndex(i, j);
			_pVertex.uv[0] = _pMesh->GetElementUV()->GetDirectArray().GetAt(_uvIndex).mData[0];
			_pVertex.uv[1] = 1 - _pMesh->GetElementUV()->GetDirectArray().GetAt(_uvIndex).mData[1];

			_pInfo->optimizeVertex.emplace_back(_pVertex);
			_pInfo->optimizeFace[i].index[j] = _pInfo->optimizeVertex.size() - 1;
		}
	}
}

int FBXLibrary_Internal::GetObjMeshListSize()
{
	if (m_pObjMeshList.size() == 0) return -1; 

	return m_pObjMeshList.size();
}

int FBXLibrary_Internal::GetVertexSize(TL_FBXLibrary::FBXNodeInfo* pNodeInfo)
{
	if (pNodeInfo->m_pMeshInfo->optimizeVertex.size() == 0) return -1;

	return pNodeInfo->m_pMeshInfo->optimizeVertex.size();
}

int FBXLibrary_Internal::GetIndexSize(TL_FBXLibrary::FBXNodeInfo* pNodeInfo)
{
	if (pNodeInfo->m_pMeshInfo->optimizeFace.size()== 0) return -1;

	return pNodeInfo->m_pMeshInfo->optimizeFace.size();
}

