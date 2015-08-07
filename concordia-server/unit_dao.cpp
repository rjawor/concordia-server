#include "unit_dao.hpp"

#include "db_connection.hpp"
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
     boost::shared_ptr<TokenizedSentence> sourceSentence,
     std::string & targetSentence,
     int tmId) {
    
    DBconnection connection;
    connection.startTransaction();
    std::string query = "INSERT INTO unit(source_segment, target_segment, tm_id, source_tokens) values($1::text,$2::text,$3::integer,$4) RETURNING id";
    std::vector<QueryParam*> params;
    params.push_back(new StringParam(sourceSentence->getSentence()));
    params.push_back(new StringParam(targetSentence));
    params.push_back(new IntParam(tmId));
    params.push_back(new IntArrayParam(_getTokenPositions(sourceSentence)));
    
    PGresult * result = connection.execute(query, params);
    int newId = connection.getIntValue(result, 0, 0);
    connection.clearResult(result);
    connection.endTransaction();
    
    BOOST_FOREACH (QueryParam * param, params) {
        delete param;
    }

    return newId;
     
}

std::vector<SimpleSearchResult> UnitDAO::getSearchResults(std::vector<MatchedPatternFragment> concordiaResults) {
    std::vector<SimpleSearchResult> results;

    DBconnection connection;
    connection.startTransaction();

    BOOST_FOREACH(MatchedPatternFragment & fragment, concordiaResults) {
        std::string query = "SELECT id, source_segment, target_segment, substring(source_segment,source_tokens[$1::integer*2+1]+1,source_tokens[$2::integer*2]-source_tokens[$1::integer*2+1]) as matched_fragment FROM unit WHERE id = $3::integer;";
        std::vector<QueryParam*> params;
        params.push_back(new IntParam(fragment.getExampleOffset()));
        params.push_back(new IntParam(fragment.getExampleOffset()+fragment.getMatchedLength()));
        params.push_back(new IntParam(fragment.getExampleId()));
        std::stringstream ss;
        ss << "example offset: " << fragment.getExampleOffset()
           << ", matched length: " << fragment.getMatchedLength()
           << ", example id: " << fragment.getExampleId();
        Logger::log(ss.str());
        PGresult * result = connection.execute(query, params);
        
        results.push_back(SimpleSearchResult(connection.getIntValue(result,0,0),
                                             connection.getStringValue(result,0,3),
                                             connection.getStringValue(result,0,1),
                                             connection.getStringValue(result,0,2)));
        connection.clearResult(result);
    }    
    connection.endTransaction();
    return results;
}


std::vector<int> UnitDAO::_getTokenPositions(boost::shared_ptr<TokenizedSentence> ts) {
    std::vector<int> result;
    BOOST_FOREACH(const TokenAnnotation & token, ts->getTokens()) {
        result.push_back(token.getStart());
        result.push_back(token.getEnd());
    }
    return result;
}



