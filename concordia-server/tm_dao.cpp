#include "tm_dao.hpp"

#include "query_param.hpp"
#include "string_param.hpp"
#include "int_param.hpp"
#include "bool_param.hpp"
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

std::vector<Tm> TmDAO::getTms() {
    std::vector<Tm> result;
    DBconnection connection;
    connection.startTransaction();
    std::string query = "select tm.id, tm.name, src_lang.code as src_code, trg_lang.code as trg_code from tm inner join language as src_lang on src_lang.id = tm.source_lang_id inner join language as trg_lang on trg_lang.id = tm.target_lang_id;";
    PGresult * dbResult = connection.execute(query);
    for (int i=0;i<connection.getRowCount(dbResult);i++) {
        int id = connection.getIntValue(dbResult, i, 0);
        std::string name = connection.getStringValue(dbResult, i, 1);
        std::string sourceLanguageCode = connection.getStringValue(dbResult, i, 2);
        std::string targetLanguageCode = connection.getStringValue(dbResult, i, 3);
        result.push_back(Tm(id, name, sourceLanguageCode, targetLanguageCode));
    }
    connection.clearResult(dbResult);
    connection.endTransaction();

    return result;

}





int TmDAO::addTm(const int sourceLangId, const int targetLangId, const std::string name) {
    addTm(sourceLangId, targetLangId, name, false);
}

int TmDAO::addTm(const int sourceLangId, const int targetLangId, const std::string name, bool lemmatized) {
    DBconnection connection;
    connection.startTransaction();

    std::string query = "INSERT INTO tm(source_lang_id, target_lang_id, name, lemmatized) values($1::integer,$2::integer,$3::text,$4::bool) RETURNING id";
    std::vector<QueryParam*> params;
    params.push_back(new IntParam(sourceLangId));
    params.push_back(new IntParam(targetLangId));
    params.push_back(new StringParam(name));
    params.push_back(new BoolParam(lemmatized));

    PGresult * result = connection.execute(query, params);
    int newId = connection.getIntValue(result, 0, 0);
    connection.clearResult(result);
    connection.endTransaction();
    BOOST_FOREACH (QueryParam * param, params) {
        delete param;
    }

    return newId;

}

std::pair<bool, std::string> TmDAO::getTmInfo(int tmId) {
    DBconnection connection;
    connection.startTransaction();
    std::string query = "select tm.id, tm.lemmatized, language.code from tm inner join language on language.id = tm.source_lang_id where tm.id = $1::integer;";
    std::vector<QueryParam*> params;
    params.push_back(new IntParam(tmId));
    PGresult * dbResult = connection.execute(query, params);
    bool lemmatized = connection.getBoolValue(dbResult, 0, 1);
    std::string languageCode = connection.getStringValue(dbResult, 0, 2);
    connection.clearResult(dbResult);
    connection.endTransaction();

    return std::pair<bool, std::string>(lemmatized, languageCode);

}
