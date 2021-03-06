#ifndef COMPLETE_CONCORDIA_SEARCH_RESULT_HDR
#define COMPLETE_CONCORDIA_SEARCH_RESULT_HDR

#include <vector>

#include "simple_search_result.hpp"

class CompleteConcordiaSearchResult {
public:
    /*! Constructor.
    */
    CompleteConcordiaSearchResult(const double bestOverlayScore);

    /*! Destructor.
    */
    virtual ~CompleteConcordiaSearchResult();

    const double getBestOverlayScore() {
        return _bestOverlayScore;
    }

    std::vector<SimpleSearchResult> getBestOverlay() const {
        return _bestOverlay;
    }

    void addToBestOverlay(const SimpleSearchResult & result);

    void offsetPattern(int offset);

private:
    double _bestOverlayScore;

    std::vector<SimpleSearchResult> _bestOverlay;
};

#endif
