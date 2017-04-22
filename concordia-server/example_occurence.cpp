#include "example_occurence.hpp"

ExampleOccurence::ExampleOccurence(
                       const int id,
                       const int matchedExampleStart,
                       const int matchedExampleEnd,
                       const std::string & sourceSegment,
                       const std::string & targetSegment):
                       _id(id),
                       _matchedExampleStart(matchedExampleStart),
                       _matchedExampleEnd(matchedExampleEnd),
                       _sourceSegment(sourceSegment),
                       _targetSegment(targetSegment) {
}

ExampleOccurence::~ExampleOccurence() {
}

void ExampleOccurence::addMatchedTargetFragment(const std::pair<int,int> & targetFragment) {
    _targetFragments.push_back(targetFragment);
}
