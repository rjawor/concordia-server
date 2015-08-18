#include "simple_search_result.hpp"

SimpleSearchResult::SimpleSearchResult(
                       const int id,
                       const int matchedFragmentStart,
                       const int matchedFragmentEnd,
                       const std::string & sourceSegment,
                       const std::string & targetSegment):
                       _id(id),
                       _matchedFragmentStart(matchedFragmentStart),
                       _matchedFragmentEnd(matchedFragmentEnd),
                       _sourceSegment(sourceSegment),
                       _targetSegment(targetSegment) {
}

SimpleSearchResult::~SimpleSearchResult() {
}

