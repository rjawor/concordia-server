#ifndef ALIGNED_UNIT_HDR
#define ALIGNED_UNIT_HDR

#include <vector>
#include <string>

#include <concordia/tokenized_sentence.hpp>

class AlignedUnit {
public:
    /*! Constructor.
    */
    AlignedUnit();
    /*! Destructor.
    */
    virtual ~AlignedUnit();
    
    boost::shared_ptr<TokenizedSentence> getSourceSentence() {
        return _sourceSentence;
    }
    
private:
    boost::shared_ptr<TokenizedSentence> _sourceSentence;
        
    boost::shared_ptr<TokenizedSentence> _targetSentence;

    std::vector<std::vector<int> > _alignments;    
};

#endif
