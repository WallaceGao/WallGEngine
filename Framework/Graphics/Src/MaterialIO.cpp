#include "Precompiled.h"
#include "MaterialIO.h"

using namespace WallG::Graphics;



void MaterialIO::Write(FILE* file, const Model::MaterialData& materials)
{
	fprintf_s(file, "Material: %f %f %f %f %f %f %f %f %f %f %f %f\n",
		materials.material.ambient.r,
		materials.material.ambient.g,
		materials.material.ambient.b,
		materials.material.ambient.a,
		materials.material.diffuse.r,
		materials.material.diffuse.g,
		materials.material.diffuse.b,
		materials.material.diffuse.a,
		materials.material.specular.r,
		materials.material.specular.g,
		materials.material.specular.b,
		materials.material.specular.a);
	fprintf_s(file, "MaterialPower: %f\n", materials.material.Power);

	if (materials.diffuseMapName.empty())
		fprintf_s(file, "DiffuseMapName: <none>\n");
	else
		fprintf_s(file, "DiffuseMapName: %s\n", materials.diffuseMapName.c_str());

	if (materials.normalMapName.empty())
		fprintf_s(file, "NormalMapName: <none>\n");
	else
		fprintf_s(file, "NormalMapName: %s\n", materials.normalMapName.c_str());

	if (materials.specularMapName.empty())
		fprintf_s(file, "SpecularMapName: <none>\n");
	else
		fprintf_s(file, "SpecularMapName: %s\n", materials.specularMapName.c_str());
}

void MaterialIO::Read(FILE* file, Model::MaterialData& materials)
{
	fscanf_s(file, "Material: %f %f %f %f %f %f %f %f %f %f %f %f\n",
		&materials.material.ambient.r,
		&materials.material.ambient.g,
		&materials.material.ambient.b,
		&materials.material.ambient.a,
		&materials.material.diffuse.r,
		&materials.material.diffuse.g,
		&materials.material.diffuse.b,
		&materials.material.diffuse.a,
		&materials.material.specular.r,
		&materials.material.specular.g,
		&materials.material.specular.b,
		&materials.material.specular.a);
	fscanf_s(file, "MaterialPower: %f\n", &materials.material.Power);

	char buffer[128] = "";
	fscanf_s( file, "DiffuseMapName: %s\n", buffer, static_cast<uint32_t>(std::size(buffer)));
	if (std::string(buffer) != "<none>")
	{
		materials.diffuseMapName = buffer;
	}

	fscanf_s( file, "NormalMapName: %s\n", buffer, static_cast<uint32_t>(std::size(buffer)));
	if (std::string(buffer) != "<none>")
	{
		materials.normalMapName = buffer;
	}

	fscanf_s( file, "SpecularMapName: %s\n", buffer, static_cast<uint32_t>(std::size(buffer)));
	if (std::string(buffer) != "<none>")
	{
		materials.specularMapName = buffer;
	}
}

//void MaterialIO::WirteBinary(FILE* file, const Model::MaterialData& materials)
//{
//	
//}
//
//void MaterialIO::ReadBinary(FILE* file, Model::MaterialData& materials)
//{
//
//}
