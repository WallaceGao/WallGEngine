#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace WallG::Graphics;
using namespace WallG::Math;

Mesh MeshBuilder::CreatSphere(uint32_t rings, uint32_t slices, float radius)
{
	Mesh mesh;

	const float ringStep = Math::Constants::Pi / rings;
	const float sliceStep = Math::Constants::TwoPi / slices;
	const float uStep = 1.0f / slices;
	const float vStep = 1.0f / rings;
	float phi = 0.0f;
	float theta = 0.0f;

	for (uint32_t r = 0; r <= rings; ++r)
	{
		const float y = radius * cosf(phi);
		theta = 0.0f;
		const float v = r * vStep;
		for (uint32_t s = 0; s <= slices; ++s)
		{
			const float x = radius * sinf(phi) * sinf(theta);
			const float z = radius * sinf(phi) * -cosf(theta);
			const float u = s * uStep;

			const Vector3 nomal = Math::Normalize({ x,y,z }); 
			mesh.vertices.push_back({ 
				{ x,y,z },					// position
				{nomal},					// normal
				Normalize({-z, 0.0f, x}),	// tangent
				{u,v}						// uv
				});	
			theta += sliceStep;
		}
		phi += ringStep;
	}
	for (uint32_t y = 0; y < rings; y++)
	{
		for (uint32_t x = 0; x < slices; x++)
		{
			const uint32_t stride = slices + 1;
			mesh.indices.push_back(y * stride + x);
			mesh.indices.push_back((y + 1) * stride + x + 1);
			mesh.indices.push_back((y + 1) * stride + x);

			mesh.indices.push_back(y * stride + x);
			mesh.indices.push_back(y * stride + x + 1);
			mesh.indices.push_back((y + 1) * stride + x + 1);
		}
	}
	return mesh;
}

Mesh MeshBuilder::CreatPlane(uint32_t rows, uint32_t columns, float spacing)
{
	Mesh mesh;
	float xOffset = (columns - 1) * spacing * 0.5f;
	float zOffset = (rows - 1) * spacing * 0.5f;
	for (uint32_t c = 0; c < columns; ++c)
	{
		float v = static_cast<float>(c) / static_cast<float>(columns);
		for (uint32_t r = 0; r < rows; ++r)
		{
			float x = c * spacing - xOffset;
			float z = r * spacing - zOffset;

			float u = static_cast<float>(r) / static_cast<float>(rows);
			mesh.vertices.push_back({ Vector3(x, 0, z), Vector3::YAxis, Vector3::ZAxis ,Vector2(u, v) });
		}
	}
	for (uint32_t y = 0; y < columns - 1; y++)
	{
		for (uint32_t x = 0; x < rows - 1; x++)
		{
			mesh.indices.push_back((y + 0) * rows + x);
			mesh.indices.push_back((y + 1) * rows + x + 1);
			mesh.indices.push_back((y + 1) * rows + x);
			mesh.indices.push_back((y + 0) * rows + x);
			mesh.indices.push_back((y + 0) * rows + x + 1);
			mesh.indices.push_back((y + 1) * rows + x + 1);
		}
	}
	return mesh;
}

