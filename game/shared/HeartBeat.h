#pragma once

#include "../../core/Shared/Timer.h"

namespace GenericBoson
{
	class HeartBeat : public GBTimer
	{
		void OnTime() override;
	};
}