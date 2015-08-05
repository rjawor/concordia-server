#ifndef STRING_PARAM_HDR
#define STRING_PARAM_HDR

#include "query_param.hpp"

#include <string>

class StringParam : public QueryParam {
public:
    /*! Constructor.
    */
    StringParam(std::string value);
    /*! Destructor.
    */
    virtual ~StringParam();
    
    const char * getValue();
    
    const int getLength();
    
    const int isBinary();
private:
    std::string _value;
};

#endif
