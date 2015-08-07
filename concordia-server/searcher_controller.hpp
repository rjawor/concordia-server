#ifndef SEARCHER_CONTROLLER_HDR
#define SEARCHER_CONTROLLER_HDR

#include <string>
#include <boost/shared_ptr.hpp>
#include <concordia/concordia.hpp>
#include <concordia/concordia_exception.hpp>

#include "unit_dao.hpp"
#include "simple_search_result.hpp"
#include "rapidjson/writer.h"


class SearcherController {
public:
    /*! Constructor.
    */
    explicit SearcherController(boost::shared_ptr<Concordia> concordia)
                                            throw(ConcordiaException);
    /*! Destructor.
    */
    virtual ~SearcherController();

    void simpleSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter, std::string & pattern);

    void concordiaSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter, std::string & pattern);

private:

    boost::shared_ptr<Concordia> _concordia;
    
    UnitDAO _unitDAO;
};

#endif
