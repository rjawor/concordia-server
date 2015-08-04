#ifndef UNIT_DAO_HDR
#define UNIT_DAO_HDR

#include <string>

#include <concordia/tokenized_sentence.hpp>
#include <concordia/common/config.hpp>
#include <boost/shared_ptr.hpp>

class UnitDAO {
public:
    /*! Constructor.
    */
    UnitDAO();
    /*! Destructor.
    */
    virtual ~UnitDAO();

    SUFFIX_MARKER_TYPE addSentence(
             boost::shared_ptr<TokenizedSentence> sourceSentence,
             std::string & targetSentence,
             int tmId);
private:

};

#endif
