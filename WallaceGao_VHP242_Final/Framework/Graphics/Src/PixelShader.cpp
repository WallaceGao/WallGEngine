#include "Precompiled.h"
#include "PixelShader.h"
#include "GraphicsSystem.h"

using namespace WallG::Graphics;

PixelShader::~PixelShader()
{
    ASSERT(mPixelShader == nullptr, "PixelShader == Terminate must be called before destruction");
}

void PixelShader::Initialize(const std::filesystem::path& filePath)
{
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    auto device = GraphicsSystem::Get()->GetDevice();
    // -------------------------------------------
    // Compile and create pixel shader
     HRESULT hr = D3DCompileFromFile(
        filePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "PS", "ps_5_0",
        shaderFlags, 0, &shaderBlob, &errorBlob);
    if (errorBlob && errorBlob->GetBufferPointer())
        LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
    ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader");

    hr = device->CreatePixelShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        &mPixelShader);
    ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");

    SafeRelease(shaderBlob);
    SafeRelease(errorBlob);
}

void PixelShader::Terminate()
{
	SafeRelease(mPixelShader);
}

void PixelShader::Bind() const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetShader(mPixelShader, nullptr, 0);

}
