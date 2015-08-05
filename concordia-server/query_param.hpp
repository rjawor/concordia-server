#ifndef QUERY_PARAM_HDR
#define QUERY_PARAM_HDR

class QueryParam {
public:
    /*! Constructor.
    */
    QueryParam();
    /*! Destructor.
    */
    virtual ~QueryParam();
    
    virtual const char * getValue();
    
    virtual const int getLength();
    
    virtual const int isBinary();
private:

};

#endif
