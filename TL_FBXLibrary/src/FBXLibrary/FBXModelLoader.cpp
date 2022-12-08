
#include "FBXLibrary/FBXModelLoader.h"

#include "FBXLibrary_Internal.h"

TL_FBXLibrary::FBXModelLoader::FBXModelLoader()
	:m_FBXLoader(nullptr)
{
}

TL_FBXLibrary::FBXModelLoader::~FBXModelLoader()
{
}

bool TL_FBXLibrary::FBXModelLoader::Init()
{
	m_FBXLoader = new FBXLibrary_Internal();
	bool _result = m_FBXLoader->Init();

	return _result;
}

bool TL_FBXLibrary::FBXModelLoader::Release()
{
	bool _result = m_FBXLoader->Release();

	return _result;
}

bool TL_FBXLibrary::FBXModelLoader::Load(tstring filename)
{
	bool _result = m_FBXLoader->Load(filename);

	// 가져온 data를 최적화

	return _result;
}

bool TL_FBXLibrary::FBXModelLoader::AllLoad()
{
	/// TODO : 구현 해 놓자.

	return  true;
}

void TL_FBXLibrary::FBXModelLoader::FBXConvertOptimize()
{
	m_FBXLoader->AllConvertOptimize();
}

std::vector<TL_FBXLibrary::FBXNodeInfo*> TL_FBXLibrary::FBXModelLoader::GetMeshList()
{
	return m_FBXLoader->m_pObjMeshList;
}

int TL_FBXLibrary::FBXModelLoader::GetObjMeshListSize()
{
	return m_FBXLoader->GetObjMeshListSize();
}

int TL_FBXLibrary::FBXModelLoader::GetVertexSize(FBXNodeInfo* pNodeInfo)
{
	return m_FBXLoader->GetVertexSize(pNodeInfo);
}

int TL_FBXLibrary::FBXModelLoader::GetIndexSize(FBXNodeInfo* pNodeInfo)
{
	return m_FBXLoader->GetIndexSize(pNodeInfo);
}