MeshPC MeshBuilder::CreatCubePC()
{
	MeshPC mesh;

    //front
    mesh.vertices.emplace_back(VertexPC{ Vector3{-1.0f,-1.0f, -1.0f }, Colors::Red });
    mesh.vertices.emplace_back(VertexPC{ Vector3{-1.0f, 1.0f, -1.0f }, Colors::Red });
    mesh.vertices.emplace_back(VertexPC{ Vector3{ 1.0f,-1.0f, -1.0f }, Colors::Red });
    mesh.vertices.emplace_back(VertexPC{ Vector3{ 1.0f, 1.0f, -1.0f }, Colors::Red });
   
    //Back
    mesh.vertices.emplace_back(VertexPC{ Vector3{-1.0f,-1.0f,  1.0f }, Colors::Green });
    mesh.vertices.emplace_back(VertexPC{ Vector3{ 1.0f,-1.0f,  1.0f }, Colors::Green });
    mesh.vertices.emplace_back(VertexPC{ Vector3{-1.0f, 1.0f,  1.0f }, Colors::Green });
    mesh.vertices.emplace_back(VertexPC{ Vector3{ 1.0f, 1.0f,  1.0f }, Colors::Green });

    //Front
    mesh.indices.push_back(2);
    mesh.indices.push_back(1);
    mesh.indices.push_back(0);
    mesh.indices.push_back(2);
    mesh.indices.push_back(3);
    mesh.indices.push_back(1);

    //Back
    mesh.indices.push_back(5);
    mesh.indices.push_back(6);
    mesh.indices.push_back(4);
    mesh.indices.push_back(7);
    mesh.indices.push_back(6);
    mesh.indices.push_back(5);

    // Left 2604
    mesh.indices.push_back(6);
    mesh.indices.push_back(2);
    mesh.indices.push_back(4);
    mesh.indices.push_back(4);
    mesh.indices.push_back(2);
    mesh.indices.push_back(0);

    // Right 3715
    mesh.indices.push_back(3);
    mesh.indices.push_back(5);
    mesh.indices.push_back(1);
    mesh.indices.push_back(3);
    mesh.indices.push_back(7);
    mesh.indices.push_back(5);

    // Top 2367
    mesh.indices.push_back(2);
    mesh.indices.push_back(6);
    mesh.indices.push_back(3);
    mesh.indices.push_back(6);
    mesh.indices.push_back(7);
    mesh.indices.push_back(3);

    // Bottom 0145
    mesh.indices.push_back(0);
    mesh.indices.push_back(1);
    mesh.indices.push_back(4);
    mesh.indices.push_back(4);
    mesh.indices.push_back(1);
    mesh.indices.push_back(5);

	return mesh;
}

MeshPX MeshBuilder::CreatCubePX()
{
    MeshPX mesh;

	//front vertices
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f } });
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f } });
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f } });
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } });

	//back vertices
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 0.5f }, { 1.0f, 0.0f } });
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 0.5f }, { 0.0f, 0.0f } });
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 0.5f }, { 0.0f, 1.0f } });
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f } });

	//left vertices
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 0.5f }, { 0.0f, 0.0f } });
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f } });
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f } });
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.5f }, { 0.0f, 1.0f } });

	//right vertices
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f } });
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 0.5f }, { 1.0f, 0.0f } });
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f } });
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } });

	//top vertices
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 0.5f }, { 0.0f, 0.0f } });
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 0.5f }, { 1.0f, 0.0f } });
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } });
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f } });

	//button vertices
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f } });
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f } });
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f } });
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.5f }, { 0.0f, 1.0f } });

	//Front index
	mesh.indices.push_back(0);
	mesh.indices.push_back(1);
	mesh.indices.push_back(2);
	mesh.indices.push_back(0);
	mesh.indices.push_back(2);
	mesh.indices.push_back(3);

	//Back
	mesh.indices.push_back(5);
	mesh.indices.push_back(4);
	mesh.indices.push_back(7);
	mesh.indices.push_back(5);
	mesh.indices.push_back(7);
	mesh.indices.push_back(6);

	//Right
	mesh.indices.push_back(12);
	mesh.indices.push_back(13);
	mesh.indices.push_back(14);
	mesh.indices.push_back(12);
	mesh.indices.push_back(14);
	mesh.indices.push_back(15);

	//Left
	mesh.indices.push_back(8);
	mesh.indices.push_back(9);
	mesh.indices.push_back(10);
	mesh.indices.push_back(8);
	mesh.indices.push_back(10);
	mesh.indices.push_back(11);

	//Top
	mesh.indices.push_back(16);
	mesh.indices.push_back(17);
	mesh.indices.push_back(18);
	mesh.indices.push_back(16);
	mesh.indices.push_back(18);
	mesh.indices.push_back(19);

	//Bottom
	mesh.indices.push_back(20);
	mesh.indices.push_back(21);
	mesh.indices.push_back(23);
	mesh.indices.push_back(21);
	mesh.indices.push_back(22);
	mesh.indices.push_back(23);

	return mesh;
}

