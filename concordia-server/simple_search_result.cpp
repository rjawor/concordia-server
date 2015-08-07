#include "simple_search_result.hpp"

SimpleSearchResult::SimpleSearchResult(
                       const int id,
                       const std::string & matchedFragment,
                       const std::string & sourceSegment,
                       const std::string & targetSegment):
                       _id(id),
                       _matchedFragment(matchedFragment),
                       _sourceSegment(sourceSegment),
                       _targetSegment(targetSegment) {
}

SimpleSearchResult::~SimpleSearchResult() {
}

