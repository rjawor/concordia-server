#include "unit_dao.hpp"

#include "query_param.hpp"
#include "string_param.hpp"
#include "int_param.hpp"
#include "int_array_param.hpp"
#include "logger.hpp"

#include <libpq-fe.h>
#include <boost/foreach.hpp>
#include <concordia/token_annotation.hpp>

UnitDAO::UnitDAO() {
}

UnitDAO::~UnitDAO() {
}

int UnitDAO::addSentence(
     const TokenizedSentence & sourceSentence,
     const std::string & targetSentence,
     const int tmId) {
    
    DBconnection connection;
    connection.startTransaction();
    int newId = _addSingleSentence(connection, sourceSentence, targetSentence, tmId);
    connection.endTransaction();
    return newId;
}

std::vector<SUFFIX_MARKER_TYPE> UnitDAO::addSentences(
             const std::vector<TokenizedSentence> & sourceSentences,
             const std::vector<std::string> & targetSentences,
             const std::vector<int> & tmIds) {
    DBconnection connection;
    std::vector<SUFFIX_MARKER_TYPE> newIds;
    connection.startTransaction();
    int index = 0;
    BOOST_FOREACH(const TokenizedSentence & sourceSentence, sourceSentences) {    
        newIds.push_back(_addSingleSentence(connection, sourceSentence, targetSentences.at(index), tmIds.at(index)));
        index++;
    }
    connection.endTransaction();
    return newIds;
}


std::vector<SimpleSearchResult> UnitDAO::getSearchResults(const std::vector<MatchedPatternFragment> & fragments) {
    std::vector<SimpleSearchResult> results;
    TokenizedSentence ts("");
    _getResultsFromFragments(results, fragments, ts);
    return results;
}

CompleteConcordiaSearchResult UnitDAO::getConcordiaResult(boost::shared_ptr<ConcordiaSearchResult> rawConcordiaResult) {
    CompleteConcordiaSearchResult result(rawConcordiaResult->getBestOverlayScore());
    _getResultsFromFragments(result.getBestOverlay(),
                             rawConcordiaResult->getBestOverlay(),
                             rawConcordiaResult->getTokenizedPattern());
    return result;
}

void UnitDAO::_getResultsFromFragments(
                              std::vector<SimpleSearchResult> & results,
                              const std::vector<MatchedPatternFragment> & fragments,
                              const TokenizedSentence & tokenizedPattern) {
    
    DBconnection connection;
    connection.startTransaction();

    BOOST_FOREACH(const MatchedPatternFragment & fragment, fragments) {
        int matchedPatternStart = 0;
        int matchedPatternEnd = 0;
        if (tokenizedPattern.getTokens().size() > 0) {
            // if it is concordia searching
            matchedPatternStart = tokenizedPattern.getTokens().at(fragment.getStart()).getStart();
            matchedPatternEnd = tokenizedPattern.getTokens().at(fragment.getStart()+fragment.getMatchedLength() - 1).getEnd();
        }
        
        std::string query = "SELECT id, source_segment, target_segment, source_tokens[$1::integer], source_tokens[$2::integer] FROM unit WHERE id = $3::integer;";
        std::vector<QueryParam*> params;
        params.push_back(new IntParam(2*fragment.getExampleOffset()+1));
        params.push_back(new IntParam(2*(fragment.getExampleOffset()+fragment.getMatchedLength())));
        params.push_back(new IntParam(fragment.getExampleId()));
        PGresult * result = connection.execute(query, params);
        results.push_back(SimpleSearchResult(connection.getIntValue(result,0,0),      // example id
                                             matchedPatternStart,
                                             matchedPatternEnd,
                                             connection.getIntValue(result,0,3),      // matched example start
                                             connection.getIntValue(result,0,4),      // matched example end
                                             connection.getStringValue(result,0,1),   // source segment
                                             connection.getStringValue(result,0,2))); // target segment
        connection.clearResult(result);
        BOOST_FOREACH (QueryParam * param, params) {
            delete param;
        }
    }    
    connection.endTransaction();
}


std::vector<int> UnitDAO::_getTokenPositions(const TokenizedSentence & ts) {
    std::vector<int> result;
    BOOST_FOREACH(const TokenAnnotation & token, ts.getTokens()) {
        result.push_back(token.getStart());
        result.push_back(token.getEnd());
    }
    return result;
}

int UnitDAO::_addSingleSentence(
         DBconnection & connection,
         const TokenizedSentence & sourceSentence,
         const std::string & targetSentence,
         const int tmId) {
    std::string query = "INSERT INTO unit(source_segment, target_segment, tm_id, source_tokens) values($1::text,$2::text,$3::integer,$4) RETURNING id";
    std::vector<QueryParam*> params;
    params.push_back(new StringParam(sourceSentence.getSentence()));
    params.push_back(new StringParam(targetSentence));
    params.push_back(new IntParam(tmId));
    params.push_back(new IntArrayParam(_getTokenPositions(sourceSentence)));
    
    PGresult * result = connection.execute(query, params);
    int newId = connection.getIntValue(result, 0, 0);
    connection.clearResult(result);
    BOOST_FOREACH (QueryParam * param, params) {
        delete param;
    }
    
    return newId;
}




