#include "aligned_unit.hpp"


AlignedUnit::AlignedUnit(const TokenizedSentence & sourceSentence,
                         const TokenizedSentence & targetSentence,
                         std::vector<std::vector<int> > alignments):
                         _sourceSentence(sourceSentence),
                         _targetSentence(targetSentence),
                         _alignments(alignments) {
}


AlignedUnit::~AlignedUnit() {
}

