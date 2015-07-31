#ifndef INDEX_CONTROLLER_HDR
#define INDEX_CONTROLLER_HDR

#include <string>
#include <boost/shared_ptr.hpp>
#include <concordia/concordia.hpp>
#include <concordia/concordia_exception.hpp>

#include "rapidjson/writer.h"


using namespace std;

class IndexController {
public:
    /*! Constructor.
    */
    explicit IndexController(boost::shared_ptr<Concordia> concordia)
                                            throw(ConcordiaException);
    /*! Destructor.
    */
    virtual ~IndexController();

    void addSentence(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter, string & sentence);

private:
    boost::shared_ptr<Concordia> _concordia;

};

#endif
