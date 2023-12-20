#include "EdsException.h"

namespace Eds {
	Exception::Exception(const EdsError& err, const std::string& function) throw() :
	err(err),
	function(function) {
		errString = getErrorString(err) + " inside " + function + "()";
	}
	Exception::~Exception() throw() {
	}
	const char* Exception::what() const throw() {
		return errString.c_str();
	}
}