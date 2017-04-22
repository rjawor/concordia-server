#include "simple_search_result.hpp"

SimpleSearchResult::SimpleSearchResult(
                       const int matchedPatternStart,
                       const int matchedPatternEnd):
                       _matchedPatternStart(matchedPatternStart),
                       _matchedPatternEnd(matchedPatternEnd) {
}

SimpleSearchResult::~SimpleSearchResult() {
}

void SimpleSearchResult::addOccurence(const ExampleOccurence & occurence) {
    _occurences.push_back(occurence);
}

void SimpleSearchResult::offsetPattern(int offset) {
    _matchedPatternStart += offset;
    _matchedPatternEnd += offset;
}
