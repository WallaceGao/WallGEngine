#pragma once
#include "Model.h"

using namespace WallG::Graphics;

namespace WallG::Graphics::MaterialIO
{
    void Write(FILE* file, const Model::MaterialData& materials); // use fprintf : is to write formatted data to the file
    void Read(FILE* file, Model::MaterialData& materials); // use fscanf
    void WirteBinary(FILE* file, const Model::MaterialData& materials); // use fwritef : is to write the file
    void ReadBinary(FILE* file, Model::MaterialData& materials); // use freadf
}