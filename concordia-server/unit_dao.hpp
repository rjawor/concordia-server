#ifndef UNIT_DAO_HDR
#define UNIT_DAO_HDR

#include <string>

#include <concordia/tokenized_sentence.hpp>
#include <boost/shared_ptr.hpp>

class UnitDAO {
public:
    /*! Constructor.
    */
    UnitDAO();
    /*! Destructor.
    */
    virtual ~UnitDAO();

    int addSentence(
             boost::shared_ptr<TokenizedSentence> sourceSentence,
             std::string & targetSentence,
             int tmId);
private:

};

#endif
