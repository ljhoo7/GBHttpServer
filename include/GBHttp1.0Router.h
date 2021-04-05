#pragma once

#include "GBHttp0.9Router.h"

namespace GenericBoson
{
	class GBHttp10Router : public GBHttp09Router
	{
	public:
		GBHttp10Router() = default;
		virtual ~GBHttp10Router() = default;

		virtual bool Route(const GBStringView subStr) override;
	};
}