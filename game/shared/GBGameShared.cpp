#include "../../core/Shared/GBShared.h"
#include "GBGameShared.h"

namespace GenericBoson
{
	bool GBGameShared::OnGatheringCompleted(VectoredIO& inputData, const BUFFER_SIZE_TYPE messageID)
	{
		const auto pStub = m_stubs.find(messageID);
		if (pStub == m_stubs.end())
		{
			ErrorLog(std::format("receive packet handler not found. - messageID : {}", messageID));
			return false;
		}

		pStub->second->CallStub(inputData.m_buffer);
	}
}