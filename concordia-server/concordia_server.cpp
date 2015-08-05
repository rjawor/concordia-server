#include "concordia_server.hpp"

#include <sstream>
#include <string>

#include "json_generator.hpp"

#define OPERATION_PARAM "operation"
#define PATTERN_PARAM "pattern"
#define SOURCE_SENTENCE_PARAM "sourceSentence"
#define TARGET_SENTENCE_PARAM "targetSentence"
#define TM_ID_PARAM "tmId"

#define ADD_SENTENCE_OP "addSentence"
#define SIMPLE_SEARCH_OP "simpleSearch"
#define CONCORDIA_SEARCH_OP "concordiaSearch"


ConcordiaServer::ConcordiaServer(const std::string & configFilePath)
                                         throw(ConcordiaException) {
    boost::shared_ptr<Concordia> concordia(new Concordia(configFilePath));
    _indexController = boost::shared_ptr<IndexController> (new IndexController(concordia));
    _searcherController = boost::shared_ptr<SearcherController> (new SearcherController(concordia));
}

ConcordiaServer::~ConcordiaServer() {
}

std::string ConcordiaServer::handleRequest(std::string & requestString) {
    rapidjson::StringBuffer outputJson;
    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(outputJson);

    std::stringstream outputString;
    try {
        outputString << "Content-type: application/json\r\n\r\n";
        rapidjson::Document d;
        bool hasError = d.Parse(requestString.c_str()).HasParseError();

        if (hasError) {
            std::stringstream errorstream;
            errorstream << "json parse error at offset: " << d.GetErrorOffset() <<
                           ", description: " <<  GetParseError_En(d.GetParseError());
            JsonGenerator::signalError(jsonWriter, errorstream.str());
        } else { // json parsed
            std::string operation = d[OPERATION_PARAM].GetString();
            if (operation == ADD_SENTENCE_OP) {
                std::string sourceSentence = d[SOURCE_SENTENCE_PARAM].GetString();
                std::string targetSentence = d[TARGET_SENTENCE_PARAM].GetString();
                int tmId = d[TM_ID_PARAM].GetInt();
                _indexController->addSentence(jsonWriter, sourceSentence, targetSentence, tmId);
            } else if (operation == SIMPLE_SEARCH_OP) {
                std::string pattern = d[PATTERN_PARAM].GetString();
                _searcherController->simpleSearch(jsonWriter, pattern);
            } else if (operation == CONCORDIA_SEARCH_OP) {
                std::string pattern = d[PATTERN_PARAM].GetString();
                _searcherController->concordiaSearch(jsonWriter, pattern);         
            } else {
                JsonGenerator::signalError(jsonWriter, "no such operation");            
            }
        }
             
    } catch (ConcordiaException & e) {
        std::stringstream errorstream;
        errorstream << "concordia error: " << e.what();
        JsonGenerator::signalError(jsonWriter, errorstream.str());        
    }

    outputString << outputJson.GetString();

    return outputString.str();

}
