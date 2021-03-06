#include "concordia_server.hpp"

#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <utility>

#include <concordia/interval.hpp>

#include "json_generator.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "tm.hpp"
#include "request.hpp"
#include "language.hpp"
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
    _lemmatizerFacade = boost::shared_ptr<LemmatizerFacade> (new LemmatizerFacade());

    _indexController = boost::shared_ptr<IndexController> (new IndexController(_concordiasMap, _lemmatizerFacade));
    _searcherController = boost::shared_ptr<SearcherController> (new SearcherController(_concordiasMap, _lemmatizerFacade));
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
        // Logger::logString("concordia request string", requestString);
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
                /*
                Logger::log("addSentences");
                Logger::logInt("sentences to add", sentencesArray.Size());
                Logger::logInt("tm id", tmId);
                */
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
                /*
                Logger::log("addAlignedSentences");
                Logger::logInt("sentences to add", sentencesArray.Size());
                Logger::logInt("tm id", tmId);
                */
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
            } else if (operation == ADD_ALIGNED_LEMMATIZED_SENTENCES_OP) {
                std::vector<std::string> sourceSentences;
                std::vector<std::string> targetSentences;
                std::vector<std::string> alignmentStrings;
                int tmId = d[TM_ID_PARAM].GetInt();
                // loading data from json
                const rapidjson::Value & sentencesArray = d[EXAMPLES_PARAM];
                /*
                Logger::log("addAlignedLemmatizedSentences");
                Logger::logInt("lemmatized sentences to add", sentencesArray.Size());
                Logger::logInt("tm id", tmId);
                */
                for (rapidjson::SizeType i = 0; i < sentencesArray.Size(); i++) {
                    if (sentencesArray[i].Size() != 3) {
                        JsonGenerator::signalError(jsonWriter, "sentence should be an array of 3 elements");
                        break;
                    } else {
                        sourceSentences.push_back(sentencesArray[i][0].GetString());
                        targetSentences.push_back(sentencesArray[i][1].GetString());
                        alignmentStrings.push_back(sentencesArray[i][2].GetString());
                    }
                }
                _indexController->addAlignedLemmatizedSentences(jsonWriter, sourceSentences, targetSentences, alignmentStrings, tmId);
            } else if (operation == GET_TMS_INFO_OP) {
                std::vector<Tm> tms = _tmDAO.getTms();

                jsonWriter.StartObject();
                jsonWriter.String("status");
                jsonWriter.String("success");
                jsonWriter.String("tms");
                jsonWriter.StartArray();
                BOOST_FOREACH(Tm & tm, tms) {
                    jsonWriter.StartObject();
                    jsonWriter.String("id");
                    jsonWriter.Int(tm.getId());
                    jsonWriter.String("name");
                    jsonWriter.String(tm.getName().c_str());
                    jsonWriter.String("sourceLanguageCode");
                    jsonWriter.String(tm.getSourceLanguageCode().c_str());
                    jsonWriter.String("targetLanguageCode");
                    jsonWriter.String(tm.getTargetLanguageCode().c_str());
                    jsonWriter.EndObject();
                }
                jsonWriter.EndArray();
                jsonWriter.EndObject();

            } else if (operation == GET_REQUESTS_INFO_OP) {
                std::vector<Request> requests = _requestDAO.getRequests();

                jsonWriter.StartObject();
                jsonWriter.String("status");
                jsonWriter.String("success");
                jsonWriter.String("requests");
                jsonWriter.StartArray();
                BOOST_FOREACH(Request & request, requests) {
                    jsonWriter.StartObject();
                    jsonWriter.String("id");
                    jsonWriter.Int(request.getId());
                    jsonWriter.String("sourceFilePath");
                    jsonWriter.String(request.getSourceFilePath().c_str());
                    jsonWriter.String("targetFilePath");
                    jsonWriter.String(request.getTargetFilePath().c_str());
                    jsonWriter.String("name");
                    jsonWriter.String(request.getName().c_str());
                    jsonWriter.String("sourceLanguageCode");
                    jsonWriter.String(request.getSourceLanguageCode().c_str());
                    jsonWriter.String("targetLanguageCode");
                    jsonWriter.String(request.getTargetLanguageCode().c_str());
                    jsonWriter.String("status");
                    jsonWriter.Int(request.getStatus());
                    jsonWriter.String("type");
                    jsonWriter.Int(request.getType());
                    jsonWriter.String("tmId");
                    jsonWriter.Int(request.getTmId());
                    jsonWriter.String("created");
                    jsonWriter.String(request.getCreated().c_str());
                    jsonWriter.EndObject();
                }
                jsonWriter.EndArray();
                jsonWriter.EndObject();


            } else if (operation == GET_LANGUAGES_OP) {
                std::vector<Language> languages = _languageDAO.getLanguages();

                jsonWriter.StartObject();
                jsonWriter.String("status");
                jsonWriter.String("success");
                jsonWriter.String("languages");
                jsonWriter.StartArray();
                BOOST_FOREACH(Language & language, languages) {
                    jsonWriter.StartObject();
                    jsonWriter.String("id");
                    jsonWriter.Int(language.getId());
                    jsonWriter.String("code");
                    jsonWriter.String(language.getCode().c_str());
                    jsonWriter.String("name");
                    jsonWriter.String(language.getName().c_str());
                    jsonWriter.EndObject();
                }
                jsonWriter.EndArray();
                jsonWriter.EndObject();


            } else if (operation == ADD_REQUEST_OP) {
                std::string sourceFilePath = _getStringParameter(d, SOURCE_FILE_PARAM);
                std::string targetFilePath = _getStringParameter(d, TARGET_FILE_PARAM);
                int sourceLangId = _getIntParameter(d, SOURCE_LANG_PARAM);
                int targetLangId = _getIntParameter(d, TARGET_LANG_PARAM);
                std::string name = _getStringParameter(d, NAME_PARAM);
                int type = _getIntParameter(d, TYPE_PARAM);
                int tmId = _getIntParameter(d, TM_ID_PARAM);
                int newId = _requestDAO.addRequest(sourceFilePath, targetFilePath, sourceLangId, targetLangId, name, type, tmId);
                jsonWriter.StartObject();
                jsonWriter.String("status");
                jsonWriter.String("success");
                jsonWriter.String("newRequestId");
                jsonWriter.Int(newId);
                jsonWriter.EndObject();

            } else if (operation == "lemmatize") {
                std::string sentence = _getStringParameter(d, "sentence");
                std::string languageCode = _getStringParameter(d, "languageCode");
                std::string lemmatizedSentence = _lemmatizerFacade->lemmatizeSentence(languageCode, sentence);
                jsonWriter.StartObject();
                jsonWriter.String("lemmatizedSentence");
                jsonWriter.String(lemmatizedSentence.c_str());
                jsonWriter.EndObject();
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
            } else if (operation == CONCORDIA_PHRASE_SEARCH_OP) {
                std::string pattern = _getStringParameter(d, PATTERN_PARAM);
                int tmId = _getIntParameter(d, TM_ID_PARAM);
                Logger::logString("concordia phrase search pattern", pattern);
                _logPhrase(requestString);
                std::vector<Interval> intervals;
                const rapidjson::Value & intervalsArray = d[INTERVALS_PARAM];
                for (rapidjson::SizeType i = 0; i < intervalsArray.Size(); i++) {
                    intervals.push_back(Interval(intervalsArray[i][0].GetInt(), intervalsArray[i][1].GetInt()));
                }
                _searcherController->concordiaPhraseSearch(jsonWriter, pattern, intervals, tmId);
            } else if (operation == ADD_TM_OP) {
                int sourceLangId = _getIntParameter(d, SOURCE_LANG_PARAM);
                int targetLangId = _getIntParameter(d, TARGET_LANG_PARAM);
                std::string name = _getStringParameter(d, NAME_PARAM);
                bool lemmatized = _getBoolParameter(d, TM_LEMMATIZED_PARAM);
                int newId = _tmDAO.addTm(sourceLangId, targetLangId, name, lemmatized);
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

int ConcordiaServer::_getBoolParameter(rapidjson::Document & d, const char * name)
                                                       throw (ConcordiaException) {
    rapidjson::Value::ConstMemberIterator itr = d.FindMember(name);
    if (itr != d.MemberEnd()) {
        bool value = itr->value.GetBool();
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

void ConcordiaServer::_logPhrase(std::string phraseString) {
    std::ofstream logFile;
    logFile.open(PHRASE_LOG_FILE_PATH, std::ios::out | std::ios::app);
    logFile << phraseString.substr(0, phraseString.size()-1) << ", \"timestamp\":" << std::time(0) << "}\n";
    logFile.close();
}
