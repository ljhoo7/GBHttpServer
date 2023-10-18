#include "GBShared.h"

namespace GenericBoson
{
	bool GBShared::Gather(VectoredIO& vectoredIO,
		const unsigned long transferredBytes)
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

	bool GBShared::ErrorLog(const std::string_view msg)
	{
		return true;
	}

	bool GBShared::WarningLog(const std::string_view msg)
	{
		return true;
	}

	bool GBShared::InfoLog(const std::string_view msg)
	{
		return true;
	}
}