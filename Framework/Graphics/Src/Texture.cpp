#include "Precompiled.h"
#include "Texture.h"

#include "GraphicsSystem.h"
#include <DirectXTK/Inc/WICTextureLoader.h>

using namespace WallG;
using namespace WallG::Graphics;


Texture::~Texture()
{
    ASSERT(mShaderResourceView == nullptr, "Texture -- Terminate must be called before destruction.");
}

void Texture::Initialize(const char* fileName)
{
    auto device = GraphicsSystem::Get()->GetDevice();
    auto context = GraphicsSystem::Get()->GetContext();
    wchar_t wbuffer[1024];
    mbstowcs_s(nullptr, wbuffer, fileName, 1024);

    HRESULT hr = DirectX::CreateWICTextureFromFile(device, wbuffer, nullptr, &mShaderResourceView);
    ASSERT(SUCCEEDED(hr), "Texture -- Failed to load texture.");

    ID3D11Resource* resource = nullptr;
    mShaderResourceView->GetResource(&resource);

    ID3D11Texture2D* texture = static_cast<ID3D11Texture2D*>(resource);
    D3D11_TEXTURE2D_DESC desc = {};
    texture->GetDesc(&desc);

    mWidth = desc.Width;
    mHeight = desc.Height;
    mIsInitialize = true;
}

void Texture::Initialize(const std::filesystem::path& fileName)
{
    auto device = GraphicsSystem::Get()->GetDevice();
    auto context = GraphicsSystem::Get()->GetContext();
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, context, fileName.c_str(), nullptr, &mShaderResourceView);
    ASSERT(SUCCEEDED(hr), "Texture -- Failed to load texture %ls.", fileName.c_str());
    mIsInitialize = true;
}

void Texture::Initialize(uint32_t width, uint32_t height, Format format)
{
    ASSERT(false, "Texture -- Not implemented");
    mIsInitialize = true;
}

void Texture::Terminate()
{
    SafeRelease(mShaderResourceView);
}

void Texture::BindVS(uint32_t slot) const
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->VSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Texture::BindPS(uint32_t slot) const
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

void WallG::Graphics::Texture::UnbindPS(uint32_t slot)
{
    static ID3D11ShaderResourceView* dummy = nullptr;
    GraphicsSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &dummy);
}



