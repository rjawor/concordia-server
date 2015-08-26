#include "json_generator.hpp"


JsonGenerator::JsonGenerator() {
}

JsonGenerator::~JsonGenerator() {
}


void JsonGenerator::signalError(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                                const std::string & message) {
    jsonWriter.StartObject();
    jsonWriter.String("status");
    jsonWriter.String("error");
    jsonWriter.String("message");
    jsonWriter.String(message.c_str());
    jsonWriter.EndObject();
}

void JsonGenerator::writeSearchResult(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                                      const SimpleSearchResult & result) {
    jsonWriter.StartObject();
    jsonWriter.String("id");
    jsonWriter.Int(result.getId());
    jsonWriter.String("matchedPatternStart");
    jsonWriter.Int(result.getMatchedPatternStart());
    jsonWriter.String("matchedPatternEnd");
    jsonWriter.Int(result.getMatchedPatternEnd());
    jsonWriter.String("matchedExampleStart");
    jsonWriter.Int(result.getMatchedExampleStart());
    jsonWriter.String("matchedExampleEnd");
    jsonWriter.Int(result.getMatchedExampleEnd());
    jsonWriter.String("sourceSegment");
    jsonWriter.String(result.getSourceSegment().c_str());
    jsonWriter.String("targetSegment");
    jsonWriter.String(result.getTargetSegment().c_str());                        
    jsonWriter.EndObject();
}



