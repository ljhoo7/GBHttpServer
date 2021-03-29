#pragma once

#include <vector>

namespace GenericBoson
{
	class GBMethod;

	class GBRouter
	{
		const SOCKET& m_acceptedSocket;
	public:
		std::vector<GBMethod> m_methodList;

		GBRouter(const SOCKET& accpetedSocket) : m_acceptedSocket(accpetedSocket) {}
		bool Route(const std::string_view subStr);
	};
}