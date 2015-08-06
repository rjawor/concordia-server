#ifndef DB_MANAGER_HDR
#define DB_MANAGER_HDR

#include <libpq-fe.h>
#include <string>
#include <vector>

#include <concordia/concordia_exception.hpp>

#include "query_param.hpp"

class DBconnection {
public:
    /*! Constructor.
    */
    DBconnection() throw(ConcordiaException);
    /*! Destructor.
    */
    virtual ~DBconnection();
    
    void startTransaction() throw(ConcordiaException);

    void endTransaction() throw(ConcordiaException);

    PGresult * execute(std::string query) throw(ConcordiaException);

    PGresult * execute(std::string query,
                       std::vector<QueryParam*> params) throw(ConcordiaException);

    void clearResult(PGresult * result);
    
    int getIntValue(PGresult * result, int row, int col);
    
private:
    void close();
    
    PGconn * _connection;
};

#endif
