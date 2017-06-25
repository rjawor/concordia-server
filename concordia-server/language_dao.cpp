#include "language_dao.hpp"

#include "query_param.hpp"
#include "string_param.hpp"
#include "int_param.hpp"
#include "bool_param.hpp"
#include "int_array_param.hpp"
#include "logger.hpp"

#include <boost/foreach.hpp>
#include <libpq-fe.h>

LanguageDAO::LanguageDAO() {
}

LanguageDAO::~LanguageDAO() {
}

std::vector<Language> LanguageDAO::getLanguages() {
    std::vector<Language> result;
    DBconnection connection;
    connection.startTransaction();
    std::string query = "select * from language order by name;";
    PGresult * dbResult = connection.execute(query);
    for (int i=0;i<connection.getRowCount(dbResult);i++) {
        int id = connection.getIntValue(dbResult, i, 0);
        std::string code = connection.getStringValue(dbResult, i, 1);
        std::string name = connection.getStringValue(dbResult, i, 2);

        result.push_back(Language(id, code, name));
    }
    connection.clearResult(dbResult);
    connection.endTransaction();

    return result;

}
