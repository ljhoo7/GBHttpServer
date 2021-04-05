#pragma once

#include "../include/GBHttpRouter.h"

namespace GenericBoson
{
	class GBHttp09Router : public GBHttpRouter
	{
	public:
		GBHttp09Router() = default;
		virtual ~GBHttp09Router() = default;

		virtual bool Route(const GBStringView subStr) override;
	};
}