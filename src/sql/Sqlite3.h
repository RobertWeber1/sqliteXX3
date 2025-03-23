#pragma once
#include <sqlite3.h>
#include <string>
#include <memory>
// #include <tuple>
// #include <vector>
// #include <exception>
// #include <cstdint>


namespace sql
{

// using std::string;
// using std::vector;
// using std::tuple;




// template <typename T>
// T get_single(sqlite3_stmt* ppsm, int iCol);

// template <> inline
// int get_single(sqlite3_stmt* ppsm, int iCol)
// {
// 	return sqlite3_column_int(ppsm, iCol);
// }

// template <> inline
// double get_single(sqlite3_stmt* ppsm, int iCol)
// {
// 		return sqlite3_column_double(ppsm, iCol);
// }

// template <> inline
// std::string get_single(sqlite3_stmt *ppsm, int iCol)
// {
// 	const char * foo = reinterpret_cast<const char*>(sqlite3_column_text(ppsm,iCol));
// 	return(foo?string(foo):"");
// }

// template <> inline
// std::int64_t get_single(sqlite3_stmt *ppsm, int iCol)
// {
// 	return sqlite3_column_int64(ppsm, iCol);
// }


// template<size_t ... I, typename ...ARGS>
// inline std::tuple<ARGS...>  get_row_(
// 	sqlite3_stmt* ppsm,
// 	std::index_sequence<I...>,
// 	tuple<ARGS...>)
// {
// 	return std::make_tuple(get_single<ARGS>(ppsm, I)...);
// }


} //namespace sql
