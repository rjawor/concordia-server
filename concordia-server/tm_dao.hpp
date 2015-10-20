#ifndef TM_DAO_HDR
#define TM_DAO_HDR

#include <string>
#include <vector>

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

    std::vector<int> getTmIds();

private:

};

#endif
