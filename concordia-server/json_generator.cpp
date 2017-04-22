#include "json_generator.hpp"

#include <boost/foreach.hpp>
#include "example_occurence.hpp"

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

void JsonGenerator::writeSimpleSearchResult(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                                      const SimpleSearchResult & result) {
    jsonWriter.StartObject();
    jsonWriter.String("matchedPatternStart");
    jsonWriter.Int(result.getMatchedPatternStart());
    jsonWriter.String("matchedPatternEnd");
    jsonWriter.Int(result.getMatchedPatternEnd());
    jsonWriter.String("occurences");
    jsonWriter.StartArray();

    BOOST_FOREACH(ExampleOccurence occurence, result.getOccurences()) {
        jsonWriter.StartObject();
        jsonWriter.String("id");
        jsonWriter.Int(occurence.getId());
        jsonWriter.String("matchedExampleStart");
        jsonWriter.Int(occurence.getMatchedExampleStart());
        jsonWriter.String("matchedExampleEnd");
        jsonWriter.Int(occurence.getMatchedExampleEnd());
        jsonWriter.String("sourceSegment");
        jsonWriter.String(occurence.getSourceSegment().c_str());
        jsonWriter.String("targetSegment");
        jsonWriter.String(occurence.getTargetSegment().c_str());
        jsonWriter.String("targetFragments");
        jsonWriter.StartArray(); // all target fragments
        for (std::vector<std::pair<int,int> >::const_iterator it = occurence.getTargetFragments().begin();
                it != occurence.getTargetFragments().end(); it++) {
            jsonWriter.StartArray(); // single target fragment
            jsonWriter.Int(it->first);
            jsonWriter.Int(it->second);
            jsonWriter.EndArray(); // single target fragment
        }
        jsonWriter.EndArray(); // all target fragments
        jsonWriter.EndObject(); // occurence
    }

    jsonWriter.EndArray(); //occurences

    jsonWriter.EndObject(); //simple search result
}
