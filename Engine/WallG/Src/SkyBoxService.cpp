#include "Precompiled.h"
#include "SkyBoxService.h"

#include "CameraService.h"
#include "GameWorld.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

void SkyBoxService::Initialize()
{
    mSkyboxMesh = MeshBuilder::CreatSkyboxPX(500.0f);
    mSkyboxMeshBuffer.Initialize(mSkyboxMesh);
    //mSkyboxTexture.Initialize("../../Assets/Images/cubemap.png");
    mSkyboxTexture.Initialize("../../Assets/Images/Skybox2.png");
    mVertexShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
    mPixelShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx");
    mConstantBuffer. Initialize();

    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
    mDisableDepthWrite.Initialize(false, false);
}

void SkyBoxService::Terminate()
{
    mDisableDepthWrite.Terminate();
    mSampler.Terminate();
    mConstantBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mSkyboxTexture.Terminate();
    mSkyboxMeshBuffer.Terminate();
}

void SkyBoxService::Render()
{
    auto cameraService = GetWorld().GetService<CameraService>();
    Matrix4 matWorld = Matrix4::Translation(cameraService->GetCamera().GetPosition());
    Matrix4 matView = cameraService->GetCamera().GetViewMatrix();
    Matrix4 matProj = cameraService->GetCamera().GetProjectionMatrix();

    TransformData transformData;
    transformData.wvp = Transpose(matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mConstantBuffer.BindVS(0);

    mVertexShader.Bind();
    mPixelShader.Bind();

    mSampler.BindPS(0);

    mSkyboxTexture.BindPS(0);

    mDisableDepthWrite.Set();
    mSkyboxMeshBuffer.Render();
    DepthStencilState::ClearState();
}