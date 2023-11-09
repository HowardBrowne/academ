#pragma once

#include "EDSDKWrapper.h"
#include <string>

namespace EDSDK {
	
	std::string getErrorString(EdsError error);
	std::string getPropertyString(EdsPropertyID property);
	
}