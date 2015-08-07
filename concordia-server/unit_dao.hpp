#ifndef UNIT_DAO_HDR
#define UNIT_DAO_HDR

#include <string>
#include <vector>

#include <concordia/tokenized_sentence.hpp>
#include <concordia/substring_occurence.hpp>
#include <concordia/matched_pattern_fragment.hpp>
#include <boost/shared_ptr.hpp>

#include "simple_search_result.hpp"

class UnitDAO {
public:
    /*! Constructor.
    */
    UnitDAO();
    /*! Destructor.
    */
    virtual ~UnitDAO();

    int addSentence(
             boost::shared_ptr<TokenizedSentence> sourceSentence,
             std::string & targetSentence,
             int tmId);

    std::vector<SimpleSearchResult> getSearchResults(std::vector<MatchedPatternFragment> concordiaResults);

private:
    std::vector<int> _getTokenPositions(boost::shared_ptr<TokenizedSentence> ts);
};

#endif
