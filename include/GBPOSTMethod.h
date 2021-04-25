#pragma once

#include "../include/GBMethod.h"

namespace GenericBoson
{
	struct GBMethodPOST : public GBMethod
	{
		~GBMethodPOST() = default;

		virtual std::string GetName()
		{
			return "POST";
		}
	};
}