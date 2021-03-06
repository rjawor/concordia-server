#include "db_connection.hpp"

#include <boost/foreach.hpp>
#include <sstream>
#include <stdlib.h>

#include "config.hpp"
#include "logger.hpp"

DBconnection::DBconnection() throw(ConcordiaException) {
    std::string connectionInfo = "dbname="DB_NAME" user="DB_USER" password="DB_PASSWORD" host="DB_HOST" port="DB_PORT;
    _connection = PQconnectdb(connectionInfo.c_str());
    if (PQstatus(_connection) != CONNECTION_OK) {
        close();
        std::stringstream ss;
        ss << "Could not establish connection with the database. ";
        ss << "Connection string: " << connectionInfo;
        throw ConcordiaException(ss.str());
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
        if (PQresultStatus(result) != PGRES_COMMAND_OK &&
            PQresultStatus(result) != PGRES_TUPLES_OK) {

            std::stringstream ss;
            ss << "query execution failed with message: ";
            ss << PQresultErrorMessage(result) << std::endl;
            PQclear(result);
            close();
            throw ConcordiaException(ss.str());
        } else {
            return result;
        }
    } else {
        throw ConcordiaException("requested query execution but the database connection is not ready");
    }
}

PGresult * DBconnection::execute(std::string query,
                                 std::vector<QueryParam*> params) throw(ConcordiaException) {
    if (_connection != NULL) {
        const char * paramValues[params.size()];
        int paramLengths[params.size()];
        int paramFormats[params.size()];
        int index = 0;
        BOOST_FOREACH (QueryParam * param, params) {
            paramValues[index] = param->getValue();
            paramLengths[index] = param->getLength();
            paramFormats[index] = param->isBinary();
            index++;
        }


        PGresult * result = PQexecParams(_connection,
                                         query.c_str(),
                                         params.size(),
                                         NULL,
                                         paramValues,
                                         paramLengths,
                                         paramFormats,
                                         0
                            );
        if (PQresultStatus(result) != PGRES_COMMAND_OK &&
            PQresultStatus(result) != PGRES_TUPLES_OK) {

            std::stringstream ss;
            ss << "parametrized query execution failed with message: ";
            ss << PQresultErrorMessage(result) << std::endl;
            PQclear(result);
            close();
            throw ConcordiaException(ss.str());
        } else {
            return result;
        }
    } else {
        throw ConcordiaException("requested query execution but the database connection is not ready");
    }
}

void DBconnection::clearResult(PGresult * result) {
    PQclear(result);
}

int DBconnection::getIntValue(PGresult * result, int row, int col) throw (ConcordiaException) {
    try {
        char * valueStr = PQgetvalue(result,row,col);
        return strtol(valueStr, NULL, 10);
    } catch (std::exception & e) {
        std::stringstream ss;
        ss << "Error getting int value. Message: " << e.what();
        throw ConcordiaException(ss.str());
    }
}

bool DBconnection::getBoolValue(PGresult * result, int row, int col) throw (ConcordiaException) {
    try {
        char * valueStr = PQgetvalue(result,row,col);
        return std::string(valueStr) == "t";
    } catch (std::exception & e) {
        std::stringstream ss;
        ss << "Error getting bool value. Message: " << e.what();
        throw ConcordiaException(ss.str());
    }
}

std::string DBconnection::getStringValue(PGresult * result, int row, int col)  throw (ConcordiaException) {
    try {
        char * valueStr = PQgetvalue(result,row,col);
        return std::string(valueStr);
    } catch (std::exception & e) {
        std::stringstream ss;
        ss << "Error getting string value. Message: " << e.what();
        throw ConcordiaException(ss.str());
    }
}

int DBconnection::getRowCount(PGresult * result) throw (ConcordiaException) {
    try {
        return PQntuples(result);
    } catch (std::exception & e) {
        std::stringstream ss;
        ss << "Error getting int value. Message: " << e.what();
        throw ConcordiaException(ss.str());
    }
}
