#include "searcher_controller.hpp"

#include <boost/foreach.hpp>
#include <vector>
#include <climits>

#include "json_generator.hpp"
#include "logger.hpp"


SearcherController::SearcherController(boost::shared_ptr<boost::ptr_map<int,Concordia> >concordiasMap,
                                       boost::shared_ptr<LemmatizerFacade> lemmatizerFacade)
                                                                     throw(ConcordiaException):
                                                                     _concordiasMap(concordiasMap),
                                                                     _lemmatizerFacade(lemmatizerFacade) {
}

SearcherController::~SearcherController() {
}


void SearcherController::simpleSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                                      std::string & pattern,
                                      const int tmId) {
    boost::ptr_map<int,Concordia>::iterator it = _concordiasMap->find(tmId);
    if (it != _concordiasMap->end()) {
        pattern = _lemmatizerFacade->lemmatizeIfNeeded(pattern, tmId);
        std::vector<SimpleSearchResult> results = _unitDAO.getSearchResults(it->second->simpleSearch(pattern));

        jsonWriter.StartObject();
        jsonWriter.String("status");
        jsonWriter.String("success");
        jsonWriter.String("results");
        jsonWriter.StartArray();
        BOOST_FOREACH(SimpleSearchResult & result, results) {
            JsonGenerator::writeSearchResult(jsonWriter, result);
        }
        jsonWriter.EndArray();
        jsonWriter.EndObject();
    } else {
        JsonGenerator::signalError(jsonWriter, "no such tm!");
    }
}

void SearcherController::concordiaPhraseSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                                               std::string & pattern,
                                               const std::vector<Interval> & intervals,
                                               const int tmId) {
    boost::ptr_map<int,Concordia>::iterator it = _concordiasMap->find(tmId);
    if (it != _concordiasMap->end()) {
        if (intervals.size() > 0) {
//            std::string shortPattern = pattern.substr(intervals[0].getStart(), intervals[0].getEnd() - intervals[0].getStart());
            pattern = _lemmatizerFacade->lemmatizeIfNeeded(pattern, tmId);
            std::string shortPattern = _substrUTF8(pattern, intervals[0].getStart(), intervals[0].getEnd() - intervals[0].getStart());

            Logger::log("concordiaPhraseSearch");
            Logger::logString("short pattern", shortPattern);
            std::vector<SimpleSearchResult> shortPatternResults = _unitDAO.getSearchResults(it->second->simpleSearch(shortPattern));



            jsonWriter.StartObject();
            jsonWriter.String("status");
            jsonWriter.String("success");
            jsonWriter.String("found");
            if (shortPatternResults.size() > 0) {
                jsonWriter.Bool(true);


                std::vector<SimpleSearchResult> bestOverlay;

                int currStart = 0;
                BOOST_FOREACH(const Interval & interval, intervals) {
                    CompleteConcordiaSearchResult restResult = _unitDAO.getConcordiaResult(
                                                    it->second->concordiaSearch(pattern.substr(currStart, interval.getStart()-currStart)));
                    restResult.offsetPattern(currStart);
                    bestOverlay.insert(bestOverlay.end(), restResult.getBestOverlay().begin(), restResult.getBestOverlay().end());

                    SimpleSearchResult shortPatternresult = shortPatternResults[0];
                    shortPatternresult.setMatchedPatternStart(interval.getStart());
                    shortPatternresult.setMatchedPatternEnd(interval.getEnd());
                    bestOverlay.push_back(shortPatternresult);
                    currStart = interval.getEnd();
                }
                CompleteConcordiaSearchResult lastRestResult = _unitDAO.getConcordiaResult(
                                                it->second->concordiaSearch(_substrUTF8(pattern,currStart,INT_MAX)));
                lastRestResult.offsetPattern(currStart);
                bestOverlay.insert(bestOverlay.end(), lastRestResult.getBestOverlay().begin(), lastRestResult.getBestOverlay().end());

                jsonWriter.String("result");
                jsonWriter.StartObject();
                jsonWriter.String("bestOverlay");
                jsonWriter.StartArray();
                BOOST_FOREACH(SimpleSearchResult & simpleResult, bestOverlay) {
                    JsonGenerator::writeSearchResult(jsonWriter, simpleResult);
                }
                jsonWriter.EndArray();
                jsonWriter.EndObject();
            } else {
                jsonWriter.Bool(false);
            }
            jsonWriter.EndObject();
        } else {
            JsonGenerator::signalError(jsonWriter, "no intervals for phrase search");
        }
    } else {
        JsonGenerator::signalError(jsonWriter, "no such tm!");
    }
}


void SearcherController::concordiaSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                                         std::string & pattern,
                                         const int tmId) {

    boost::ptr_map<int,Concordia>::iterator it = _concordiasMap->find(tmId);
    if (it != _concordiasMap->end()) {
        std::string lemmatizedPattern = _lemmatizerFacade->lemmatizeIfNeeded(pattern, tmId);
        TokenizedSentence originalPattern = it->second->tokenize(pattern, true, false);
        CompleteConcordiaSearchResult result = _unitDAO.getConcordiaResult(it->second->concordiaSearch(lemmatizedPattern), originalPattern);

        jsonWriter.StartObject();
        jsonWriter.String("status");
        jsonWriter.String("success");
        jsonWriter.String("result");
        jsonWriter.StartObject();
        jsonWriter.String("bestOverlayScore");
        jsonWriter.Double(result.getBestOverlayScore());
        jsonWriter.String("bestOverlay");
        jsonWriter.StartArray();
        BOOST_FOREACH(SimpleSearchResult & simpleResult, result.getBestOverlay()) {
            JsonGenerator::writeSearchResult(jsonWriter, simpleResult);
        }
        jsonWriter.EndArray();
        jsonWriter.EndObject();


        jsonWriter.EndObject();
    } else {
        JsonGenerator::signalError(jsonWriter, "no such tm!");
    }
}

std::string SearcherController::_substrUTF8(std::string source, int start, int length) {
    UnicodeString s(source.c_str());

    UnicodeString unicodeValue;
    s.extract(start, length, unicodeValue);

    std::string result;
    unicodeValue.toUTF8String(result);

    return result;
}
