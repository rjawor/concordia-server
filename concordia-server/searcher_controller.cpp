#include "searcher_controller.hpp"

#include <boost/foreach.hpp>
#include <vector>
#include <climits>
#include <concordia/tokenized_sentence.hpp>


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
        TokenizedSentence tokenizedPattern = it->second->tokenize(pattern, false, false);
        pattern = _lemmatizerFacade->lemmatizeIfNeeded(tokenizedPattern.getTokenizedSentence(), tmId);
        SimpleSearchResult result = _unitDAO.getSimpleSearchResult(it->second->simpleSearch(pattern, true));
        jsonWriter.StartObject();
        jsonWriter.String("status");
        jsonWriter.String("success");
        jsonWriter.String("result");
        JsonGenerator::writeSimpleSearchResult(jsonWriter, result);
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
            SimpleSearchResult shortPatternResult = _unitDAO.getSimpleSearchResult(it->second->simpleSearch(shortPattern));



            jsonWriter.StartObject();
            jsonWriter.String("status");
            jsonWriter.String("success");
            jsonWriter.String("found");
            if (shortPatternResult.getOccurences().size() > 0) {
                jsonWriter.Bool(true);


                std::vector<SimpleSearchResult> bestOverlay;

                int currStart = 0;
                BOOST_FOREACH(const Interval & interval, intervals) {
                    CompleteConcordiaSearchResult restResult = _unitDAO.getConcordiaResult(
                                                    it->second->concordiaSearch(pattern.substr(currStart, interval.getStart()-currStart)));
                    restResult.offsetPattern(currStart);
                    bestOverlay.insert(bestOverlay.end(), restResult.getBestOverlay().begin(), restResult.getBestOverlay().end());

                    shortPatternResult.setMatchedPatternStart(interval.getStart());
                    shortPatternResult.setMatchedPatternEnd(interval.getEnd());
                    bestOverlay.push_back(shortPatternResult);
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
                    JsonGenerator::writeSimpleSearchResult(jsonWriter, simpleResult);
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
        TokenizedSentence originalPattern = it->second->tokenize(pattern, false, false);
        std::string lemmatizedPattern = _lemmatizerFacade->lemmatizeIfNeeded(originalPattern.getTokenizedSentence(), tmId);
        boost::shared_ptr<ConcordiaSearchResult> rawConcordiaResult = it->second->concordiaSearch(lemmatizedPattern, true);
        CompleteConcordiaSearchResult result = _unitDAO.getConcordiaResult(rawConcordiaResult, originalPattern);

        jsonWriter.StartObject();
        jsonWriter.String("status");
        jsonWriter.String("success");
        jsonWriter.String("result");
        jsonWriter.StartObject();
        jsonWriter.String("bestOverlayScore");
        jsonWriter.Double(result.getBestOverlayScore());
        jsonWriter.String("bestOverlay");
        jsonWriter.StartArray();
        BOOST_FOREACH(const SimpleSearchResult & simpleResult, result.getBestOverlay()) {
            JsonGenerator::writeSimpleSearchResult(jsonWriter, simpleResult);
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
