#ifndef ALIGNED_UNIT_HDR
#define ALIGNED_UNIT_HDR

#include <vector>
#include <string>

#include <concordia/tokenized_sentence.hpp>

class AlignedUnit {
public:
    /*! Constructor.
    */
    AlignedUnit(const TokenizedSentence & sourceSentence,
                const TokenizedSentence & targetSentence,
                std::vector<std::vector<int> > alignments);
    /*! Destructor.
    */
    virtual ~AlignedUnit();
    
    TokenizedSentence getSourceSentence() const {
        return _sourceSentence;
    }

    TokenizedSentence getTargetSentence() const {
        return _targetSentence;
    }

    std::vector<std::vector<int> > getAlignments() const {
        return _alignments;
    }
    
private:
    TokenizedSentence _sourceSentence;
        
    TokenizedSentence _targetSentence;

    std::vector<std::vector<int> > _alignments;    
};

#endif
