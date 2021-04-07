#pragma once

#include "GBHttpLineReader.h"

namespace GenericBoson
{
	/*
	Read the first line of request.
	*/
	class GBHttpRequestLineReader : public GBHttpLineReader
	{
		const SOCKET& m_acceptedSocket;
	public:
		GBHttpRequestLineReader(const SOCKET& acceptedSocket) : m_acceptedSocket(acceptedSocket){}
		virtual ~GBHttpRequestLineReader() = default;

		bool Read(const std::string_view);
	};
}