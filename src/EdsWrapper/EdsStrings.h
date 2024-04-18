#pragma once

#include "EdsWrapper.h"
#include <string>

namespace Eds {
	std::string getErrorString(EdsError error);
	std::string getPropertyIDString(EdsPropertyID property);
	std::string getPropertyEventString(EdsPropertyEvent event);
	std::string getObjectEventString(EdsObjectEvent event);
	std::string getStateEventString(EdsStateEvent event);
}