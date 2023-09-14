#pragma once

#include "flatbuffers/allocator.h"

namespace GenericBoson
{
	class BufferAllocator : public flatbuffers::Allocator
	{
	public:
		uint8_t* allocate(size_t size) override {
			return new uint8_t[size];
		}

		void deallocate(uint8_t* p, size_t) override { delete[] p; }
	};
}