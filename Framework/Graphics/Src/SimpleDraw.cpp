#include "Precompiled.h"
#include "SimpleDraw.h"

#include "Camera.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "VertexTypes.h"

using namespace WallG;
using namespace WallG::Graphics;

namespace
{
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount);
		void Terminate();

		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
		void Render(const Camera& camera);

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		ConstantBuffer mConstantBuffer;
		MeshBuffer mMeshBuffer;
		std::unique_ptr<VertexPC[]> mLineVertices;
		uint32_t mLineVertexCount = 0;
		uint32_t mMaxVertexCount = 0;
	};

	void SimpleDrawImpl::Initialize(uint32_t maxVertexCount)
	{
		mVertexShader.Initialize("../../Assets/Shaders/SimpleDraw.fx", VertexPC::Format);
		mPixelShader.Initialize("../../Assets/Shaders/SimpleDraw.fx");
		mConstantBuffer.Initialize(sizeof(Math::Matrix4));
		mMeshBuffer.Initialize(nullptr, sizeof(VertexPC), maxVertexCount, true);
		mLineVertices = std::make_unique<VertexPC[]>(maxVertexCount);
		mLineVertexCount = 0;
		mMaxVertexCount = maxVertexCount;
	}

	void SimpleDrawImpl::Terminate()
	{
		mMeshBuffer.Terminate();
		mConstantBuffer.Terminate();
		mPixelShader.Terminate();
		mVertexShader.Terminate();
	}

	void SimpleDrawImpl::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
	{
		if (mLineVertexCount + 2 <= mMaxVertexCount)
		{
			mLineVertices[mLineVertexCount++] = VertexPC{ v0, color };
			mLineVertices[mLineVertexCount++] = VertexPC{ v1, color };
		}
	}

	void SimpleDrawImpl::Render(const Camera& camera)
	{
		auto matView = camera.GetViewMatrix();
		auto matProj = camera.GetProjectionMatrix();
		auto transform = Math::Transpose(matView * matProj);
		mConstantBuffer.Update(&transform);
		mConstantBuffer.BindVS(0);

		mVertexShader.Bind();
		mPixelShader.Bind();

		mMeshBuffer.Update(mLineVertices.get(), mLineVertexCount);
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
		mMeshBuffer.Render();

		mLineVertexCount = 0;
	}

	std::unique_ptr<SimpleDrawImpl> sInstance;
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
	sInstance = std::make_unique<SimpleDrawImpl>();
	sInstance->Initialize(maxVertexCount);
}

void SimpleDraw::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}

void SimpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
{
	sInstance->AddLine(v0, v1, color);
}

void WallG::Graphics::SimpleDraw::AddAABB(const Math::Vector3& center, const Math::Vector3& extend, const Color& color)
{
	// Call sInstance->AddLine 8 times here ...
	float minX = center.x - extend.x;
	float minY = center.y - extend.y;
	float minZ = center.z - extend.z;
	float maxX = center.x + extend.x;
	float maxY = center.y + extend.y;
	float maxZ = center.z + extend.z;

	sInstance->AddLine({ minX, minY, minZ }, { minX, minY, maxZ }, color);
	sInstance->AddLine({ maxX, minY, minZ }, { maxX, minY, maxZ }, color);
	sInstance->AddLine({ minX, maxY, minZ }, { minX, maxY, maxZ }, color);
	sInstance->AddLine({ maxX, maxY, minZ }, { maxX, maxY, maxZ }, color);

	sInstance->AddLine({ minX, minY, minZ }, { maxX, minY, minZ }, color);
	sInstance->AddLine({ minX, minY, maxZ }, { maxX, minY, maxZ }, color);
	sInstance->AddLine({ minX, maxY, minZ }, { maxX, maxY, minZ }, color);
	sInstance->AddLine({ minX, maxY, maxZ }, { maxX, maxY, maxZ }, color);

	sInstance->AddLine({ minX, minY, minZ }, { minX, maxY, minZ }, color);
	sInstance->AddLine({ maxX, minY, minZ }, { maxX, maxY, minZ }, color);
	sInstance->AddLine({ minX, minY, maxZ }, { minX, maxY, maxZ }, color);
	sInstance->AddLine({ maxX, minY, maxZ }, { maxX, maxY, maxZ }, color);
}

void WallG::Graphics::SimpleDraw::AddSphere(const Math::Vector3& center, float radius, const Color& color, uint32_t slices, uint32_t rings)
{

	const float x = center.x;
	const float y = center.y;
	const float z = center.z;

	const uint32_t kSlices = Math::Max(3u, slices);
	const uint32_t kRings = Math::Max(2u, rings);
	const uint32_t kLines = (4 * kSlices * kRings) - (2 * kSlices);

	// Check if we have enough space
		// Add lines
	const float kTheta = Math::Constants::Pi / (float)kRings;
	const float kPhi = Math::Constants::TwoPi / (float)kSlices;
	for (uint32_t j = 0; j < kSlices; ++j)
	{
		for (uint32_t i = 0; i < kRings; ++i)
		{
			const float a = i * kTheta;
			const float b = a + kTheta;
			const float ay = radius * cos(a);
			const float by = radius * cos(b);

			const float theta = j * kPhi;
			const float phi = theta + kPhi;

			const float ar = sqrt(radius * radius - ay * ay);
			const float br = sqrt(radius * radius - by * by);

			const float x0 = x + (ar * sin(theta));
			const float y0 = y + (ay);
			const float z0 = z + (ar * cos(theta));

			const float x1 = x + (br * sin(theta));
			const float y1 = y + (by);
			const float z1 = z + (br * cos(theta));

			const float x2 = x + (br * sin(phi));
			const float y2 = y + (by);
			const float z2 = z + (br * cos(phi));

			sInstance->AddLine({ x0,y0,z0 }, {x1,y1,z1}, color);

			if (i < kRings - 1)
			{
				sInstance->AddLine({ x1, y1, z1 }, { x2,y2,z2 }, color);
			}
		}
	}
}

void WallG::Graphics::SimpleDraw::AddRing(const Math::Vector3& center, float radius, const Color& color, uint32_t slices)
{
	const float x = center.x;
	const float y = center.y;
	const float z = center.z;

	const uint32_t kSlices = Math::Max(3u, slices);
	const float kPhi = Math::Constants::TwoPi / (float)kSlices;

	for (uint32_t j = 0; j < kSlices; ++j)
	{
		const float theta = j * kPhi;
		const float phi = theta + kPhi;

		const float x0 = x + (radius * sin(theta));
		const float y0 = y;
		const float z0 = z + (radius * cos(theta));

		const float x1 = x + (radius * sin(phi));
		const float y1 = y;
		const float z1 = z + (radius * cos(phi));

		sInstance->AddLine({ x0,y0,z0 }, { x1,y1,z1 }, color);
	}
}

void SimpleDraw::Render(const Camera& camera)
{
	sInstance->Render(camera);
}