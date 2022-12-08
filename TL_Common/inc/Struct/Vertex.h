#pragma once
#include "Math/TL_Math.h"

namespace TL_Common
{
    // 78 bytes
    struct StaticMeshVertex
    {
        TL_Math::Vector3 position;
        TL_Math::Vector2 uv;
        TL_Math::Vector3 normal;
        TL_Math::Vector3 tangent;
        TL_Math::Vector3 bitangent;
    };

    // 110 bytes (78 bytes + 32 bytes)
    struct SkeletalMeshVertex :
        public StaticMeshVertex
    {
        int boneIndex[4];
        float boneWeight[4];
    };
}
