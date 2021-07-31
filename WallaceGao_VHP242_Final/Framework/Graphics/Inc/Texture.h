#pragma once

namespace WallG::Graphics
{
    class Texture
    {
    public:
        Texture() = default;
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        void Initialize(const std::filesystem::path& fileName);
        void Terminate();

        void BindVS(uint32_t slot) const;
        void BindPS(uint32_t slot) const;

    private:
        ID3D11ShaderResourceView* mShaderResourceView = nullptr;
    };
}
