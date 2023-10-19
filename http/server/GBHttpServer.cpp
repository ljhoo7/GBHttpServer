#include "GBHttpServer.h"

namespace GenericBoson
{
	//bool GBHttpServer::TraversePathTree(const std::vector<std::string_view>& pathTree, const std::map<std::string_view, std::string_view> queryMap, PathSegment*& pTargetPath)
	//{
	//	for (const auto& iPathSegment : pathTree)
	//	{
	//		// 원래 true == pTargetPath->m_subTreeMap.contains(iPathSegment) 였는데,
	//		// travis가 VS2017까지만 지원해서 아래와 같이 바꿈.
	//		if (pTargetPath->m_subTreeMap.end() == pTargetPath->m_subTreeMap.find(iPathSegment))
	//		{
	//			pTargetPath->m_subTreeMap.emplace(iPathSegment, std::make_unique<PathSegment>());
	//		}

	//		pTargetPath = (pTargetPath->m_subTreeMap[iPathSegment]).get();
	//	}

	//	if (nullptr != pTargetPath->m_pGetMethod)
	//	{
	//		// #ToDo
	//		// The action method Already Exists at the path.
	//		return false;
	//	}

	//	return true;
	//}

	bool GBServer::OnSent(GBExpandedOverlapped* pEol, DWORD sentBytes)
	{
		return true;
	}

	bool GBServer::OnReceived(VectoredIO& eol, DWORD receivedBytes)
	{
		bool succeeded = false;

		//bool parseResult = pEol->GatherAndParseLines(receivedBytes);
		pEol->m_receiveOffset += receivedBytes;

		bool gatheringNotFinished = false;
		bool gatheringFinishedButNothing = false;

		/*if (false == parseResult)
		{
			gatheringNotFinished = true;
		}
		else if (0 == pEol->m_receiveOffset)
		{
			gatheringFinishedButNothing = true;
		}*/

		// 개더링이 끝나지 않았거나, 끝났어도 받은게 전혀없다면, 더 받으려고 한다.
		// ... 이 부분 코드 core로 옮김.

		GBHttpRequestReader requestReader(pEol);
		HTTP_STATUS_CODE readResult = requestReader.Read();


#if defined(_DEBUG)
		// 통신 표시
		std::cout << pEol->m_pReceiveBuffer << '\n';
#endif

		GBHttpResponse response;
		{
			std::lock_guard<std::mutex> lock(g_mainCriticalsection);

			pEol->m_httpVersion = requestReader.m_pRequestLineInformation->m_version;

			//g_pRouter->m_methodList.emplace_back("GET", [](const std::string_view path)
			//{
			//	std::cout << "GET : path = " << path.data() << std::endl;
			//});
			//g_pRouter->m_methodList.emplace_back("PUT", [](const std::string_view path)
			//{
			//	std::cout << "PUT : path = " << path.data() << std::endl;
			//});
			//g_pRouter->m_methodList.emplace_back("POST", [](const std::string_view path)
			//{
			//	std::cout << "POST : path = " << path.data() << std::endl;
			//});

			//succeeded = g_pRouter->Route(g_rootPath, requestReader, response);

			if (false == succeeded)
			{
				// #ToDo 로깅으로 바꾸자
				//return { false, "Routing failed." };
			}
		}

		GBHttpResponseWriter responseWriter(pEol);
		responseWriter.WriteStatusLine(HttpVersion::Http10, response, "none");//requestReader.m_pRequestLineInformation->m_version

		std::vector<std::pair<std::string, std::string>> headerList;

		headerList.emplace_back("Host", "localhost:8000");
		headerList.emplace_back("Connection", "keep-alive");
		headerList.emplace_back("Accept", "*/*");

		//headerList.emplace_back("Content-type", "text/html");

		responseWriter.WriteHeader(headerList);

		responseWriter.WriteBody();

		return true;
	}

	//bool GBExpandedOverlapped::GatherAndParseLines(DWORD receivedBytes)
	//{
	//	int stringOffset = 0;
	//	// All Http message ( except for Entity-Body ) must be ended by CRLF or LF.
	//	int k = m_receiveOffset;
	//	char* pLineStart = nullptr;
	//	for (; k < m_receiveOffset + receivedBytes; ++k)
	//	{
	//		switch (m_pBuffer[k])
	//		{
	//		case '\r':
	//			// carrage return은 무시한다.
	//			// RFC 문서에 의하면 HTTP message의 request line과 header-value list들은 개항을 CRLF로 해야 된다고 나왔있다.
	//			// 그러나, 'HTTP 완벽 가이드'책에 의하면, 옛날 프로그램 중 CR 없이 LF만으로 개행하는 것들이 많다고 한다.
	//			break;
	//		case '\n':
	//			m_lines.emplace(pLineStart, stringOffset);
	//			stringOffset = 0;
	//			break;
	//		default:
	//			if (0 == stringOffset)
	//			{
	//				pLineStart = &m_pBuffer[k];
	//			}
	//			stringOffset++;
	//			break;
	//		}
	//	}

	//	return 0 == stringOffset;
	//}
}