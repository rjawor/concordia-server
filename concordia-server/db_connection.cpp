#include "db_connection.hpp"

#include "config.hpp"

DBconnection::DBconnection() throw(ConcordiaException) {
    std::string connectionInfo = "dbname="DB_NAME" user="DB_USER;
    _connection = PQconnectdb(connectionInfo.c_str());
    if (PQstatus(_connection) != CONNECTION_OK) {
        close();
        throw ConcordiaException("Could not establish connection with the database");
    }
    
}

DBconnection::~DBconnection() {
    close();
}

void DBconnection::close() {
    if (_connection != NULL) {
        PQfinish(_connection);
        _connection = NULL;
    }
}

void DBconnection::startTransaction() throw(ConcordiaException) {
    if (_connection != NULL) {
        PGresult * result = PQexec(_connection, "BEGIN");
        if (PQresultStatus(result) != PGRES_COMMAND_OK) {
            PQclear(result);
            close();
            throw ConcordiaException("starting transaction failed");
        }
    } else {
        throw ConcordiaException("requested start transaction but the database connection is not ready");
    }
}

void DBconnection::endTransaction() throw(ConcordiaException) {
    if (_connection != NULL) {
        PGresult * result = PQexec(_connection, "END");
        if (PQresultStatus(result) != PGRES_COMMAND_OK) {
            PQclear(result);
            close();
            throw ConcordiaException("ending transaction failed");
        }
    } else {
        throw ConcordiaException("requested end transaction but the database connection is not ready");
    }
}

PGresult * DBconnection::execute(std::string query) throw(ConcordiaException) {
    if (_connection != NULL) {
        PGresult * result = PQexec(_connection, query.c_str());
        if (PQresultStatus(result) != PGRES_COMMAND_OK) {
            PQclear(result);
            close();
            throw ConcordiaException("ending transaction failed");
        } else {
            return result;
        }
    } else {
        throw ConcordiaException("requested query execution but the database connection is not ready");
    }
}

PGresult * DBconnection::execute(std::string query,
                                 std::vector<std::string> params) throw(ConcordiaException) {
}


