#include "unit_dao.hpp"

#include <sstream>
#include <string>

#include "query_param.hpp"
#include "string_param.hpp"
#include "int_param.hpp"
#include "int_array_param.hpp"
#include "logger.hpp"
#include "example_occurence.hpp"

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
             const int tmId) {
    DBconnection connection;
    std::vector<SUFFIX_MARKER_TYPE> newIds;
    connection.startTransaction();
    int index = 0;
    BOOST_FOREACH(const TokenizedSentence & sourceSentence, sourceSentences) {
        newIds.push_back(_addSingleSentence(connection, sourceSentence, targetSentences.at(index), tmId));
        index++;
    }
    connection.endTransaction();
    return newIds;
}

std::vector<SUFFIX_MARKER_TYPE> UnitDAO::addAlignedSentences(
         const std::vector<TokenizedSentence> & sourceSentences,
         const std::vector<TokenizedSentence> & targetSentences,
         const std::vector<std::vector<std::vector<int> > > & allAlignments,
         const int tmId) throw (ConcordiaException) {

    DBconnection connection;
    std::vector<SUFFIX_MARKER_TYPE> newIds;
    connection.startTransaction();

    for (int i=0; i< sourceSentences.size(); i++) {
        newIds.push_back(_addAlignedUnit(connection, sourceSentences.at(i), targetSentences.at(i), allAlignments.at(i), tmId));
    }

    connection.endTransaction();
    return newIds;
}

SimpleSearchResult UnitDAO::getSimpleSearchResult(const MatchedPatternFragment & fragment) {
    SimpleSearchResult result(fragment.getStart(), fragment.getEnd());
    TokenizedSentence ts("");
    return _getResultFromFragment(fragment, ts);
}

CompleteConcordiaSearchResult UnitDAO::getConcordiaResult(boost::shared_ptr<ConcordiaSearchResult> rawConcordiaResult) {
    CompleteConcordiaSearchResult result(rawConcordiaResult->getBestOverlayScore());
    BOOST_FOREACH(MatchedPatternFragment fragment, rawConcordiaResult->getBestOverlay()) {
        result.addToBestOverlay(_getResultFromFragment(fragment, rawConcordiaResult->getTokenizedPattern()));
    }
    return result;
}

CompleteConcordiaSearchResult UnitDAO::getConcordiaResult(boost::shared_ptr<ConcordiaSearchResult> rawConcordiaResult, TokenizedSentence originalPattern) {
    CompleteConcordiaSearchResult result(rawConcordiaResult->getBestOverlayScore());
    BOOST_FOREACH(MatchedPatternFragment fragment, rawConcordiaResult->getBestOverlay()) {
        result.addToBestOverlay(_getResultFromFragment(fragment, originalPattern));
    }
    return result;
}

