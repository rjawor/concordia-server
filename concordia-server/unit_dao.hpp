#ifndef UNIT_DAO_HDR
#define UNIT_DAO_HDR

#include <string>

#include <concordia/common/config.hpp>

class UnitDAO {
public:
    /*! Constructor.
    */
    UnitDAO();
    /*! Destructor.
    */
    virtual ~UnitDAO();

    SUFFIX_MARKER_TYPE addSentence(std::string & sentence);
private:

};

#endif
