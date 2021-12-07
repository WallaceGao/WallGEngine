#include "Precompiled.h"
#include "SimpleDraw.h"

#include "Camera.h"
#include "ConstantBuffer.h"
#include "GraphicsSystem.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "VertexTypes.h"
#include "Texture.h"
#include "TextureManager.h"
#include "TextureType.h"

using namespace WallG;
using namespace WallG::Graphics;

namespace
{
	struct CBSimpleDraw
	{
		Math::Matrix4 transform;
	};


	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount);
		void Terminate();

		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
		void AddLine2D(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
		void AddCircle2D(const Math::Circle& circle, const Color& color);

		void Render(const Camera& camera);

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		ConstantBuffer mConstantBuffer;
		MeshBuffer mMeshBuffer;
		std::unique_ptr<VertexPC[]> mLineVertices;
		std::unique_ptr<VertexPC[]> mLineVertices2D;
		uint32_t mLineVertexCount = 0;
		uint32_t mLineVertexCount2D = 0;
		uint32_t mMaxVertexCount = 0;
		uint32_t mScreenWidth = 0;
		uint32_t mScreenHeight = 0;
		Math::Matrix4 mTransform = Math::Matrix4::Indentity;
	};

	void SimpleDrawImpl::Initialize(uint32_t maxVertexCount)
	{
		mVertexShader.Initialize("../../Assets/Shaders/SimpleDraw.fx", VertexPC::Format);
		mPixelShader.Initialize("../../Assets/Shaders/SimpleDraw.fx");
		mConstantBuffer.Initialize(sizeof(Math::Matrix4));
		mMeshBuffer.Initialize(nullptr, sizeof(VertexPC), maxVertexCount, true);
		mLineVertices = std::make_unique<VertexPC[]>(maxVertexCount);
		mLineVertices2D = std::make_unique<VertexPC[]>(maxVertexCount);
		mLineVertexCount = 0;
		mLineVertexCount2D = 0;
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

	// 2D don't use VertexPC
	void SimpleDrawImpl::AddLine2D(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
	{
		if (mLineVertexCount2D + 2 <= mMaxVertexCount)
		{
			mLineVertices2D[mLineVertexCount2D++] = { v0, color };
			mLineVertices2D[mLineVertexCount2D++] = { v1, color };
		}
	}


	void SimpleDrawImpl::AddCircle2D(const Math::Circle& circle, const Color& color)
	{
		// Check if we have enough space
		if (mLineVertexCount2D + 32 <= mMaxVertexCount)
		{
			float x = circle.center.x;
			float y = circle.center.y;
			float r = circle.radius;

			// Add line
			const float kAngle = Math::Constants::Pi / 8.0f;
			for (uint32_t i = 0; i < 16; ++i)
			{
				const float alpha = i * kAngle;
				const float beta = alpha + kAngle;
				const float x0 = x + (r * sin(alpha));
				const float y0 = y + (r * cos(alpha));
				const float x1 = x + (r * sin(beta));
				const float y1 = y + (r * cos(beta));
				mLineVertices2D[mLineVertexCount2D++] = { Math::Vector3(x0, y0, 0.0f), color };
				mLineVertices2D[mLineVertexCount2D++] = { Math::Vector3(x1, y1, 0.0f), color };
			}
		}
	}

	void SimpleDrawImpl::Render(const Camera& camera)
	{
		auto matView = camera.GetViewMatrix();
		auto matProj = camera.GetProjectionMatrix();
		auto transform = Math::Transpose(matView * matProj);

		mVertexShader.Bind();
		mPixelShader.Bind();

		if (mLineVertexCount > 0)
		{
			mConstantBuffer.Update(&transform);
			mConstantBuffer.BindVS(0);
			mMeshBuffer.Update(mLineVertices.get(), mLineVertexCount);
			mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
			mMeshBuffer.Render();
		}

		auto gs = GraphicsSystem::Get();
		auto context = gs->GetContext();

		if (mLineVertexCount2D > 0)
		{
			const uint32_t w = (mScreenWidth == 0) ? gs->GetBackBufferWidth() : mScreenWidth;
			const uint32_t h = (mScreenHeight == 0) ? gs->GetBackBufferHeight() : mScreenHeight;
			Math::Matrix4 matInvScreen
			(
				2.0f / w, 0.0f, 0.0f, 0.0f,
				0.0f, -2.0f / h, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				-1.0f, 1.0f, 0.0f, 1.0f
			);

			auto transform = Math::Transpose(matInvScreen);
			mConstantBuffer.Update(&transform);
			mConstantBuffer.BindVS(0);
			mMeshBuffer.Update(mLineVertices2D.get(), mLineVertexCount2D);
			mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
			mMeshBuffer.Render();
		}


		mLineVertexCount2D = 0;
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

void SimpleDraw::AddLine2D(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color)
{
	sInstance->AddLine2D(Math::Vector3{ v0.x,v0.y, 0.0f }, Math::Vector3{ v1.x,v1.y, 0.0f }, color);
}

void SimpleDraw::AddLine2D(float x0, float y0, float x1, float y1, const Color& color)
{
	sInstance->AddLine2D(Math::Vector3(x0, y0, 0.0f), Math::Vector3(x1, y1,0.0f), color);
}

void SimpleDraw::AddCircle2D(const Math::Circle& circle, const Color& color)
{
	sInstance->AddCircle2D(circle, color);
}

void SimpleDraw::AddCircle2D(const Math::Vector2& center, float r, const Color& color)
{
	sInstance->AddCircle2D(Math::Circle(center, r), color);
}

void SimpleDraw::AddBone(const Math::Vector3& postion, const Math::Vector3& childPosition, const Color& color, const float scale, uint32_t numberOfLine)
{
	AddSphere(postion, scale, color, 16, 16);
	//const uint32_t kSlices = Math::Max(3u, numberOfLine);
	//const float kPhi = Math::Constants::TwoPi / (float)kSlices;
	//
	//for (size_t i = 0; i < numberOfLine; ++i)
	//{
	//	const float theta = i * kPhi;
	//	const float x0 = postion.x + (scale * sin(theta));
	//	const float y0 = postion.y;
	//	const float z0 = postion.z + (scale * cos(theta));
	//	AddLine({ x0,y0,z0 }, childPosition, color);
	//}
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