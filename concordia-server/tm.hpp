#ifndef TM_HDR
#define TM_HDR

#include <string>
#include <vector>

class Tm {
public:
    /*! Constructor.
    */
    Tm(const int id,
       const std::string & name,
       const std::string & sourceLanguageCode,
       const std::string & targetLanguageCode);
    /*! Destructor.
    */
    virtual ~Tm();

    int getId() const {
        return _id;
    }

    const std::string & getName() const {
        return _name;
    }

    const std::string & getSourceLanguageCode() const {
        return _sourceLanguageCode;
    }

    const std::string & getTargetLanguageCode() const {
        return _targetLanguageCode;
    }


private:
    int _id;

    std::string _name;

    std::string _sourceLanguageCode;

    std::string _targetLanguageCode;
};

#endif
