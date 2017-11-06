#include "sort.h"

#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <functional>

namespace
{
	struct Line : public std::string {};

	std::istream & operator >> (std::istream & is, Line & line)
	{
		return std::getline(is, line);
	}

	struct NoCase
	{
		bool operator()(const std::string & a, const std::string & b) const
		{
			return _stricmp(a.c_str(), b.c_str()) < 0;
		}
	};

	struct Reverse
	{
		template<typename Pred>
		Reverse(Pred pred) : _predicate(pred) {}

		bool operator()(const std::string & a, const std::string & b) const
		{
			return _predicate(b, a);
		}
	private:
		std::function<bool(const std::string &, const std::string &)> _predicate;
	};
}

bool sort::process(Order order, Filter filter, Case compare, std::istream & input, std::ostream & output)
{
	std::vector<std::string> lines { std::istream_iterator<Line>(input), std::istream_iterator<Line>() };

	// implementation of sorting
	std::function<bool(const std::string &, const std::string &)> fnOrder;
	
	if (compare == Case::ignore)
		fnOrder = NoCase();
	else
		fnOrder = std::less<std::string>();

	std::sort(lines.begin(), lines.end(), order == Order::ascending ? fnOrder : Reverse(fnOrder));

	if (filter == Filter::unique)
		lines.erase(std::unique(lines.begin(), lines.end(), [fnOrder](const std::string & a, const std::string & b) { return !fnOrder(a, b) && !fnOrder(b, a); }), lines.end());

	std::copy(lines.begin(), lines.end(), std::ostream_iterator<std::string>(output, "\n"));

	return true;
}


