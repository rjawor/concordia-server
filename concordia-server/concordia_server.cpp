#include "concordia_server.hpp"

#include <sstream>
#include <string>

#include "json_generator.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "rapidjson/rapidjson.h"

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
            } else if (operation == ADD_SENTENCES_OP) {
                std::vector<std::string> sourceSentences;
                std::vector<std::string> targetSentences;
                std::vector<int> tmIds;
                // loading data from json
                const rapidjson::Value & sentencesArray = d[SENTENCES_PARAM];
                for (rapidjson::SizeType i = 0; i < sentencesArray.Size(); i++) {
                    if (sentencesArray[i].Size() != 3) {
                        JsonGenerator::signalError(jsonWriter, "sentence should be an array of 3 elements");
                        break;
                    } else {
                        tmIds.push_back(sentencesArray[i][0].GetInt());
                        sourceSentences.push_back(sentencesArray[i][1].GetString());
                        targetSentences.push_back(sentencesArray[i][2].GetString());
                    }
                }
                _indexController->addSentences(jsonWriter, sourceSentences, targetSentences, tmIds);
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
