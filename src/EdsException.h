#pragma once

#include <typeinfo>
#include <string>

#include "EdsWrapper.h"

/*
	even more useful would be to go through the different error codes and make a class for each,
	or at least for subsets. this would take a little longer.
*/

namespace Eds {
	class Exception : public std::exception {
	public:
		Exception(const EdsError& err, const std::string& function) throw();
		virtual ~Exception() throw();
		virtual const char* what() const throw();
	private:
		EdsError err;
		std::string function;
		std::string errString;
	};
}