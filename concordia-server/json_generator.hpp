#ifndef JSON_GENERATOR_HDR
#define JSON_GENERATOR_HDR

#include <string>

#include "rapidjson/writer.h"

#include "simple_search_result.hpp"

class JsonGenerator {
public:
    /*! Constructor.
    */
    JsonGenerator();
    /*! Destructor.
    */
    virtual ~JsonGenerator();

    static void signalError(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                            const std::string & message);

    static void writeSimpleSearchResult(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                                        const SimpleSearchResult & result);

private:

};

#endif
