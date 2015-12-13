#ifndef INDEX_CONTROLLER_HDR
#define INDEX_CONTROLLER_HDR

#include <string>
#include <boost/shared_ptr.hpp>
#include <concordia/concordia.hpp>
#include <concordia/concordia_exception.hpp>
#include <boost/ptr_container/ptr_map.hpp>


#include "unit_dao.hpp"
#include "aligned_unit.hpp"

#include "rapidjson/writer.h"

class IndexController {
public:
    /*! Constructor.
    */
    explicit IndexController(boost::shared_ptr<boost::ptr_map<int,Concordia> >concordiasMap)
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
                     const int tmId);

    void addAlignedSentences(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                             const std::vector<std::string> & sourceSentences,
                             const std::vector<std::string> & targetSentences,
                             const int tmId);

    void refreshIndexFromRAM(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                             const int tmId);
    
private:
    std::vector<AlignedUnit> _getAlignedUnits(const std::vector<std::string> & sourceSentences,
                                              const std::vector<std::string> & targetSentences);

    boost::shared_ptr<boost::ptr_map<int,Concordia> > _concordiasMap;
    
    UnitDAO _unitDAO;
};

#endif
