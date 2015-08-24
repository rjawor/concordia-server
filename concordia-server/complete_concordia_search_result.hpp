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

    std::vector<SimpleSearchResult> & getBestOverlay() {
        return _bestOverlay;
    }
    
private:
    double _bestOverlayScore;
    
    std::vector<SimpleSearchResult> _bestOverlay;
};

#endif
