#include "unit_dao.hpp"

#include "db_connection.hpp"

UnitDAO::UnitDAO() {
}

UnitDAO::~UnitDAO() {
}

SUFFIX_MARKER_TYPE UnitDAO::addSentence(
     boost::shared_ptr<TokenizedSentence> sourceSentence,
     std::string & targetSentence,
     int tmId) {

    DBconnection connection;
    connection.startTransaction();
    connection.execute("INSERT INTO unit(source_segment) values('just testing')");
    connection.endTransaction();
     
}

