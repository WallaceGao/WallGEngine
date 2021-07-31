#pragma once

namespace WallG::Graphics
{
    class ConstantBuffer
    {
    public:
        ConstantBuffer() = default;
        ~ConstantBuffer();

        ConstantBuffer(const ConstantBuffer&) = delete;
        ConstantBuffer& operator=(const ConstantBuffer&) = delete;

        void Initialize(uint32_t bufferSize);
        void Terminate();

        void Update(const void* data) const;

        void BindVS(uint32_t slot) const;
        void BindPS(uint32_t slot) const;

    private:
        ID3D11Buffer* mConstantBuffer = nullptr;
    };

    template <class T>
    class TypedConstantBuffer : public ConstantBuffer
    {
    public:
        using DataType = T;

        void Initialize()
        {
            static_assert(sizeof(DataType) % 16 == 0, "Data type is must be 16 byte aligned.");
            ConstantBuffer::Initialize(sizeof(DataType));
        }

        void Update(const DataType& data) const
        {
            ConstantBuffer::Update(&data);
        }
    };
}

