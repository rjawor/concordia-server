#ifndef CONCORDIA_SERVER_HDR
#define CONCORDIA_SERVER_HDR

#include <string>
#include <concordia/concordia_exception.hpp>
#include <boost/shared_ptr.hpp>
#include <concordia/concordia.hpp>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"

#include "index_controller.hpp"
#include "searcher_controller.hpp"

class ConcordiaServer {
public:
    /*! Constructor.
      \param configFilePath path to the Concordia configuration file
      \throws ConcordiaException
    */
    explicit ConcordiaServer(const std::string & configFilePath)
                                         throw(ConcordiaException);
    /*! Destructor.
    */
    virtual ~ConcordiaServer();

    std::string handleRequest(std::string & requestString);

private:    
    boost::shared_ptr<IndexController> _indexController;

    boost::shared_ptr<SearcherController> _searcherController;

};

#endif
