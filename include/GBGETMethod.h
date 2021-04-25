#pragma once

#include "../include/GBMethod.h"

namespace GenericBoson
{
	struct GBMethodGET : public GBMethod
	{
		~GBMethodGET() = default;

		virtual std::string GetName()
		{
			return "GET";
		}
	};
}