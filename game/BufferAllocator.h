#pragma once

#include "flatbuffers/allocator.h"

#include <iostream>

namespace GenericBoson
{
	class SendBufferManager
	{
	public:
		SendBufferManager()
		{

		}

		virtual ~SendBufferManager()
		{

		}
	};

	class ThreadSafeBufferAllocator : public flatbuffers::Allocator
	{
	public:
		uint8_t* allocate(size_t size) override
		{
			std::cout << "allocated : " << size << "\n";
			return new uint8_t[size];
		}

		void deallocate(uint8_t* p, size_t size) override
		{
			std::cout << "deallocated : " << size << "\n";
			delete[] p;
		}
	};
}