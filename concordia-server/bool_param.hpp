#ifndef BOOL_PARAM_HDR
#define BOOL_PARAM_HDR

#include "query_param.hpp"

class BoolParam : public QueryParam {
public:
    /*! Constructor.
    */
    BoolParam(bool value);
    /*! Destructor.
    */
    virtual ~BoolParam();

    const char * getValue();

    const int getLength();

    const int isBinary();
private:
    bool _value;
};

#endif
