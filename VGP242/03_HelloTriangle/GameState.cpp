#include "GameState.h"

using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

void GameState::Initialize()
{
    // NDC - Normalized Device Coordinate
    //
    //            +-----------------+
    //           /                 /|
    //     1.0  +-----------------+ |
    //          |        ^        | |
    //          |        |        | |
    //          | <------+------> | |
    //          |        |        | | 1.0
    //          |        v        |/
    //    -1.0  +-----------------+ 0.0
    //      -1.0               1.0
    //
    mVertices.push_back({ Vector3{  0.0f,  0.5f, 0.0f }, Colors::Red });
    mVertices.push_back({ Vector3{  0.0f, -0.5f, 0.0f }, Colors::Green });
    mVertices.push_back({ Vector3{ -0.5f, -0.5f, 0.0f }, Colors::Blue });
    mVertices.push_back({ Vector3{  0.0f,  0.5f, 0.0f }, Colors::Blue });
    mVertices.push_back({ Vector3{  0.5f, -0.5f, 0.0f }, Colors::Blue });
    mVertices.push_back({ Vector3{  0.0f, -0.5f, 0.0f }, Colors::Blue });
    auto device = GraphicsSystem::Get()->GetDevice();

    // -------------------------------------------
    // Create vertex buffer
    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = mVertices.size() * sizeof(Vertex);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = mVertices.data();

    HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
    ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");
    // -------------------------------------------

    // -------------------------------------------
    // Compile and create vertex shader
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    hr = D3DCompileFromFile(
        L"../../Assets/Shaders/DoNothing.fx",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VS", "vs_5_0",
        shaderFlags, 0, &shaderBlob, &errorBlob);
    if (errorBlob && errorBlob->GetBufferPointer())
        LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
    ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader");

    hr = device->CreateVertexShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        &mVertexShader);
    ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

    // Define vertex element descriptions
    std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
    vertexLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
    vertexLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

    // Create the input layout
    hr = device->CreateInputLayout(
        vertexLayout.data(),
        (UINT)vertexLayout.size(),
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        &mInputLayout);
    ASSERT(SUCCEEDED(hr), "Failed to create input layout.");

    SafeRelease(shaderBlob);
    SafeRelease(errorBlob);
    // -------------------------------------------

    // -------------------------------------------
    // Compile and create pixel shader
    hr = D3DCompileFromFile(
        L"../../Assets/Shaders/DoNothing.fx",
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
    // -------------------------------------------
}

void GameState::Terminate()
{
    SafeRelease(mVertexBuffer);
    SafeRelease(mVertexShader);
    SafeRelease(mInputLayout);
    SafeRelease(mPixelShader);
}

void GameState::Update(float deltaTime)
{
}

void GameState::Render()
{
    auto context = GraphicsSystem::Get()->GetContext();

    context->IASetInputLayout(mInputLayout);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

    context->VSSetShader(mVertexShader, nullptr, 0);
    context->PSSetShader(mPixelShader, nullptr, 0);

    context->Draw((UINT)mVertices.size(), 0);
}

// Device interface - used for VRAM allocation/resource creation
// Context interface - used to issue draw commands
