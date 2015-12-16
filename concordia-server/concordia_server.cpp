#include "concordia_server.hpp"

#include <sstream>
#include <string>

#include "json_generator.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "rapidjson/rapidjson.h"
#include <boost/foreach.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/filesystem/path.hpp>

ConcordiaServer::ConcordiaServer(const std::string & configFilePath)
                                         throw(ConcordiaException) :
                                         _configFilePath(configFilePath) {
    std::vector<int> tmIds = _tmDAO.getTmIds();
    _concordiasMap = boost::shared_ptr<boost::ptr_map<int,Concordia> >(new boost::ptr_map<int,Concordia>());

    BOOST_FOREACH(int & tmId, tmIds) {
        _addTm(tmId);
    }
    _indexController = boost::shared_ptr<IndexController> (new IndexController(_concordiasMap));
    _searcherController = boost::shared_ptr<SearcherController> (new SearcherController(_concordiasMap));    
}

ConcordiaServer::~ConcordiaServer() {
}

std::string ConcordiaServer::handleRequest(std::string & requestString) {
    rapidjson::StringBuffer outputJson;
    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(outputJson);

    std::stringstream outputString;
    outputString << "Content-type: application/json\r\n\r\n";
    try {
        rapidjson::Document d;
        bool hasError = d.Parse(requestString.c_str()).HasParseError();

        if (hasError) {
            std::stringstream errorstream;
            errorstream << "json parse error at offset: " << d.GetErrorOffset() <<
                           ", description: " <<  GetParseError_En(d.GetParseError());
            JsonGenerator::signalError(jsonWriter, errorstream.str());
        } else { // json parsed
            std::string operation = _getStringParameter(d, OPERATION_PARAM);
            if (operation == ADD_SENTENCE_OP) {                    
                std::string sourceSentence = _getStringParameter(d, SOURCE_SENTENCE_PARAM);
                std::string targetSentence = _getStringParameter(d, TARGET_SENTENCE_PARAM);
                int tmId = _getIntParameter(d, TM_ID_PARAM);
                _indexController->addSentence(jsonWriter, sourceSentence, targetSentence, tmId);
            } else if (operation == ADD_SENTENCES_OP) {
                std::vector<std::string> sourceSentences;
                std::vector<std::string> targetSentences;
                int tmId = _getIntParameter(d, TM_ID_PARAM);
                // loading data from json
                const rapidjson::Value & sentencesArray = d[SENTENCES_PARAM];
                Logger::log("addSentences");
                Logger::logInt("sentences to add", sentencesArray.Size());
                Logger::logInt("tm id", tmId);
                for (rapidjson::SizeType i = 0; i < sentencesArray.Size(); i++) {
                    if (sentencesArray[i].Size() != 2) {
                        JsonGenerator::signalError(jsonWriter, "sentence should be an array of 2 elements");
                        break;
                    } else {
                        sourceSentences.push_back(sentencesArray[i][0].GetString());
                        targetSentences.push_back(sentencesArray[i][1].GetString());
                    }
                }
                _indexController->addSentences(jsonWriter, sourceSentences, targetSentences, tmId);
            } else if (operation == ADD_ALIGNED_SENTENCES_OP) {
                std::vector<std::string> sourceSentences;
                std::vector<std::string> targetSentences;
                int tmId = d[TM_ID_PARAM].GetInt();
                // loading data from json
                const rapidjson::Value & sentencesArray = d[SENTENCES_PARAM];
                Logger::log("addAlignedSentences");
                Logger::logInt("sentences to add", sentencesArray.Size());
                Logger::logInt("tm id", tmId);
                for (rapidjson::SizeType i = 0; i < sentencesArray.Size(); i++) {
                    if (sentencesArray[i].Size() != 2) {
                        JsonGenerator::signalError(jsonWriter, "sentence should be an array of 2 elements");
                        break;
                    } else {
                        sourceSentences.push_back(sentencesArray[i][0].GetString());
                        targetSentences.push_back(sentencesArray[i][1].GetString());
                    }
                }
                _indexController->addAlignedSentences(jsonWriter, sourceSentences, targetSentences, tmId);
            } else if (operation == REFRESH_INDEX_OP) {
                int tmId = _getIntParameter(d, TM_ID_PARAM);
                _indexController->refreshIndexFromRAM(jsonWriter, tmId);
            } else if (operation == SIMPLE_SEARCH_OP) {
                std::string pattern = _getStringParameter(d, PATTERN_PARAM);
                int tmId = _getIntParameter(d, TM_ID_PARAM);
                _searcherController->simpleSearch(jsonWriter, pattern, tmId);
            } else if (operation == CONCORDIA_SEARCH_OP) {
                std::string pattern = _getStringParameter(d, PATTERN_PARAM);
                int tmId = _getIntParameter(d, TM_ID_PARAM);
                Logger::logString("concordia search pattern", pattern);
                _searcherController->concordiaSearch(jsonWriter, pattern, tmId);         
            } else if (operation == ADD_TM_OP) {
                int sourceLangId = _getIntParameter(d, SOURCE_LANG_PARAM);
                int targetLangId = _getIntParameter(d, TARGET_LANG_PARAM);
                std::string name = _getStringParameter(d, NAME_PARAM);
                int newId = _tmDAO.addTm(sourceLangId, targetLangId, name);
                _addTm(newId);
                
                jsonWriter.StartObject();
                jsonWriter.String("status");
                jsonWriter.String("success");
                jsonWriter.String("newTmId");
                jsonWriter.Int(newId);
                jsonWriter.EndObject();
                                
            } else {
                JsonGenerator::signalError(jsonWriter, "no such operation: " + operation);
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

std::string ConcordiaServer::_getStringParameter(rapidjson::Document & d, const char * name)
                                                                  throw (ConcordiaException) {
    rapidjson::Value::ConstMemberIterator itr = d.FindMember(name);
    if (itr != d.MemberEnd()) {
        std::string value = itr->value.GetString();
        return value;
    } else {
        throw ConcordiaException("missing parameter: " + std::string(name));
    }
}

int ConcordiaServer::_getIntParameter(rapidjson::Document & d, const char * name)
                                                       throw (ConcordiaException) {
    rapidjson::Value::ConstMemberIterator itr = d.FindMember(name);
    if (itr != d.MemberEnd()) {
        int value = itr->value.GetInt();
        return value;
    } else {
        throw ConcordiaException("missing parameter: " + std::string(name));
    }
}

void ConcordiaServer::_addTm(int tmId) {
    std::stringstream indexPath;
    indexPath << INDEX_DIRECTORY << "/tm_" << tmId;
    if (!boost::filesystem::exists(indexPath.str())) {
        boost::filesystem::create_directories(indexPath.str());
    }
    _concordiasMap->insert(tmId, new Concordia(indexPath.str(), _configFilePath));
}

