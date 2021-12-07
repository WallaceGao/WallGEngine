#pragma once
#include "TextureManager.h"

namespace WallG::Graphics
{
    class Texture
    {
    public:
        static void UnbindPS(uint32_t slot);
    public:
        enum class Format
        {
            RGBA_U8,
            RGBA_F32
        };

        Texture() = default;
        virtual ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        virtual void Initialize(const char* fileName) ;
        virtual void Initialize(const std::filesystem::path& fileName);
        virtual void Initialize(uint32_t width, uint32_t height, Format format = Format::RGBA_U8);
        virtual void Terminate();

        void BindVS(uint32_t slot) const;
        void BindPS(uint32_t slot) const;
    
        void* GetRawData() const { return mShaderResourceView; }

        ID3D11ShaderResourceView* GetShaderResourceView() const { return mShaderResourceView; }
        uint32_t GetWidth() const { return mWidth; }
        uint32_t GetHeight() const { return mHeight; }
        
        bool GetIsInitialize() const { return mIsInitialize; }


    protected:
        ID3D11ShaderResourceView* mShaderResourceView = nullptr;
        uint32_t mWidth = 0;
        uint32_t mHeight = 0;
    private:
        bool mIsInitialize = false;
    };
}
