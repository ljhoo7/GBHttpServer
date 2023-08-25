#include "stdafx.h"
#include "GBExpandedOverlapped.h"

namespace GenericBoson
{
	void GBExpandedOverlapped::SetIndex(const int index)
	{
		m_index = index;
	}

	int GBExpandedOverlapped::GetIndex()
	{
		return m_index;
	}

	bool GBExpandedOverlapped::IsForSend() const
	{
		return m_index % 2;
	}
}