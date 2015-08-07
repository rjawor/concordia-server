#ifndef SIMPLE_SEARCH_RESULT_HDR
#define SIMPLE_SEARCH_RESULT_HDR

#include <string>

class SimpleSearchResult {
public:
    /*! Constructor.
    */
    SimpleSearchResult(const int id,
                       const std::string & matchedFragment,
                       const std::string & sourceSegment,
                       const std::string & targetSegment                       
                      );
    /*! Destructor.
    */
    virtual ~SimpleSearchResult();
    
    int & getId() {
        return _id;
    }

    const std::string & getMatchedFragment() {
        return _matchedFragment;
    }

    const std::string & getSourceSegment() {
        return _sourceSegment;
    }

    const std::string & getTargetSegment() {
        return _targetSegment;
    }
    
private:
    int _id;
    
    std::string _matchedFragment;

    std::string _sourceSegment;

    std::string _targetSegment;
};

#endif
