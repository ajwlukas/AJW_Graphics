#pragma once

#include "fbxsdk.h"
#include <Common.h>
#include "Math/TL_Math.h"
#include <vector>

//#include "FBXParserData.h"

namespace TL_FBXLibrary
{
	//struct MeshInfo;
	struct MySkeleton;
	struct FBXNodeInfo;
}

struct FBXNodePointer
{
	FbxNode* m_pFBXParentNode;
	FbxNode* m_pFBXMyNode;
};

class FBXLibrary_Internal
{
public:
	FBXLibrary_Internal();
	~FBXLibrary_Internal();

public:
	bool Init();
	bool Release();
	bool Load(tstring filename);

private:
	// Scene 의 정보 해석 
	void ProcessNode(FbxNode* pParentNode, FbxNode* pNode); // 루트 노드부터 재귀 함수를 통해 오브젝트 개수와 종류를 알아옴
	void ProcessSkeleton(FbxNode* pParentNode, FbxNode* pNode);

	int FindBoneIndex(tstring parentName);

public:
	// 최적화
	void AllConvertOptimize();
	void Convert_Optimize(int index);

	// GetMeshInfo
	int GetObjMeshListSize();

	// Get vertex, index info
	int GetVertexSize(TL_FBXLibrary::FBXNodeInfo* pNodeInfo);
	int GetIndexSize(TL_FBXLibrary::FBXNodeInfo* pNodeInfo);

private:
	FbxManager* m_pFbxManager;
	FbxImporter* m_pFbxImporter;
	FbxScene* m_pFbxScene;

public:
	std::vector<FBXNodePointer*> m_FBXNodeHierarchy;
	std::vector<TL_FBXLibrary::FBXNodeInfo*> m_pObjMeshList;
	std::vector<TL_FBXLibrary::MySkeleton*> m_pBoneList;
};
