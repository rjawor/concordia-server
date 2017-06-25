#ifndef LANGUAGE_DAO_HDR
#define LANGUAGE_DAO_HDR

#include <string>
#include <vector>
#include <utility>

#include <concordia/common/config.hpp>
#include "db_connection.hpp"
#include "language.hpp"

class LanguageDAO {
public:
    /*! Constructor.
    */
    LanguageDAO();
    /*! Destructor.
    */
    virtual ~LanguageDAO();

    std::vector<Language> getLanguages();

private:

};

#endif
