#include "EdsException.h"

namespace Eds {
	Exception::Exception(const EdsError& err) throw() : err(err), errString(getErrorString(err)) {
	}
	Exception::~Exception() throw() {
	}
	const char* Exception::what() const throw() {
		return errString.c_str();
	}
}