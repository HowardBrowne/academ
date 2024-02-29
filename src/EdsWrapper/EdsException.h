#pragma once

#include "EdsWrapper.h"
#include <string>
#include <typeinfo>

namespace Eds {
	class Exception : public std::exception {
	public:
		Exception(EdsError err, const std::string& function) throw();
		virtual ~Exception() throw();
		virtual const char* what() const throw();
		bool operator!=(EdsError err) const;
		bool operator==(EdsError err) const;
	private:
		EdsError err;
		std::string function;
		std::string errString;
	};
}