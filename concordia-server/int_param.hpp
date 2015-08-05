#ifndef INT_PARAM_HDR
#define INT_PARAM_HDR

#include "query_param.hpp"

class IntParam : public QueryParam {
public:
    /*! Constructor.
    */
    IntParam(int value);
    /*! Destructor.
    */
    virtual ~IntParam();
    
    const char * getValue();
    
    const int getLength();
    
    const int isBinary();
private:
    unsigned int _value;
};

#endif
