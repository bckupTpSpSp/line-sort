#include "sort.h"

#include <string>
#include <iterator>
#include <set>
#include <algorithm>
#include <cctype>
#include <sstream>


namespace
{
	struct Line : public std::string {};

	std::istream & operator >> (std::istream & is, Line & line)
	{
		return std::getline(is, line);
	}

	struct Sensitive
	{
		bool operator()(const std::string & a, const std::string & b) const { return a < b; }
	};

	struct NoCase
	{
		bool operator()(const std::string & a, const std::string & b) const { return _stricmp(a.c_str(), b.c_str()) < 0; }
	};

	template<typename Pred>
	struct Reverse
	{
		bool operator()(const std::string & a, const std::string & b) const { return Pred()(b, a); }
	};

	template<typename Cont>
	void Output(std::istream & input, std::ostream & output)
	{
		Cont lines { std::istream_iterator<Line>(input), std::istream_iterator<Line>() };

		std::copy(lines.begin(), lines.end(), std::ostream_iterator<std::string>(output, "\n"));
	}

	template<typename Pred>
	void ApplyFilter(Filter filter, std::istream & input, std::ostream & output)
	{
		if (filter == Filter::unique)
			Output<std::set<std::string, Pred>>(input, output);
		else
			Output<std::multiset<std::string, Pred>>(input, output);
	}

	template<typename Pred> 
	void ApplyOrder(Order order, Filter filter, std::istream & input, std::ostream & output)
	{
		if (order == Order::descending)
			ApplyFilter<Reverse<Pred>>(filter, input, output);
		else
			ApplyFilter<Pred>(filter, input, output);
	}
}

bool sort::process(Order order, Filter filter, Case compare, std::istream & input, std::ostream & output)
{
	if (compare == Case::sensitive)
		ApplyOrder<Sensitive>(order, filter, input, output);
	else
		ApplyOrder<NoCase>(order, filter, input, output);

	return true;
}


