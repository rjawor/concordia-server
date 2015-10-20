#include "tm_dao.hpp"

#include "query_param.hpp"
#include "string_param.hpp"
#include "int_param.hpp"
#include "int_array_param.hpp"
#include "logger.hpp"

#include <boost/foreach.hpp>
#include <libpq-fe.h>

TmDAO::TmDAO() {
}

TmDAO::~TmDAO() {
}

std::vector<int> TmDAO::getTmIds() {
    std::vector<int> result;
    DBconnection connection;
    connection.startTransaction();
    std::string query = "SELECT id FROM tm;";
    PGresult * dbResult = connection.execute(query);
    for (int i=0;i<connection.getRowCount(dbResult);i++) {
        int tmId = connection.getIntValue(dbResult, i, 0);
        result.push_back(tmId);
    }
    connection.clearResult(dbResult);
    connection.endTransaction();
    
    return result;
}

int TmDAO::addTm(const int sourceLangId, const int targetLangId, const std::string name) {
    DBconnection connection;
    connection.startTransaction();

    std::string query = "INSERT INTO tm(source_lang_id, target_lang_id, name) values($1::integer,$2::integer,$3::text) RETURNING id";
    std::vector<QueryParam*> params;
    params.push_back(new IntParam(sourceLangId));
    params.push_back(new IntParam(targetLangId));
    params.push_back(new StringParam(name));
    
    PGresult * result = connection.execute(query, params);
    int newId = connection.getIntValue(result, 0, 0);
    connection.clearResult(result);
    connection.endTransaction();
    BOOST_FOREACH (QueryParam * param, params) {
        delete param;
    }
    
    return newId;

}

