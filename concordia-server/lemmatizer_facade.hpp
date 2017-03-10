#ifndef LEMMATIZER_FACADE_HDR
#define LEMMATIZER_FACADE_HDR

#include "socket_lemmatizer.hpp"
#include "tm_dao.hpp"

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

    std::string lemmatizeIfNeeded(std::string pattern, int tmId);

    std::vector<std::string> lemmatizeSentencesIfNeeded(std::vector<std::string> patterns, int tmId);

private:
    boost::ptr_map<std::string,SocketLemmatizer> _lemmatizersMap;

    TmDAO _tmDAO;
};

#endif