MeshPX MeshBuilder::CreatPlanePX(uint32_t rows, uint32_t columns, float spacing)
{
	MeshPX mesh;
	float xOffset = (columns - 1) * spacing * 0.5f;
	float zOffset = (rows - 1) * spacing * 0.5f;
	for (uint32_t c = 0; c < columns; ++c)
	{
		float v = static_cast<float>(c) / static_cast<float>(columns);
		for (uint32_t r = 0; r < rows; ++r)
		{
			float x = c * spacing - xOffset;
			float z = r * spacing - zOffset;

			float u = static_cast<float>(r) / static_cast<float>(rows);
			mesh.vertices.push_back({ Vector3(x, 0, z), Vector2(u, v) });
		}

	}
	for (uint32_t y = 0; y < columns-1; y++)
	{
		for (uint32_t x = 0; x < rows-1; x++)
		{
			mesh.indices.push_back(y * rows + x);
			mesh.indices.push_back((y + 1) * rows + x + 1);
			mesh.indices.push_back((y + 1) * rows + x);
			mesh.indices.push_back(y * rows + x);
			mesh.indices.push_back(y * rows + x + 1);
			mesh.indices.push_back((y + 1) * rows + x + 1);
		}
	}
	return mesh;
}

MeshPX MeshBuilder::CreatCylinderPX( uint32_t rings, uint32_t slices , float height, float radius)
{
    MeshPX mesh;

    const float ringStep = height / (rings - 1);
	const float sliceStep = Math::Constants::TwoPi / slices;
	const float uStep = 1.0f / slices;
	const float vStep = 1.0f / (rings-1);

	for (uint32_t r = 0; r < rings; ++r)
	{
		const float y = r * ringStep;
		const float v = 1.0f - (r * vStep);
		for (uint32_t s = 0; s <= slices; ++s)
		{
			const float x = -sin(s * sliceStep) * radius;
			const float z = cos(s * sliceStep) * radius;
			const float u = s * uStep;
			mesh.vertices.push_back({ { x,y,z }, {u,v} });
		}
	}
	for (uint32_t r = 0; r + 1 < rings; ++r)
	{
		for (uint32_t s = 0; s < slices; ++s)
		{
			mesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			mesh.indices.push_back((s + 0) + ((r + 1) * (slices + 1)));
			mesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));

			mesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			mesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));
			mesh.indices.push_back((s + 1) + ((r + 0) * (slices + 1)));
		}
	}
    return mesh;
}

MeshPX MeshBuilder::CreatSpherePX(uint32_t rings, uint32_t slices, float radius)
{
	MeshPX mesh;

	const float ringStep = Math::Constants::Pi / rings;
	const float sliceStep = Math::Constants::TwoPi / slices;
	const float uStep = 1.0f / slices;
	const float vStep = 1.0f / rings;
	float phi = 0.0f;
	float theta = 0.0f;


	for (uint32_t r = 0; r <= rings; ++r)
	{
		const float y = radius * cosf(phi);
		const float v = r * vStep;
		theta = 0.0f;
		for (uint32_t s = 0; s <= slices; ++s)
		{
			const float x = radius * sinf(phi) * sinf(theta);
			const float z = radius * sinf(phi) * -cosf(theta);
			const float u = s * uStep;
			mesh.vertices.push_back({ { x,y,z }, {u,v} });
			theta += sliceStep;
		}
		phi += ringStep;
	}
	for (uint32_t y = 0; y < rings; y++)
	{
		for (uint32_t x = 0; x < slices; x++)
		{
			const uint32_t stride = slices + 1;
			mesh.indices.push_back(y * stride + x);
			mesh.indices.push_back((y + 1) * stride + x + 1);
			mesh.indices.push_back((y + 1) * stride + x);
			
			mesh.indices.push_back(y * stride + x);
			mesh.indices.push_back(y * stride + x + 1);
			mesh.indices.push_back((y + 1) * stride + x + 1);
		}
	}
	return mesh;
}

