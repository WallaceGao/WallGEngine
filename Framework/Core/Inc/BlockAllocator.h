#pragma once

namespace WallG::Core
{
	class BlockAllocator
	{
	public:
		BlockAllocator(std::size_t blockSize, std::size_t capacity);
		~BlockAllocator();

		BlockAllocator(const BlockAllocator&) = delete;
		BlockAllocator& operator=(const BlockAllocator&) =delete;

		BlockAllocator(const BlockAllocator&&) = delete;
		BlockAllocator& operator=(const BlockAllocator&&) = delete;

		void* Allocate();
		void Free(void* ptr);

	private:

		std::vector<uint32_t> mFreeSlots;
		void* mData = nullptr;
		std::size_t mBlockSize = 0;
		std::size_t mCapacity = 0;
		//struct Pool
		//{
		//};
		//std::vector<Pool> mPools;
	};
}

#define  MEMPOOL_DECLARE\
	static WallG::Core::BlockAllocator sAllocator;\
	static void* operator new(std::size_t size);\
	static void operator delete(void* ptr);

#define MEMPOOL_DEFINE(Class, Capacity)\
	WallG::Core::BlockAllocator Class::sAllocator(sizeof(Class), Capacity); \
	void* Class::operator new(std::size_t size)\
	{\
		return sAllocator.Allocate();\
	}\
	void Class::operator delete(void* ptr)\
	{\
		sAllocator.Free(ptr);\
	}