SimpleSearchResult UnitDAO::_getResultFromFragment(
                                const MatchedPatternFragment & fragment,
                                const TokenizedSentence & tokenizedPattern) {

    DBconnection connection;
    connection.startTransaction();

    int matchedPatternStart = 0;
    int matchedPatternEnd = 0;
    if (tokenizedPattern.getTokens().size() > 0) {
        // if it is concordia searching
        matchedPatternStart = tokenizedPattern.getTokens().at(fragment.getStart()).getStart();
        matchedPatternEnd = tokenizedPattern.getTokens().at(fragment.getStart()+fragment.getMatchedLength() - 1).getEnd();
    }

    SimpleSearchResult ssResult(matchedPatternStart, matchedPatternEnd);

    BOOST_FOREACH(SubstringOccurence sOccurence, fragment.getOccurences()) {
        std::string query = "SELECT id, source_segment, target_segment, source_tokens[$1::integer], source_tokens[$2::integer] FROM unit WHERE id = $3::integer;";
        std::vector<QueryParam*> params;
        params.push_back(new IntParam(2*sOccurence.getOffset()+1));
        params.push_back(new IntParam(2*(sOccurence.getOffset()+fragment.getMatchedLength())));
        params.push_back(new IntParam(sOccurence.getId()));
        PGresult * result = connection.execute(query, params);
        ExampleOccurence occurence(connection.getIntValue(result,0,0),      // example id
                                    connection.getIntValue(result,0,3),      // matched example start
                                    connection.getIntValue(result,0,4),      // matched example end
                                    connection.getStringValue(result,0,1),   // source segment
                                    connection.getStringValue(result,0,2));  // target segment
        connection.clearResult(result);
        BOOST_FOREACH (QueryParam * param, params) {
            delete param;
        }

        // now add all target fragments matched with this fragment
        std::string targetQuery = "SELECT target_token_pos, target_tokens[2*target_token_pos+1], target_tokens[2*target_token_pos+2] FROM unit INNER JOIN alignment ON alignment.unit_id = unit.id AND unit.id = $1::integer AND source_token_pos between $2::integer and $3::integer ORDER BY target_token_pos";
        std::vector<QueryParam*> targetParams;
        targetParams.push_back(new IntParam(sOccurence.getId()));
        targetParams.push_back(new IntParam(sOccurence.getOffset()));
        targetParams.push_back(new IntParam(sOccurence.getOffset() + fragment.getMatchedLength() - 1));
        PGresult * targetResult = connection.execute(targetQuery, targetParams);

        int prevPos = -2;
        int currStart = -1;
        int currEnd = -1;

        for (int i=0;i<connection.getRowCount(targetResult);i++) {
            int targetPos = connection.getIntValue(targetResult, i, 0);
            int targetStart = connection.getIntValue(targetResult, i, 1);
            int targetEnd = connection.getIntValue(targetResult, i, 2);

            if (prevPos < targetPos - 1) { // beginning of detached fragment
                // check if there is a fragment to end
                if (currStart >= 0) {
                    occurence.addMatchedTargetFragment(std::pair<int,int>(currStart,currEnd));
                }
                currStart = targetStart;
            }

            currEnd = targetEnd;
            prevPos = targetPos;
        }

        // check if there are remaining fragments
        if (currStart >= 0) {
            occurence.addMatchedTargetFragment(std::pair<int,int>(currStart,currEnd));
        }

        connection.clearResult(targetResult);
        BOOST_FOREACH (QueryParam * param, targetParams) {
            delete param;
        }

        ssResult.addOccurence(occurence);

    }

    connection.endTransaction();

    return ssResult;
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


int UnitDAO::_addAlignedUnit (
     DBconnection & connection,
     const TokenizedSentence & sourceSentence,
     const TokenizedSentence & targetSentence,
     const std::vector<std::vector<int> > & alignments,
     const int tmId) throw(ConcordiaException) {

    if (sourceSentence.getTokens().size() != alignments.size()) {
        // Here we check if the source sentence, taken from src.tok,
        // is shorter than alignments array.
        std::stringstream ss;
        ss << "The size of source sentence is different than the size of alignments array. Source sentence: " << sourceSentence.getSentence() << ", alignments size:" << alignments.size();
        throw ConcordiaException(ss.str());
    }

    std::string query = "INSERT INTO unit(source_segment, target_segment, tm_id, source_tokens, target_tokens) values($1::text,$2::text,$3::integer,$4,$5) RETURNING id";
    std::vector<QueryParam*> params;
    params.push_back(new StringParam(sourceSentence.getOriginalSentence()));
    params.push_back(new StringParam(targetSentence.getOriginalSentence()));
    params.push_back(new IntParam(tmId));
    params.push_back(new IntArrayParam(_getTokenPositions(sourceSentence)));
    params.push_back(new IntArrayParam(_getTokenPositions(targetSentence)));

    PGresult * result = connection.execute(query, params);
    int newId = connection.getIntValue(result, 0, 0);
    connection.clearResult(result);
    BOOST_FOREACH (QueryParam * param, params) {
        delete param;
    }

    // add alignments
    bool nonEmpty = false;
    std::stringstream alignmentsQuery;
    alignmentsQuery << "INSERT INTO alignment(unit_id, source_token_pos, target_token_pos) values ";

    for(int i=0;i<alignments.size();i++) {
        for (int j=0;j<alignments[i].size();j++) {
            nonEmpty = true;
            alignmentsQuery << "(" << newId << "," << i << "," << alignments[i][j] << "),";
        }
    }
    if (nonEmpty) {
        query = alignmentsQuery.str();
        query = query.substr(0, query.length()-1);
        PGresult * result = connection.execute(query);
        connection.clearResult(result);
    }

    return newId;
}
