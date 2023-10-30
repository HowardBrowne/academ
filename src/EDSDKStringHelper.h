#pragma once

#define __MACOS__
#include "EDSDK.h"
#include "EDSDKErrors.h"
#include "EDSDKTypes.h"

#include <string>

std::string getEdsErrorString(EdsError error);
std::string getEdsPropertyString(EdsPropertyID property);