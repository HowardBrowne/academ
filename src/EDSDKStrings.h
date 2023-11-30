#pragma once

#include "EdsdkWrapper.h"
#include <string>

namespace Edsdk {
	
	std::string getErrorString(EdsError error);
	std::string getPropertyString(EdsPropertyID property);
	
}