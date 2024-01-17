#include "Shared.h"
#include "StubAdaptor.h"

#include <iostream>

namespace GenericBoson
{
	bool Common::Gather(VectoredIO& vectoredIO,
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

	bool Common::ErrorLog(const std::string_view msg)
	{
		PrintMessage(msg);
		return true;
	}

	bool Common::WarningLog(const std::string_view msg)
	{
		PrintMessage(msg);
		return true;
	}

	bool Common::InfoLog(const std::string_view msg)
	{
		PrintMessage(msg);
		return true;
	}

	bool Common::ReadWholePartialMessages(VectoredIO& inputData, const unsigned long transferredBytes)
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

	bool Common::OnReceived(VectoredIO& inputData, const unsigned long transferredBytes)
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

	bool Common::OnSent(VectoredIO& outputData, const unsigned long transferredBytes)
	{
		return true;
	}

	bool Common::OnGatheringCompleted(VectoredIO& inputData)
	{
		const auto pStub = m_stubs.find(inputData.m_messageID);
		if (pStub == m_stubs.end())
		{
			ErrorLog(std::format("receive packet handler not found. - messageID : {}", inputData.m_messageID));
			return false;
		}

		const auto payloadStartOffset = sizeof(inputData.m_messageID) + sizeof(inputData.m_length);
		pStub->second->CallStub(&inputData.m_buffer[payloadStartOffset]);

		return true;
	}
}