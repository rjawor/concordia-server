#ifndef REQUEST_HDR
#define REQUEST_HDR

#include <string>
#include <vector>

class Request {
public:
    /*! Constructor.
    */
    Request(
       const int id,
       const std::string & sourceFilePath,
       const std::string & targetFilePath,
       const std::string & name,
       const std::string & sourceLanguageCode,
       const std::string & targetLanguageCode,
       const int status,
       const int type,
       const int tm_id,
       const std::string & created
   );
    /*! Destructor.
    */
    virtual ~Request();

    int getId() const {
        return _id;
    }

    const std::string & getSourceFilePath() const {
        return _sourceFilePath;
    }

    const std::string & getTargetFilePath() const {
        return _targetFilePath;
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

    int getStatus() const {
        return _status;
    }

    int getType() const {
        return _type;
    }

    int getTmId() const {
        return _tmId;
    }

    const std::string & getCreated() const {
        return _created;
    }

private:
    int _id;

    std::string _sourceFilePath;

    std::string _targetFilePath;

    std::string _name;

    std::string _sourceLanguageCode;

    std::string _targetLanguageCode;

    int _status;

    int _type;

    int _tmId;

    std::string _created;
};

#endif
