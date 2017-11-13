#include "sort.h"

#include <string>
#include <iterator>
#include <set>
#include <algorithm>
#include <cctype>
#include <sstream>

bool Operators::Sensitive::operator()(const std::string & a, const std::string & b) const
{
#if 0
	for (const char * ptrA = a.c_str(), *ptrB = b.c_str(); ; ++ptrA, ++ptrB)
	{
		if (*ptrA != *ptrB || !*ptrA || !*ptrB)
			return *ptrA < *ptrB;
	}

	return false;
#endif
	return a < b;
}

bool Operators::NoCase::operator()(const std::string & a, const std::string & b) const
{
#if 0
	for (const char * ptrA = a.c_str(), *ptrB = b.c_str(); ; ++ptrA, ++ptrB)
	{
		if (std::tolower(*ptrA) != std::tolower(*ptrB) || !*ptrA || !*ptrB)
			return std::tolower(*ptrA) < std::tolower(*ptrB);
	}

	return false;
#endif
	std::string _a;
	std::transform(a.begin(), a.end(), std::back_inserter(_a), std::tolower);

	std::string _b;
	std::transform(b.begin(), b.end(), std::back_inserter(_b), std::tolower);

	return _a < _b;
}

namespace
{
	struct Line : public std::string {};

	std::istream & operator >> (std::istream & is, Line & line)
	{
		return std::getline(is, line);
	}

	struct WhiteSpace
	{
		bool operator()(const std::string & str) const
		{
			for (auto & c : str)
			{
				if (!std::isspace(c))
					return true;
			}
			return false;
		}
	};

	template<typename Pred>
	struct Reverse
	{
		bool operator()(const std::string & a, const std::string & b) const { return Pred()(b, a); }
	};

	template<typename Cont>
	void Output(Filter filter, std::istream & input, std::ostream & output)
	{
		Cont lines { std::istream_iterator<Line>(input), std::istream_iterator<Line>() };

		if ((filter & Filter::nowhite) == Filter::nowhite)
			std::copy_if(lines.begin(), lines.end(), std::ostream_iterator<std::string>(output, "\n"), WhiteSpace());
		else
			std::copy(lines.begin(), lines.end(), std::ostream_iterator<std::string>(output, "\n"));
	}

	template<typename Pred>
	void ApplyFilter(Filter filter, std::istream & input, std::ostream & output)
	{
		if ((filter & Filter::unique) == Filter::unique)
			Output<std::set<std::string, Pred>>(filter, input, output);
		else
			Output<std::multiset<std::string, Pred>>(filter,input, output);
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
		ApplyOrder<Operators::Sensitive>(order, filter, input, output);
	else
		ApplyOrder<Operators::NoCase>(order, filter, input, output);

	return true;
}


