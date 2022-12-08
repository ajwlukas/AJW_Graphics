#pragma once

#include <vector>
#include "Common/Common_typedef.h"

namespace TL_FBXLibrary
{
	struct MyFace
	{
		UINT index[3] = {};
	};

	struct MyVertex
	{
		float pos[3] = {};			// 좌표상의 위치값

		float uv[2] = {};
		 
		float normal[3] = {};		// 노말값

		float tangent[3] = {};		// 노말값

		float bitangent[3] = {};		// 노말값

		//bool bTextureSet;	// 텍스쳐값이 셋팅됐는가?

		// 인덱스
		// int indices;

		// int* indicesPointer;

		/// Skinned Mesh 에서 추가
		// weight 9개 : 최대 9개 bone의 영향을 받을 수 있다. 
		// BYTE m_weightIndices[9];
	};

	struct MeshInfo
	{
		// 메모리 카피는 일어나지만 주소로 저장하면 메모리공간을 절약할 수 있다. 
		// 쉽게 꺼낼 수 있게 포인터 제거
		std::vector<MyVertex> optimizeVertex;
		std::vector<MyFace> optimizeFace;
	};

	struct MySkeleton
	{
		tstring boneName;
		// tstring parentboneName; // 디버깅 용

		int parentBoneIndex;
	};

	struct FBXNodeInfo
	{
		MeshInfo* m_pMeshInfo;

		MySkeleton* m_pSkeleton;
	};
}
