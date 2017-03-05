#include "lemmatizer_facade.hpp"


LemmatizerFacade::LemmatizerFacade() throw(ConcordiaException) {
    _lemmatizersMap = boost::ptr_map<std::string,SocketLemmatizer>();

    // todo: extract this to configuration, especially when new lemmatizers ConstMemberIterator
    SocketLemmatizer * socketLemmatizer1 =  new SocketLemmatizer(11000);
    std::string plCode = "pl";
    std::string enCode = "en";
    std::string hrCode = "hr";

    _lemmatizersMap.insert(plCode, socketLemmatizer1);
    _lemmatizersMap.insert(enCode, socketLemmatizer1);
    _lemmatizersMap.insert(hrCode, socketLemmatizer1);
}

LemmatizerFacade::~LemmatizerFacade() {
}

std::string LemmatizerFacade::lemmatizeSentence(std::string languageCode, std::string sentence) {

    boost::ptr_map<std::string,SocketLemmatizer>::iterator it = _lemmatizersMap.find(languageCode);
    if (it != _lemmatizersMap.end()) {
        return it->second->lemmatizeSentence(languageCode, sentence);
    } else {
        throw ConcordiaException("lemmatizer for language: "+languageCode+" not found.");
    }

}
