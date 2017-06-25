#ifndef REQUEST_DAO_HDR
#define REQUEST_DAO_HDR

#include <string>
#include <vector>
#include <utility>

#include <concordia/common/config.hpp>
#include "db_connection.hpp"
#include "request.hpp"

class RequestDAO {
public:
    /*! Constructor.
    */
    RequestDAO();
    /*! Destructor.
    */
    virtual ~RequestDAO();

    int addRequest(const std::string sourceFilePath, const std::string targetFilePath, const int sourceLangId, const int targetLangId, const std::string name, int type, int tmId);

    std::vector<Request> getRequests();

private:

};

#endif
