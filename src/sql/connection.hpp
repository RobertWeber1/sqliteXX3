#pragma once
#include <sqlite3.h>
#include <exception>

namespace sql
{


struct Connection
{
private:
	friend struct Statement;
	sqlite3 *db;

public:
	Connection(std::string const& dbfile)
	{
		if(sqlite3_open(dbfile.c_str(), &db) != SQLITE_OK)
		{
			throw(std::runtime_error(sqlite3_errmsg(db)));
		}
	}

	~Connection()
	{
		sqlite3_close(db);
	}
};


} //namespace sql
