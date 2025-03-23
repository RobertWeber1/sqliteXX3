#pragma once
#include <sqlite3.h>
#include <memory>
#include <string>


namespace sql
{

struct StaticString
{
	char const* data;
	size_t size;
};

struct TransientString
{
	char const* data;
	size_t size;
};

////////////////////////////////////////////////////////////////////////////////

int32_t to_base_type(bool value)
{
	return static_cast<int32_t>(value);
}

int32_t to_base_type(uint8_t value)
{
	return static_cast<int32_t>(value);
}

int32_t to_base_type(uint16_t value)
{
	return static_cast<int32_t>(value);
}

int32_t to_base_type(uint32_t value)
{
	return static_cast<int32_t>(value);
}

int64_t to_base_type(uint64_t value)
{
	return static_cast<int64_t>(value);
}

int32_t to_base_type(int8_t value)
{
	return static_cast<int32_t>(value);
}

int32_t to_base_type(int16_t value)
{
	return static_cast<int32_t>(value);
}

int32_t to_base_type(int32_t value)
{
	return value;
}

int64_t to_base_type(int64_t value)
{
	return value;
}

double to_base_type(float value)
{
	return static_cast<double>(value);
}

double to_base_type(double value)
{
	return value;
}

StaticString to_base_type(StaticString value)
{
	return value;
}

template<size_t N>
StaticString to_base_type(char const (&value)[N])
{
	return StaticString{value, N-1};
}

TransientString to_base_type(TransientString value)
{
	return value;
}

TransientString to_base_type(std::string const& value)
{
	return TransientString{value.data(), value.size()};
}

TransientString to_base_type(std::string_view value)
{
	return TransientString{value.data(), value.size()};
}

////////////////////////////////////////////////////////////////////////////////




void bind_base_type(sqlite3_stmt * stmt, size_t position, int32_t value)
{
	if(sqlite3_bind_int(stmt, position, value) != SQLITE_OK)
	{
		throw(std::runtime_error(sqlite3_errmsg(sqlite3_db_handle(stmt))));
	}
}

void bind_base_type(sqlite3_stmt * stmt, size_t position, int64_t value)
{
	if(sqlite3_bind_int64(stmt, position, value) != SQLITE_OK)
	{
		throw(std::runtime_error(sqlite3_errmsg(sqlite3_db_handle(stmt))));
	}
}

void bind_base_type(sqlite3_stmt * stmt, size_t position, double value)
{
	if(sqlite3_bind_double(stmt, position, value) != SQLITE_OK)
	{
		throw(std::runtime_error(sqlite3_errmsg(sqlite3_db_handle(stmt))));
	}
}

void bind_base_type(sqlite3_stmt * stmt, size_t position, StaticString const& value)
{
	if(sqlite3_bind_text(stmt, position, value.data, value.size, SQLITE_STATIC)
		!= SQLITE_OK)
	{
		throw(std::runtime_error(sqlite3_errmsg(sqlite3_db_handle(stmt))));
	}
}

void bind_base_type(sqlite3_stmt * stmt, size_t position, TransientString const& value)
{
	if(sqlite3_bind_text(stmt, position, value.data, value.size, SQLITE_TRANSIENT)
		!= SQLITE_OK)
	{
		throw(std::runtime_error(sqlite3_errmsg(sqlite3_db_handle(stmt))));
	}
}

template<size_t ... Is, class ... Args>
void bind_pos_args(sqlite3_stmt * stmt, std::index_sequence<Is...>, Args && ... args)
{
	(bind_base_type(stmt, Is+1, to_base_type(std::forward<Args>(args))), ...);
}

template<class ... Args>
void bind_args(sqlite3_stmt * stmt, Args && ... args)
{
	bind_pos_args(
		stmt,
		std::make_index_sequence<sizeof...(Args)>{},
		std::forward<Args>(args)...);
}
////////////////////////////////////////////////////////////////////////////////

// struct Connection;

struct Statement
{
	Statement(
		std::shared_ptr<Connection> connection,
		std::string const& query)
	: connection_(std::move(connection))
	, stmt(nullptr)
	{
		if(sqlite3_prepare_v2(
			connection_.get()->db,
			query.c_str(),
			query.size(),
			&stmt,
			nullptr)!= SQLITE_OK)
		{
			throw(std::runtime_error(sqlite3_errmsg(connection_.get()->db)));
		}
	}

	~Statement()
	{
		sqlite3_finalize(stmt);
	}

	void reset()
	{
		sqlite3_reset(stmt);
	}

	int step()
	{
		return sqlite3_step(stmt);
	}

	char const* get_error_msg() const
	{
		return sqlite3_errmsg(sqlite3_db_handle(stmt));
	}
private:
	friend struct PreparedStatement;
	friend struct InsertStatement;
	std::shared_ptr<Connection> connection_;
	sqlite3_stmt * stmt;
};


struct PreparedStatement
{
public:
	template<class ... Args>
	PreparedStatement& bind(Args && ... args)
	{
		statement.reset();
		bind_args(statement.stmt, std::forward<Args>(args)...);
		return *this;
	}

	void run()
	{
		int ret = statement.step();
		if(ret != SQLITE_DONE and ret != SQLITE_ROW)
		{
			throw(std::runtime_error(statement.get_error_msg()));
		}
	}

	void reset()
	{
		statement.reset();
	}

private:
	PreparedStatement(
		std::shared_ptr<Connection> connection,
		std::string const& query)
	: statement(std::move(connection), query)
	{}



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

private:
	friend struct DataBase;
	Statement statement;
};


struct InsertStatement
{
public:
	template<class ... Args>
	InsertStatement& values(Args && ... args)
	{
		statement.reset();
		bind_args(statement.stmt, std::forward<Args>(args)...);

		int ret = statement.step();
		if(ret != SQLITE_DONE and ret != SQLITE_ROW)
		{
			throw(std::runtime_error(statement.get_error_msg()));
		}

		return *this;
	}

private:
	InsertStatement(
		std::shared_ptr<Connection> connection,
		std::string const& query)
	: statement(std::move(connection), query)
	{}

private:
	friend struct DataBase;
	Statement statement;
};


struct SelectStatement
{
public:
	// template<class ... Args>
	// InsertStatement& values(Args && ... args)
	// {
	// 	statement.reset();
	// 	bind_args(statement.stmt, std::forward<Args>(args)...);

	// 	int ret = statement.step();
	// 	if(ret != SQLITE_DONE and ret != SQLITE_ROW)
	// 	{
	// 		throw(std::runtime_error(statement.get_error_msg()));
	// 	}

	// 	return *this;
	// }


private:
	SelectStatement(
		std::shared_ptr<Connection> connection,
		std::string const& query)
	: statement(std::move(connection), query)
	{}

private:
	friend struct DataBase;
	Statement statement;
};


} //namespace sql
