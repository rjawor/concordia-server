#include "json_generator.hpp"


JsonGenerator::JsonGenerator() {
}

JsonGenerator::~JsonGenerator() {
}


void JsonGenerator::signalError(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter, string  message) {
    jsonWriter.StartObject();
    jsonWriter.String("status");
    jsonWriter.String("error");
    jsonWriter.String("message");
    jsonWriter.String(message.c_str());
    jsonWriter.EndObject();
}
    


