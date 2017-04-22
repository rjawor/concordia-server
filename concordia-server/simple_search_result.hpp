#ifndef SIMPLE_SEARCH_RESULT_HDR
#define SIMPLE_SEARCH_RESULT_HDR

#include "example_occurence.hpp"
#include <string>
#include <vector>

class SimpleSearchResult {
public:
    /*! Constructor.
    */
    SimpleSearchResult(const int matchedPatternStart,
                       const int matchedPatternEnd);
    /*! Destructor.
    */
    virtual ~SimpleSearchResult();

    int getMatchedPatternStart() const {
        return _matchedPatternStart;
    }

    void setMatchedPatternStart(int newStart) {
        _matchedPatternStart = newStart;
    }

    int getMatchedPatternEnd() const {
        return _matchedPatternEnd;
    }

    void setMatchedPatternEnd(int newEnd) {
        _matchedPatternEnd = newEnd;
    }

    std::vector<ExampleOccurence> getOccurences() const {
        return _occurences;
    }

    void addOccurence(const ExampleOccurence & occurence);

    void offsetPattern(int offset);

private:
    std::vector<ExampleOccurence> _occurences;

    int _matchedPatternStart;

    int _matchedPatternEnd;
};

#endif
