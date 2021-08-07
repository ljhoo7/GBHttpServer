#pragma once

#include "GBHttpRequestLineReader.h"
#include "GBHttpHeaderReader.h"
#include "GBHttpMessageBody.h"

namespace GenericBoson
{
	class GBHttpRequestReader
	{
	public:
		GBHttpRequestReader(std::queue<std::string_view>& lines)
		{
			m_pRequestLineInformation = std::make_shared<GBHttpRequestLineInformation>();
			m_readerToInformationQueue.emplace(std::make_unique<GBHttpRequestLineReader>(lines), m_pRequestLineInformation);
			m_pHeaderInformation = std::make_shared<GBHttpHeaderInformation>();
			m_readerToInformationQueue.emplace(std::make_unique<GBHttpHeaderReader>(lines), m_pHeaderInformation);

			m_pMessageBody = std::make_shared<GBHttpMessageBody>();
		}

		std::shared_ptr<GBHttpRequestLineInformation> m_pRequestLineInformation = nullptr;
		std::shared_ptr<GBHttpHeaderInformation> m_pHeaderInformation = nullptr;
		std::shared_ptr<GBHttpMessageBody> m_pMessageBody = nullptr;

		std::queue<std::pair<std::unique_ptr<GBHttpReader>, std::shared_ptr<GBHttpInformation>>> m_readerToInformationQueue;

		HTTP_STATUS_CODE Read();
	};
}