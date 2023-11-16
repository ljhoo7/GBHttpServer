#include "GBShared.h"

#include <iostream>

namespace GenericBoson
{
	bool GBShared::Gather(VectoredIO& vectoredIO,
		const unsigned long transferredBytes)
	{
		// offset means previous received size
		if (vectoredIO.m_length <= vectoredIO.m_offset + transferredBytes)
		{
			vectoredIO.m_offset = 0;
			vectoredIO.AdvanceState();

			return true;
		}

		vectoredIO.m_offset += transferredBytes;

		return false;
	}

	void PrintMessage(const std::string_view msg)
	{
		//std::cout << msg.data() << "\n";
	}

	bool GBShared::ErrorLog(const std::string_view msg)
	{
		PrintMessage(msg);
		return true;
	}

	bool GBShared::WarningLog(const std::string_view msg)
	{
		PrintMessage(msg);
		return true;
	}

	bool GBShared::InfoLog(const std::string_view msg)
	{
		PrintMessage(msg);
		return true;
	}

	bool GBShared::ReadWholePartialMessages(VectoredIO& inputData, const unsigned long transferredBytes)
	{
		switch (inputData.GetState())
		{
		case VectoredIO::STATE::ID:
		{
			if (Gather(inputData, transferredBytes))
			{
				// store received ID
				inputData.m_messageID = *reinterpret_cast<int32_t*>(&inputData.m_buffer[0]);

				// set length size to be read
				inputData.m_length = sizeof(inputData.m_length);

				return true;
			}
		}
		break;
		case VectoredIO::STATE::LENGTH:
		{
			if (Gather(inputData, transferredBytes))
			{
				// store received length and set payload size to be read
				
				inputData.m_length = *reinterpret_cast<int32_t*>(
					&inputData.m_buffer[sizeof(inputData.m_messageID)]
				);

				return true;
			}
		}
		break;
		case VectoredIO::STATE::PAYLOAD:
		{
			if (Gather(inputData, transferredBytes))
			{
				OnGatheringCompleted(inputData);

				// set ID size to be read
				inputData.m_length = sizeof(inputData.m_messageID);
			}
		}
		break;
		default:
		{
			std::abort();
		}
		break;
		}

		return false;
	}

	bool GBShared::OnReceived(VectoredIO& inputData, const unsigned long transferredBytes)
	{
		while (1)
		{
			if (!ReadWholePartialMessages(inputData, transferredBytes))
			{
				break;
			}
		}

		return true;
	}

	bool GBShared::OnSent(VectoredIO& outputData, const unsigned long transferredBytes)
	{
		return true;
	}
}