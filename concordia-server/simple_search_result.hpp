#ifndef SIMPLE_SEARCH_RESULT_HDR
#define SIMPLE_SEARCH_RESULT_HDR

#include <string>

class SimpleSearchResult {
public:
    /*! Constructor.
    */
    SimpleSearchResult();
    /*! Destructor.
    */
    virtual ~SimpleSearchResult();
    
private:
    int id;
    
    std::string _matchedFragment;

    std::string _sourceSegment;

    std::string _targetSegment;
};

#endif
