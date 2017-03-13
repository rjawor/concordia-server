#include "tm.hpp"

Tm::Tm(const int id,
   const std::string & name,
   const std::string & sourceLanguageCode,
   const std::string & targetLanguageCode) :
                       _id(id),_name(name),
                       _sourceLanguageCode(sourceLanguageCode),
                       _targetLanguageCode(targetLanguageCode) {
}

Tm::~Tm() {
}
