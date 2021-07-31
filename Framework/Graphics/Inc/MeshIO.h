#pragma once
#include "MeshTypes.h"

namespace WallG::Graphics::MeshIO
{
    void Write(FILE* file, const SkinnedMesh& mesh); // use fprintf : is to write formatted data to the file
    void Read(FILE* file, SkinnedMesh& mesh); // use fscanf
    void WirteBinary(FILE* file, const SkinnedMesh& mesh); // use fwritef : is to write the file
    void ReadBinary(FILE* file, SkinnedMesh& mesh); // use freadf
}

