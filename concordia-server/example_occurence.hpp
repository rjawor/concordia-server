#ifndef EXAMPLE_OCCURENCE_HDR
#define EXAMPLE_OCCURENCE_HDR

#include <string>
#include <vector>

class ExampleOccurence {
public:
    /*! Constructor.
    */
    ExampleOccurence (const int id,
                       const int matchedExampleStart,
                       const int matchedExampleEnd,
                       const std::string & sourceSegment,
                       const std::string & targetSegment
                      );
    /*! Destructor.
    */
    virtual ~ExampleOccurence();

    int getId() const {
        return _id;
    }

    int getMatchedExampleStart() const {
        return _matchedExampleStart;
    }

    int getMatchedExampleEnd() const {
        return _matchedExampleEnd;
    }

    const std::string & getSourceSegment() const {
        return _sourceSegment;
    }

    const std::string & getTargetSegment() const {
        return _targetSegment;
    }

    const std::vector<std::pair<int,int> > & getTargetFragments() const {
        return _targetFragments;
    }

    void addMatchedTargetFragment(const std::pair<int,int> & targetFragment);

private:
    int _id;

    int _matchedExampleStart;

    int _matchedExampleEnd;

    std::string _sourceSegment;

    std::string _targetSegment;

    std::vector<std::pair<int,int> > _targetFragments;
};

#endif
