#pragma once

namespace GenericBoson
{
	class GBRouter
	{
		std::map<char*, size_t> m_methodNameSizeMap;
		const std::vector<char*> m_methodList;
	public:
		// {"A", "B", ..., "Z"} 형태의 임시객체로만 초기화하게 한다.
		GBRouter(const std::vector<char*>&& rvalue)
			: m_methodList(rvalue)
		{
			for (auto iMethod : m_methodList)
			{
				m_methodNameSizeMap.emplace(iMethod, strlen(iMethod));
			}
		}

		std::string_view Route(const std::string_view subStr);
	};
}