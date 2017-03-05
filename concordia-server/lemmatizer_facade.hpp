#ifndef LEMMATIZER_FACADE_HDR
#define LEMMATIZER_FACADE_HDR

#include "socket_lemmatizer.hpp"

#include <string>
#include <concordia/concordia_exception.hpp>
#include <boost/ptr_container/ptr_map.hpp>


class LemmatizerFacade {
public:
    /*! Constructor.
    */
    LemmatizerFacade() throw(ConcordiaException);
    /*! Destructor.
    */
    virtual ~LemmatizerFacade();

    std::string lemmatizeSentence(std::string languageCode, std::string sentence);
private:
    boost::ptr_map<std::string,SocketLemmatizer> _lemmatizersMap;
};

#endif
