#pragma once

#include <fstream>
#include <ostream>

#include "defs.h"

namespace Operators
{
	struct Sensitive
	{
		bool operator()(const std::string & a, const std::string & b) const;
	};

	struct NoCase
	{
		bool operator()(const std::string & a, const std::string & b) const;
	};
}

namespace sort
{
	bool process(Order order, Filter filter, Case compare, std::istream & input, std::ostream & output);
}

