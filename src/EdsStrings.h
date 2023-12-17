#pragma once

#include "EdsWrapper.h"
#include <string>

namespace Eds {
	
	std::string getErrorString(EdsError error);
	std::string getPropertyString(EdsPropertyID property);
	
}