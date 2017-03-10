#ifndef TM_DAO_HDR
#define TM_DAO_HDR

#include <string>
#include <vector>
#include <utility>

#include <concordia/common/config.hpp>
#include "db_connection.hpp"

class TmDAO {
public:
    /*! Constructor.
    */
    TmDAO();
    /*! Destructor.
    */
    virtual ~TmDAO();

    int addTm(const int sourceLangId, const int targetLangId, const std::string name);

    int addTm(const int sourceLangId, const int targetLangId, const std::string name, bool lemmatized);

    std::vector<int> getTmIds();

    std::pair<bool, std::string> getTmInfo(int tmId);

private:

};

#endif
