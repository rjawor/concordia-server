#include "concordia_server.hpp"

#include <sstream>
#include <string>

#include "json_generator.hpp"

#define OPERATION_PARAM "operation"
#define SENTENCE_PARAM "sentence"

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

string ConcordiaServer::handleRequest(string & requestString) {
    rapidjson::StringBuffer outputJson;
    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(outputJson);

    stringstream outputString;

    try {
        outputString << "Content-type: application/json\r\n\r\n";
        rapidjson::Document d;
        bool hasError = d.Parse(requestString.c_str()).HasParseError();

        if (hasError) {
            stringstream errorstream;
            errorstream << "json parse error at offset: " << d.GetErrorOffset() <<
                           ", description: " <<  GetParseError_En(d.GetParseError());
            JsonGenerator::signalError(jsonWriter, errorstream.str());
        } else { // json parsed
            string operation = d[OPERATION_PARAM].GetString();
            string sentence = d[SENTENCE_PARAM].GetString();
            if (operation == ADD_SENTENCE_OP) {
                _indexController->addSentence(jsonWriter, sentence);
            } else if (operation == SIMPLE_SEARCH_OP) {
                _searcherController->simpleSearch(jsonWriter, sentence);
            } else if (operation == CONCORDIA_SEARCH_OP) {
                _searcherController->concordiaSearch(jsonWriter, sentence);         
            } else {
                JsonGenerator::signalError(jsonWriter, "no such operation");            
            }
        }
             
    } catch (ConcordiaException & e) {
        stringstream errorstream;
        errorstream << "concordia error: " << e.what();
        JsonGenerator::signalError(jsonWriter, errorstream.str());        
    }

    outputString << outputJson.GetString();

    return outputString.str();

}
