#pragma once

#include "FBXLibrary/dll.h"
#include "Common.h"
#include <vector>

namespace TL_FBXLibrary
{
	struct FBXNodeInfo;
}

class FBXLibrary_Internal;

namespace TL_FBXLibrary
{
	class FBX_LIBRARY_API FBXModelLoader
	{
	public:
		FBXModelLoader();
		~FBXModelLoader();

		// 로드 함수들
		bool Init();
		bool Release();

		bool Load(tstring filename); 
		bool AllLoad(); // 경로에 있는 모든 파일을 읽어오는 함수

		void FBXConvertOptimize();

		std::vector<TL_FBXLibrary::FBXNodeInfo*> GetMeshList();
		int GetObjMeshListSize();
		// Get vertex, index info
		int GetVertexSize(FBXNodeInfo* pNodeInfo);
		int GetIndexSize(FBXNodeInfo* pNodeInfo);

	private:
		FBXLibrary_Internal* m_FBXLoader;
	};
}


