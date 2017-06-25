#include "request.hpp"

Request::Request(
   const int id,
   const std::string & sourceFilePath,
   const std::string & targetFilePath,
   const std::string & name,
   const std::string & sourceLanguageCode,
   const std::string & targetLanguageCode,
   const int status,
   const int type,
   const int tmId,
   const std::string & created) :
                       _id(id),
                       _sourceFilePath(sourceFilePath),
                       _targetFilePath(targetFilePath),
                       _name(name),
                       _sourceLanguageCode(sourceLanguageCode),
                       _targetLanguageCode(targetLanguageCode),
                       _status(status),
                       _type(type),
                       _tmId(tmId),
                       _created(created) {
}

Request::~Request() {
}
