#include "Precompiled.h"
#include "BlockAllocator.h"

#include "DebugUtil.h"

using namespace WallG::Core;

BlockAllocator::BlockAllocator(std::size_t blockSize, std::size_t capacity)
	: mBlockSize(blockSize)
	, mCapacity(capacity)
{
	mFreeSlots.clear();
	mFreeSlots.reserve(capacity);
	for (size_t i = 0; i < capacity; ++i)
	{
		mFreeSlots.push_back(i);
	}
	mData = malloc(blockSize * capacity);
}

BlockAllocator::~BlockAllocator()
{
	mBlockSize = 0;
	mCapacity = 0;
	mFreeSlots.clear();
	free(mData);
}

void* BlockAllocator::Allocate()
{
	if (mFreeSlots.empty())
	{
		return nullptr;
	}

	int slot = mFreeSlots.back();

	void* newData = static_cast<char*>(mData) + slot * mBlockSize;
	mFreeSlots.pop_back();

	return newData;
}

void BlockAllocator::Free(void* ptr)
{
	if (ptr == nullptr)
	{
		return;
	}

	auto slot = (static_cast<char*>(ptr) - mData) / mBlockSize;
	mFreeSlots.push_back(slot);
}

