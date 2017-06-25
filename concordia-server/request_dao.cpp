#include "request_dao.hpp"

#include "query_param.hpp"
#include "string_param.hpp"
#include "int_param.hpp"
#include "bool_param.hpp"
#include "int_array_param.hpp"
#include "logger.hpp"

#include <boost/foreach.hpp>
#include <libpq-fe.h>

RequestDAO::RequestDAO() {
}

RequestDAO::~RequestDAO() {
}

std::vector<Request> RequestDAO::getRequests() {
    std::vector<Request> result;
    DBconnection connection;
    connection.startTransaction();
    std::string query = "select request.id, request.source_file_path, request.target_file_path, request.name, src_lang.code as src_code, trg_lang.code as trg_code, request.status, request.type, request.tm_id, to_char(request.created,'YYYY-MM-DD HH24:MI:SS') from request inner join language as src_lang on src_lang.id = request.source_lang_id inner join language as trg_lang on trg_lang.id = request.target_lang_id order by request.created desc limit 20;";
    PGresult * dbResult = connection.execute(query);
    for (int i=0;i<connection.getRowCount(dbResult);i++) {
        int id = connection.getIntValue(dbResult, i, 0);
        std::string sourceFilePath = connection.getStringValue(dbResult, i, 1);
        std::string targetFilePath = connection.getStringValue(dbResult, i, 2);
        std::string name = connection.getStringValue(dbResult, i, 3);
        std::string sourceLanguageCode = connection.getStringValue(dbResult, i, 4);
        std::string targetLanguageCode = connection.getStringValue(dbResult, i, 5);
        int status = connection.getIntValue(dbResult, i, 6);
        int type = connection.getIntValue(dbResult, i, 7);
        int tmId = connection.getIntValue(dbResult, i, 8);
        std::string created = connection.getStringValue(dbResult, i, 9);

        result.push_back(Request(id, sourceFilePath, targetFilePath, name, sourceLanguageCode, targetLanguageCode, status, type, tmId, created));
    }
    connection.clearResult(dbResult);
    connection.endTransaction();

    return result;

}

int RequestDAO::addRequest(const std::string sourceFilePath, const std::string targetFilePath, const int sourceLangId, const int targetLangId, const std::string name, int type, int tmId) {
    Logger::log("addRequest");
    Logger::logString("source file path", sourceFilePath);
    Logger::logString("target file path", targetFilePath);
    Logger::logInt("source lang id", sourceLangId);
    Logger::logInt("target lang id", targetLangId);
    Logger::logString("name", name);
    Logger::logInt("type", type);
    Logger::logInt("tm id", tmId);

    DBconnection connection;
    connection.startTransaction();

    std::string query = "INSERT INTO request(source_file_path, target_file_path, source_lang_id, target_lang_id, name, status, type, tm_id) values($1::text,$2::text,$3::integer,$4::integer,$5::text, $6::integer, $7::integer, $8::integer) RETURNING id";
    std::vector<QueryParam*> params;
    params.push_back(new StringParam(sourceFilePath));
    params.push_back(new StringParam(targetFilePath));
    params.push_back(new IntParam(sourceLangId));
    params.push_back(new IntParam(targetLangId));
    params.push_back(new StringParam(name));
    params.push_back(new IntParam(0));
    params.push_back(new IntParam(type));
    params.push_back(new IntParam(tmId));

    PGresult * result = connection.execute(query, params);
    int newId = connection.getIntValue(result, 0, 0);
    connection.clearResult(result);
    connection.endTransaction();
    BOOST_FOREACH (QueryParam * param, params) {
        delete param;
    }

    return newId;

}
