#pragma once

enum class Order { ascending, descending };
enum class Filter { all = 0, unique = 1, nowhite = 2 };
enum class Case { sensitive, ignore };

inline Filter operator|(Filter a, Filter b) { return static_cast<Filter>(static_cast<int>(a) | static_cast<int>(b)); }
inline Filter operator&(Filter a, Filter b) { return static_cast<Filter>(static_cast<int>(a) & static_cast<int>(b)); }
