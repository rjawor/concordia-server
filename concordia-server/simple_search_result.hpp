#ifndef SIMPLE_SEARCH_RESULT_HDR
#define SIMPLE_SEARCH_RESULT_HDR

#include <string>

class SimpleSearchResult {
public:
    /*! Constructor.
    */
    SimpleSearchResult(const int id,
                       const int matchedPatternStart,
                       const int matchedPatternEnd,
                       const int matchedExampleStart,
                       const int matchedExampleEnd,
                       const std::string & sourceSegment,
                       const std::string & targetSegment                       
                      );
    /*! Destructor.
    */
    virtual ~SimpleSearchResult();
    
    int getId() const {
        return _id;
    }

    int getMatchedPatternStart() const {
        return _matchedPatternStart;
    }

    int getMatchedPatternEnd() const {
        return _matchedPatternEnd;
    }

    int getMatchedExampleStart() const {
        return _matchedExampleStart;
    }

    int getMatchedExampleEnd() const {
        return _matchedExampleEnd;
    }

    const std::string & getSourceSegment() const {
        return _sourceSegment;
    }

    const std::string & getTargetSegment() const {
        return _targetSegment;
    }
    
private:
    int _id;
    
    int _matchedPatternStart;

    int _matchedPatternEnd;

    int _matchedExampleStart;

    int _matchedExampleEnd;

    std::string _sourceSegment;

    std::string _targetSegment;
};

#endif
