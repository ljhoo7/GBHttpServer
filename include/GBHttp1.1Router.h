#pragma once

#include "GBHttp1.0Router.h"

namespace GenericBoson
{
	class GBHttp11Router : public GBHttp10Router
	{
	public:
		GBHttp11Router(const SOCKET& acceptedSocket) : GBHttp10Router(acceptedSocket) {}
		virtual ~GBHttp11Router() = default;

		virtual bool Route(const GBStringView subStr) override;
	};
}