MeshPX MeshBuilder::CreatSkyboxPX( float size)
{
	MeshPX mesh;

	//front vertices
	mesh.vertices.push_back({ Vector3{ -size, -size, -size }, { 1.0f, 0.665f } });
	mesh.vertices.push_back({ Vector3{ -size,  size, -size }, { 1.0f, 0.334f } });
	mesh.vertices.push_back({ Vector3{  size,  size, -size }, { 0.75f, 0.334f  } });
	mesh.vertices.push_back({ Vector3{  size, -size, -size }, { 0.75f, 0.665f } });

	//back vertices
	mesh.vertices.push_back({ Vector3{  size, -size, size }, { 0.5f, 0.665f } });
	mesh.vertices.push_back({ Vector3{  size,  size, size }, { 0.5f, 0.334f } });
	mesh.vertices.push_back({ Vector3{ -size,  size, size }, { 0.25f, 0.334f } });
	mesh.vertices.push_back({ Vector3{ -size, -size, size }, { 0.25f, 0.665f } });

	//left vertices
	mesh.vertices.push_back({ Vector3{ -size, -size,  size }, { 0.25f, 0.665f } });
	mesh.vertices.push_back({ Vector3{ -size,  size,  size }, { 0.25f, 0.334f } });
	mesh.vertices.push_back({ Vector3{ -size,  size, -size }, { 0.0f, 0.334f } });
	mesh.vertices.push_back({ Vector3{ -size, -size, -size }, { 0.0f, 0.665f } });

	//right vertices
	mesh.vertices.push_back({ Vector3{  size, -size, -size }, { 0.75f, 0.665f } });
	mesh.vertices.push_back({ Vector3{  size,  size, -size }, { 0.75f, 0.334f } });
	mesh.vertices.push_back({ Vector3{  size,  size,  size }, { 0.5f, 0.334f } });
	mesh.vertices.push_back({ Vector3{  size, -size,  size }, { 0.5f, 0.665f } });

	//top vertices
	mesh.vertices.push_back({ Vector3{ -size,  size, -size }, { 0.25f, 0.001f} });
	mesh.vertices.push_back({ Vector3{ -size,  size,  size }, { 0.25f, 0.33f } });
	mesh.vertices.push_back({ Vector3{  size,  size,  size }, { 0.5f, 0.33f  } });
	mesh.vertices.push_back({ Vector3{  size,  size, -size }, { 0.5f, 0.001f } });

	//button vertices
	mesh.vertices.push_back({ Vector3{ -size, -size,  size }, { 0.25f, 0.66f }  });
	mesh.vertices.push_back({ Vector3{ -size, -size, -size }, { 0.25f, 0.999f}  });
	mesh.vertices.push_back({ Vector3{  size, -size, -size }, { 0.5f,  0.999f}  });
	mesh.vertices.push_back({ Vector3{  size, -size,  size }, { 0.5f,  0.66f }  });

	//Front index
	mesh.indices.push_back(0);
	mesh.indices.push_back(2);
	mesh.indices.push_back(1);
	mesh.indices.push_back(0);
	mesh.indices.push_back(3);
	mesh.indices.push_back(2);

	//Back
	mesh.indices.push_back(4);
	mesh.indices.push_back(6);
	mesh.indices.push_back(5);
	mesh.indices.push_back(4);
	mesh.indices.push_back(7);
	mesh.indices.push_back(6);

	//Right
	mesh.indices.push_back(12);
	mesh.indices.push_back(14);
	mesh.indices.push_back(13);
	mesh.indices.push_back(12);
	mesh.indices.push_back(15);
	mesh.indices.push_back(14);

	//Left
	mesh.indices.push_back(8);
	mesh.indices.push_back(10);
	mesh.indices.push_back(9);
	mesh.indices.push_back(8);
	mesh.indices.push_back(11);
	mesh.indices.push_back(10);

	//Top
	mesh.indices.push_back(16);
	mesh.indices.push_back(18);
	mesh.indices.push_back(17);
	mesh.indices.push_back(16);
	mesh.indices.push_back(19);
	mesh.indices.push_back(18);

	//Bottom
	mesh.indices.push_back(20);
	mesh.indices.push_back(22);
	mesh.indices.push_back(21);
	mesh.indices.push_back(20);
	mesh.indices.push_back(23);
	mesh.indices.push_back(22);

	return mesh;
}

