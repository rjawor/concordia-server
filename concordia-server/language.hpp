#ifndef LANGUAGE_HDR
#define LANGUAGE_HDR

#include <string>
#include <vector>

class Language {
public:
    /*! Constructor.
    */
    Language(
       const int id,
       const std::string & code,
       const std::string & name
   );
    /*! Destructor.
    */
    virtual ~Language();

    int getId() const {
        return _id;
    }

    const std::string & getCode() const {
        return _code;
    }

    const std::string & getName() const {
        return _name;
    }


private:
    int _id;

    std::string _code;

    std::string _name;
};

#endif
