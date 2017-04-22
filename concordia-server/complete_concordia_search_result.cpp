#include "complete_concordia_search_result.hpp"

#include <boost/foreach.hpp>

CompleteConcordiaSearchResult::CompleteConcordiaSearchResult(
                       const double bestOverlayScore):
                       _bestOverlayScore(bestOverlayScore) {
}

CompleteConcordiaSearchResult::~CompleteConcordiaSearchResult() {
}

void CompleteConcordiaSearchResult::addToBestOverlay(const SimpleSearchResult & result) {
    _bestOverlay.push_back(result);
}


void CompleteConcordiaSearchResult::offsetPattern(int offset) {
    BOOST_FOREACH(SimpleSearchResult & simpleResult, _bestOverlay) {
        simpleResult.offsetPattern(offset);
    }

}
