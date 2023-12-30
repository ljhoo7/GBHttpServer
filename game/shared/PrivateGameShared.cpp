#include "../../core/Shared/PrivateShared.h"
#include "PrivateGameShared.h"

namespace GenericBoson
{
	bool PrivateGameShared::OnGatheringCompleted(VectoredIO& inputData)
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