MeshPN MeshBuilder::CreatCubePN()
{
	MeshPN mesh;

	//front vertices
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, -0.5f }, {0.0f , 0.0f, 1.0f} });
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f, -0.5f }, {0.0f , 0.0f, 1.0f} });
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f, -0.5f }, {0.0f , 0.0f, 1.0f} });
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, -0.5f }, {0.0f , 0.0f, 1.0f} });

	//back vertices
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 0.5f }, {0.0f , 0.0f, -1.0f} });
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 0.5f }, {0.0f , 0.0f, -1.0f} });
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 0.5f }, {0.0f , 0.0f, -1.0f} });
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.5f }, {0.0f , 0.0f, -1.0f} });

	//left vertices
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f,  0.5f }, { -1.0f , 0.0f, 0.0f} });
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, -0.5f }, { -1.0f , 0.0f, 0.0f} });
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, -0.5f }, { -1.0f , 0.0f, 0.0f} });
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f,  0.5f }, { -1.0f , 0.0f, 0.0f} });

	//right vertices
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f, -0.5f }, { 1.0f , 0.0f, 0.0f} });
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f,  0.5f }, { 1.0f , 0.0f, 0.0f} });
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f,  0.5f }, { 1.0f , 0.0f, 0.0f} });
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f, -0.5f }, { 1.0f , 0.0f, 0.0f} });

	//top vertices
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f,  0.5f }, { 0.0f , 1.0f, 0.0f} });
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f,  0.5f }, { 0.0f , 1.0f, 0.0f} });
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f, -0.5f }, { 0.0f , 1.0f, 0.0f} });
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, -0.5f }, { 0.0f , 1.0f, 0.0f} });

	//button vertices
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, -0.5f }, { 0.0f , -1.0f, 0.0f} });
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f, -0.5f }, { 0.0f , -1.0f, 0.0f} });
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f,  0.5f }, { 0.0f , -1.0f, 0.0f} });
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f,  0.5f }, { 0.0f , -1.0f, 0.0f} });

	//Front index
	mesh.indices.push_back(0);
	mesh.indices.push_back(1);
	mesh.indices.push_back(2);
	mesh.indices.push_back(0);
	mesh.indices.push_back(2);
	mesh.indices.push_back(3);

	//Back
	mesh.indices.push_back(5);
	mesh.indices.push_back(4);
	mesh.indices.push_back(7);
	mesh.indices.push_back(5);
	mesh.indices.push_back(7);
	mesh.indices.push_back(6);

	//Right
	mesh.indices.push_back(12);
	mesh.indices.push_back(13);
	mesh.indices.push_back(14);
	mesh.indices.push_back(12);
	mesh.indices.push_back(14);
	mesh.indices.push_back(15);

	//Left
	mesh.indices.push_back(8);
	mesh.indices.push_back(9);
	mesh.indices.push_back(10);
	mesh.indices.push_back(8);
	mesh.indices.push_back(10);
	mesh.indices.push_back(11);

	//Top
	mesh.indices.push_back(16);
	mesh.indices.push_back(17);
	mesh.indices.push_back(18);
	mesh.indices.push_back(16);
	mesh.indices.push_back(18);
	mesh.indices.push_back(19);

	//Bottom
	mesh.indices.push_back(20);
	mesh.indices.push_back(21);
	mesh.indices.push_back(23);
	mesh.indices.push_back(21);
	mesh.indices.push_back(22);
	mesh.indices.push_back(23);

	return mesh;
}

MeshPN MeshBuilder::CreatSpherePN(uint32_t rings, uint32_t slices, float radius)
{
	MeshPN mesh;

	const float ringStep = Math::Constants::Pi / rings;
	const float sliceStep = Math::Constants::TwoPi / slices;
	float phi = 0.0f;
	float theta = 0.0f;

	for (uint32_t r = 0; r <= rings; ++r)
	{
		const float y = radius * cosf(phi);
		theta = 0.0f;
		for (uint32_t s = 0; s <= slices; ++s)
		{
			const float x = radius * sinf(phi) * sinf(theta);
			const float z = radius * sinf(phi) * -cosf(theta);
			
			const Vector3 nomal = Math::Normalize({x,y,z});
			mesh.vertices.push_back({ { x,y,z }, {nomal} });
			theta += sliceStep;
		}
		phi += ringStep;
	}
	for (uint32_t y = 0; y < rings; y++)
	{
		for (uint32_t x = 0; x < slices; x++)
		{
			const uint32_t stride = slices + 1;
			mesh.indices.push_back(y * stride + x);
			mesh.indices.push_back((y + 1) * stride + x + 1);
			mesh.indices.push_back((y + 1) * stride + x);

			mesh.indices.push_back(y * stride + x);
			mesh.indices.push_back(y * stride + x + 1);
			mesh.indices.push_back((y + 1) * stride + x + 1);
		}
	}
	return mesh;
}
