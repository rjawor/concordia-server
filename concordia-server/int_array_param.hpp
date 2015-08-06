#ifndef INT_ARRAY_PARAM_HDR
#define INT_ARRAY_PARAM_HDR

#include "query_param.hpp"

#include <string>
#include <vector>

class IntArrayParam : public QueryParam {
public:
    /*! Constructor.
    */
    IntArrayParam(std::vector<int> array);
    /*! Destructor.
    */
    virtual ~IntArrayParam();
    
    const char * getValue();
    
    const int getLength();
    
    const int isBinary();
private:
    std::string _arrayString;
};

#endif
