#ifndef JSON_GENERATOR_HDR
#define JSON_GENERATOR_HDR

#include <string>

#include "rapidjson/writer.h"


using namespace std;

class JsonGenerator {
public:
    /*! Constructor.
    */
    JsonGenerator();
    /*! Destructor.
    */
    virtual ~JsonGenerator();

    static void signalError(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter, string  message);

private:

};

#endif
