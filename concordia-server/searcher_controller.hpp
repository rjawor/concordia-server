#ifndef SEARCHER_CONTROLLER_HDR
#define SEARCHER_CONTROLLER_HDR

#include <string>
#include <boost/shared_ptr.hpp>
#include <concordia/concordia.hpp>
#include <concordia/concordia_exception.hpp>

#include "rapidjson/writer.h"


using namespace std;

class SearcherController {
public:
    /*! Constructor.
    */
    explicit SearcherController(boost::shared_ptr<Concordia> concordia)
                                            throw(ConcordiaException);
    /*! Destructor.
    */
    virtual ~SearcherController();

    void simpleSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter, string & pattern);

    void concordiaSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter, string & pattern);

private:
    boost::shared_ptr<Concordia> _concordia;

};

#endif
