#include "Shared.h"

#include <iostream>

namespace GenericBoson
{
	bool Shared::Gather(VectoredIO& vectoredIO,
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

	bool Shared::ErrorLog(const std::string_view msg)
	{
		PrintMessage(msg);
		return true;
	}

	bool Shared::WarningLog(const std::string_view msg)
	{
		PrintMessage(msg);
		return true;
	}

	bool Shared::InfoLog(const std::string_view msg)
	{
		PrintMessage(msg);
		return true;
	}

	bool Shared::ReadWholePartialMessages(VectoredIO& inputData, const unsigned long transferredBytes)
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

	bool Shared::OnReceived(VectoredIO& inputData, const unsigned long transferredBytes)
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

	bool Shared::OnSent(VectoredIO& outputData, const unsigned long transferredBytes)
	{
		return true;
	}
}