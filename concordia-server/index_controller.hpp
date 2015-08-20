#ifndef INDEX_CONTROLLER_HDR
#define INDEX_CONTROLLER_HDR

#include <string>
#include <boost/shared_ptr.hpp>
#include <concordia/concordia.hpp>
#include <concordia/concordia_exception.hpp>

#include "unit_dao.hpp"

#include "rapidjson/writer.h"

class IndexController {
public:
    /*! Constructor.
    */
    explicit IndexController(boost::shared_ptr<Concordia> concordia)
                                            throw(ConcordiaException);
    /*! Destructor.
    */
    virtual ~IndexController();

    void addSentence(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                     const std::string & sourceSentence,
                     const std::string & targetSentence,
                     const int tmId);

    void addSentences(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                     const std::vector<std::string> & sourceSentences,
                     const std::vector<std::string> & targetSentences,
                     const std::vector<int> & tmIds);

private:
    boost::shared_ptr<Concordia> _concordia;
    
    UnitDAO _unitDAO;
};

#endif
