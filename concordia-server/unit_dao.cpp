#include "unit_dao.hpp"

#include "db_connection.hpp"
#include "query_param.hpp"
#include "string_param.hpp"
#include "int_param.hpp"
#include "logger.hpp"

#include <vector>
#include <boost/foreach.hpp>

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
    std::string query = "INSERT INTO unit(source_segment, target_segment, tm_id) values($1::text,$2::text,$3::integer)";
    std::vector<QueryParam*> params;
    params.push_back(new StringParam(sourceSentence->getSentence()));
    params.push_back(new StringParam(targetSentence));
    params.push_back(new IntParam(tmId));
    
    connection.execute(query, params);
    connection.endTransaction();
    
    BOOST_FOREACH (QueryParam * param, params) {
        delete param;
    }

    //todo return new unit id
    return 0;
     
}

