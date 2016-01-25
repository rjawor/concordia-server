#include "simple_search_result.hpp"

SimpleSearchResult::SimpleSearchResult(
                       const int id,
                       const int matchedPatternStart,
                       const int matchedPatternEnd,
                       const int matchedExampleStart,
                       const int matchedExampleEnd,
                       const std::string & sourceSegment,
                       const std::string & targetSegment):
                       _id(id),
                       _matchedPatternStart(matchedPatternStart),
                       _matchedPatternEnd(matchedPatternEnd),
                       _matchedExampleStart(matchedExampleStart),
                       _matchedExampleEnd(matchedExampleEnd),
                       _sourceSegment(sourceSegment),
                       _targetSegment(targetSegment) {
}

SimpleSearchResult::~SimpleSearchResult() {
}

void SimpleSearchResult::addMatchedTargetFragment(const std::pair<int,int> & targetFragment) {
    _targetFragments.push_back(targetFragment);
}

void SimpleSearchResult::offsetPattern(int offset) {
    _matchedPatternStart += offset;
    _matchedPatternEnd += offset;
}

