#ifndef SIMPLE_SEARCH_RESULT_HDR
#define SIMPLE_SEARCH_RESULT_HDR

#include <string>

class SimpleSearchResult {
public:
    /*! Constructor.
    */
    SimpleSearchResult(const int id,
                       const int matchedFragmentStart,
                       const int matchedFragmentEnd,
                       const std::string & sourceSegment,
                       const std::string & targetSegment                       
                      );
    /*! Destructor.
    */
    virtual ~SimpleSearchResult();
    
    const int getId() {
        return _id;
    }

    const int getMatchedFragmentStart() {
        return _matchedFragmentStart;
    }

    const int getMatchedFragmentEnd() {
        return _matchedFragmentEnd;
    }

    const std::string & getSourceSegment() {
        return _sourceSegment;
    }

    const std::string & getTargetSegment() {
        return _targetSegment;
    }
    
private:
    int _id;
    
    int _matchedFragmentStart;

    int _matchedFragmentEnd;

    std::string _sourceSegment;

    std::string _targetSegment;
};

#endif
