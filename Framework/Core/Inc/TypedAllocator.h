#pragma once
#include "BlockAllocator.h"

namespace WallG::Core
{
	template<class Datatype>
	class TypedAllocator final: private BlockAllocator 
	{
		//malloc - allocates memory
		// new - allocates memory(step 1), then calls constructor(step 2)
	public: 
		TypedAllocator(std::size_t capacity):
			BlockAllocator(sizeof(Datatype), capacity) 
		{}

		Datatype* New()
		{
			Datatype* instance = static_cast<Datatype*>(Allocate());
			if (instance != nullptr)
				new(instance) Datatype();
			return instance;
		}

		void Delete(Datatype* ptr)
		{
			if (ptr == nullptr)
			{
				return;
			}

			ptr->~Datatype();
			Free(ptr);
		}

	};
}