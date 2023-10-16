#include "GBShared.h"

namespace GenericBoson
{
	bool GBShared::Gather(VectoredIO& vectoredIO,
		const DWORD transferredBytes)
	{
		if (vectoredIO.m_length <= vectoredIO.m_offset + transferredBytes)
		{
			vectoredIO.m_offset = 0;
			vectoredIO.AdvanceState();

			return true;
		}

		vectoredIO.m_offset += transferredBytes;

		return false;
	}
}