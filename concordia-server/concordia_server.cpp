#include "concordia_server.hpp"

#include <sstream>
#include <concordia/example.hpp>
#include <concordia/substring_occurence.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"

ConcordiaServer::ConcordiaServer(const std::string & configFilePath)
                                         throw(ConcordiaException) {
    _concordia = boost::shared_ptr<Concordia> (
                            new Concordia(configFilePath));

}

ConcordiaServer::~ConcordiaServer() {
}

string ConcordiaServer::handleRequest(string & requestString) {
    rapidjson::StringBuffer outputJson;
    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(outputJson);

    stringstream outputString;

    try {
        outputString << "Content-type: application/json\r\n\r\n";
        /*
        ss << "    <h1>Adding content as example:</h1>\n";

        Example example1(requestString, 0);
        Example example2("Ala ma kota", 1);
        Example example3("Marysia nie ma kota chyba", 2);
        _concordia->addExample(example1);
        _concordia->addExample(example2);
        _concordia->addExample(example3);

        _concordia->refreshSAfromRAM();

        ss << "    <h1>Searching ma kota:</h1>\n";
        std::vector<SubstringOccurence> result =
                                 _concordia->simpleSearch("ma kota");
        BOOST_FOREACH(SubstringOccurence occurence, result) {
            ss << "\t\tfound match in sentence number: "
                      << occurence.getId() << "<br/><br/>";
        } 
        */
        rapidjson::Document d;
        bool hasError = d.Parse(requestString.c_str()).HasParseError();

        if (hasError) {
            stringstream errorstream;
            errorstream << "json parse error at offset: " << d.GetErrorOffset() <<
                           ", description: " <<  GetParseError_En(d.GetParseError());
            jsonWriter.StartObject();
            jsonWriter.String("status");
            jsonWriter.String("error");
            jsonWriter.String("message");
            jsonWriter.String(errorstream.str().c_str());
            jsonWriter.EndObject();            
        } else {
            jsonWriter.StartObject();
            jsonWriter.String("status");
            jsonWriter.String("success");
            jsonWriter.String("passedOperation");
            jsonWriter.String(d["operation"].GetString());
            jsonWriter.EndObject();            
        }
             
    } catch (ConcordiaException & e) {
        stringstream errorstream;
        errorstream << "concordia error: " << e.what();
        jsonWriter.StartObject();
        jsonWriter.String("status");
        jsonWriter.String("error");
        jsonWriter.String("message");
        jsonWriter.String(errorstream.str().c_str());
        jsonWriter.EndObject();            
        
    }

    outputString << outputJson.GetString();

    return outputString.str();

}
