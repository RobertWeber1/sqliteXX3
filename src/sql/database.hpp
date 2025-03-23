#pragma once
#include <sql/connection.hpp>
#include <sql/prepared_statement.hpp>
#include <string>

namespace sql
{

struct DataBase
{
protected:
	std::shared_ptr<Connection> connection;

public:
	DataBase(std::string const& dbfile)
	: connection(std::make_shared<Connection>(dbfile))
	{}

	PreparedStatement prepare_statement(std::string const& query)
	{
		return PreparedStatement(connection, query);
	}

	InsertStatement insert_into(std::string const& query)
	{
		return InsertStatement(connection, query);
	}

	SelectStatement select_from(std::string const& query)
	{
		return SelectStatement(connection, query);
	}

	template<class ... Args>
	void execute(std::string const& query, Args && ... args)
	{
		PreparedStatement(connection, query, std::forward<Args>(args) ...).run();
	}
	// template<typename ...ARGS>
	// vector<tuple<ARGS...>> bindnquery(const string& query,
	// 		vector<string>&& bindvals={}){
	// 	PreparedStatement ppsm(db, query);
	// 	ppsm.bindvals(std::forward<decltype(bindvals)>(bindvals));
	// 	vector<tuple<ARGS...> > answer {};

	// 	using Result = tuple<ARGS ...>;

	// 	while (sqlite3_step(ppsm.me) == SQLITE_ROW){
	// 		answer.push_back(
	// 			get_row_(
	// 				ppsm.me,
	// 				std::make_index_sequence<sizeof...(ARGS)>{},
	// 				Result{}));
	// 	}
	// 	return answer;
	// }

	// void query_nothing(const string& query, vector<string>&& bindvals={}){
	// 	PreparedStatement ppsm(db,query);
	// 	ppsm.bindvals(std::forward<decltype(bindvals)>(bindvals));
	// 	auto result = sqlite3_step(ppsm.me);
	// 	if(result ==SQLITE_BUSY){
	// 		do {
	// 			sched_yield();
	// 			result = sqlite3_step(ppsm.me);;
	// 		} while (result == SQLITE_BUSY);
	// 	}
	// }


	// template<typename QType>
	// QType query_one(const string & query, vector<string>&& bindvals={}){
	// 	PreparedStatement ppsm(db, query);
	// 	ppsm.bindvals(std::forward<decltype(bindvals)>(bindvals));
	// 	if(sqlite3_step(ppsm.me) != SQLITE_ROW){
	// 		throw(std::runtime_error("Query did not yield answer:"+query));
	// 	}
	// 	return get_single<QType>(ppsm.me,0);
	// }
};



} //namespace sql
