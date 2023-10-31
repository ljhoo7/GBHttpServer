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

	bool GBShared::OnReceived(VectoredIO& inputData, const unsigned long transferredBytes)
	{
		static BUFFER_SIZE_TYPE messageID = 0;

		switch (inputData.GetState())
		{
		case VectoredIO::STATE::ID:
		{
			if (Gather(inputData, transferredBytes))
			{
				messageID = reinterpret_cast<decltype(messageID)>(inputData.m_buffer);
				inputData.m_length = sizeof(inputData.m_length);
			}
		}
		break;
		case VectoredIO::STATE::LENGTH:
		{
			if (Gather(inputData, transferredBytes))
			{
				inputData.m_length = reinterpret_cast<BUFFER_SIZE_TYPE>(inputData.m_buffer);
			}
		}
		break;
		case VectoredIO::STATE::PAYLOAD:
		{
			if (Gather(inputData, transferredBytes))
			{
				OnGatheringCompleted(inputData, messageID);
				inputData.m_length = sizeof(messageID);
			}
		}
		break;
		default:
		{
			assert(false);
		}
		break;
		}

		return true;
	}

	bool GBShared::OnSent(VectoredIO& outputData, const unsigned long transferredBytes)
	{
		return true;
	}
}