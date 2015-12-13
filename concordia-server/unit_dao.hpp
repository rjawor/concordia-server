#ifndef UNIT_DAO_HDR
#define UNIT_DAO_HDR

#include <string>
#include <vector>

#include <concordia/common/config.hpp>
#include <concordia/tokenized_sentence.hpp>
#include <concordia/substring_occurence.hpp>
#include <concordia/matched_pattern_fragment.hpp>
#include <concordia/concordia_search_result.hpp>
#include <boost/shared_ptr.hpp>

#include "aligned_unit.hpp"
#include "simple_search_result.hpp"
#include "complete_concordia_search_result.hpp"
#include "db_connection.hpp"

class UnitDAO {
public:
    /*! Constructor.
    */
    UnitDAO();
    /*! Destructor.
    */
    virtual ~UnitDAO();

    int addSentence(
             const TokenizedSentence & sourceSentence,
             const std::string & targetSentence,
             const int tmId);

    std::vector<SUFFIX_MARKER_TYPE> addSentences(
             const std::vector<TokenizedSentence> & sourceSentences,
             const std::vector<std::string> & targetSentences,
             const int tmId);
             
    std::vector<SUFFIX_MARKER_TYPE> addAlignedUnits(
             const std::vector<AlignedUnit> & alignedUnits,
             const int tmId);
    
    std::vector<SimpleSearchResult> getSearchResults(const std::vector<MatchedPatternFragment> & fragments);

    CompleteConcordiaSearchResult getConcordiaResult(boost::shared_ptr<ConcordiaSearchResult> rawConcordiaResult);

private:
    void _getResultsFromFragments(std::vector<SimpleSearchResult> & results,
                                  const std::vector<MatchedPatternFragment> & fragments,
                                  const TokenizedSentence & tokenizedPattern);

    std::vector<int> _getTokenPositions(const TokenizedSentence & ts);
    
    int _addSingleSentence(
         DBconnection & connection,
         const TokenizedSentence & sourceSentence,
         const std::string & targetSentence,
         const int tmId);

};

#endif
