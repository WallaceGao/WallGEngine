#include "Precompiled.h"
#include "UniverseService.h"

#include "GameObject.h"
#include "PlanetModelComponet.h"
#include "TransformComponent.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

void UniverseService::Initialize()
{

    //mPlantPosition.resize(7);
    //mShipLocation.resize(3);
}

void UniverseService::Terminate()
{
    ////Ship
    //mShipMeshBuffer.Terminate();
    //mShipTexture.Terminate();
}

void UniverseService::Update(float deltaTime)
{


    //
    //auto direction = mPlantPosition[targetPlanet] - mShipLocation[0];
    //if (Magnitude(direction) < 10.0f)
    //{
    //    targetPlanet = std::rand() % 7;
    //}
    //mShipLocation[0] += Normalize(direction) * deltaTime * 50.0f;
}


    ////// Sun
    //mSunTexture.BindPS(0);
    //matWorld = Matrix4::RotationX(mSumRotation.x) * Matrix4::RotationY(mSumRotation.y) * Matrix4::Translation({ 0.0f,0.0f,0.0f });
    //auto scale = Matrix4::Scaling(mPlanetScale[0]);
    //transformData.wvp = Transpose(scale * matWorld * matView * matProj);
    //mConstantBuffer.Update(transformData);
    //mSphereBuffer.Render();
    //
    //// 41- 43 is XYZ
    //mPlantPosition[0] = { matWorld._41,matWorld._42, matWorld._43 };
    //// Mercury
    //mMercuryTexture.BindPS(0);
    //matWorld = Matrix4::RotationY(mSelfMercuryRotation.y) * Matrix4::Translation({ 15.0f,0.0f,0.0f }) * Matrix4::RotationY(mMercuryRotation.y);
    //scale = Matrix4::Scaling(mPlanetScale[1]);
    //transformData.wvp = Transpose(scale * matWorld * matView * matProj);
    //mConstantBuffer.Update(transformData);
    //mSphereBuffer.Render();
    //
    //// 41- 43 is XYZ
    //mPlantPosition[1] = { matWorld._41,matWorld._42, matWorld._43 };
    //// Venus
    //mVenusTexture.BindPS(0);
    //matWorld = Matrix4::RotationY(mSelfVenusRotation.y) * Matrix4::Translation({ 30.0f,0.0f,0.0f }) * Matrix4::RotationY(mVenusRotation.y);
    //scale = Matrix4::Scaling(mPlanetScale[2]);
    //transformData.wvp = Transpose(scale * matWorld * matView * matProj);
    //mConstantBuffer.Update(transformData);
    //mSphereBuffer.Render();
    //
    //// 41- 43 is XYZ
    //mPlantPosition[2] = { matWorld._41,matWorld._42, matWorld._43 };
    //// Earth
    //mEarthTexture.BindPS(0);
    //matWorld = Matrix4::RotationY(mSelfEarthRotation.y) * Matrix4::Translation({ 45.0f,0.0f,0.0f }) * Matrix4::RotationY(mEarthRotation.y);
    //scale = Matrix4::Scaling(mPlanetScale[3]);
    //transformData.wvp = Transpose(scale * matWorld * matView * matProj);
    //mConstantBuffer.Update(transformData);
    //mSphereBuffer.Render();
    //
    //// 41- 43 is XYZ
    //mPlantPosition[3] = { matWorld._41,matWorld._42, matWorld._43 };
    //
    //// Moon
    //mMoonTexture.BindPS(0);
    ////
    //auto matMoon = Matrix4::RotationY(mSelfMoonRotation.y) * Matrix4::Translation({ 5.0f,0.0f,0.0f }) * Matrix4::RotationY(mMoonRotation.y);
    //scale = Matrix4::Scaling(mPlanetScale[6]);
    //// mat world come from earth and we need keep it
    //transformData.wvp = Transpose(scale * matMoon * matWorld * matView * matProj);
    //mConstantBuffer.Update(transformData);
    //mSphereBuffer.Render();
    //// 41- 43 is XYZ
    //mPlantPosition[4] = { matMoon._41, matMoon._42,  matMoon._43 };
    //
    //// Mars
    //mMarsTexture.BindPS(0);
    //matWorld = Matrix4::RotationY(mSelfMarsRotation.y) * Matrix4::Translation({ 60.0f,0.0f,0.0f }) * Matrix4::RotationY(mMarsRotation.y);
    //scale = Matrix4::Scaling(mPlanetScale[4]);
    //transformData.wvp = Transpose(scale * matWorld * matView * matProj);
    //mConstantBuffer.Update(transformData);
    //mSphereBuffer.Render();
    //mPlantPosition[5] = { matWorld._41,matWorld._42, matWorld._43 };
    //
    //// Jupiter
    //mJupiterTexture.BindPS(0);
    //matWorld = Matrix4::RotationY(mSelfJupiterRotation.y) * Matrix4::Translation({ 75.0f,0.0f,0.0f }) * Matrix4::RotationY(mJupiterRotation.y);
    //scale = Matrix4::Scaling(mPlanetScale[5]);
    //transformData.wvp = Transpose(scale * matWorld * matView * matProj);
    //mConstantBuffer.Update(transformData);
    //mSphereBuffer.Render();
    //mPlantPosition[6] = { matWorld._41,matWorld._42, matWorld._43 };


void UniverseService::DebugUI()
{

}

//