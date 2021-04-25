#pragma once

#include "../include/GBMethod.h"

namespace GenericBoson
{
	struct GBMethodHEAD : public GBMethod
	{
		~GBMethodHEAD() = default;

		virtual std::string GetName()
		{
			return "HEAD";
		}
	};
}