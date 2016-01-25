#ifndef SEARCHER_CONTROLLER_HDR
#define SEARCHER_CONTROLLER_HDR

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <concordia/concordia.hpp>
#include <concordia/concordia_exception.hpp>
#include <concordia/interval.hpp>

#include "unit_dao.hpp"
#include "simple_search_result.hpp"
#include "rapidjson/writer.h"


class SearcherController {
public:
    /*! Constructor.
    */
    explicit SearcherController(boost::shared_ptr<boost::ptr_map<int,Concordia> >concordiasMap)
                                                                      throw(ConcordiaException);
    /*! Destructor.
    */
    virtual ~SearcherController();

    void simpleSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                      std::string & pattern,
                      const int tmId);

    void concordiaSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                         std::string & pattern,
                         const int tmId);

    void concordiaPhraseSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                               std::string & pattern,
                               const std::vector<Interval> & intervals,
                               const int tmId);
private:

    boost::shared_ptr<boost::ptr_map<int,Concordia> > _concordiasMap;
    
    UnitDAO _unitDAO;
};

#endif
