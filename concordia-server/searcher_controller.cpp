#include "searcher_controller.hpp"

#include <vector>

SearcherController::SearcherController(boost::shared_ptr<Concordia> concordia)
                                         throw(ConcordiaException):
                                         _concordia(concordia) {
}

SearcherController::~SearcherController() {
}


void SearcherController::simpleSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter, std::string & pattern) {
    std::vector<SubstringOccurence> results = _concordia->simpleSearch(pattern);

    jsonWriter.StartObject();
    jsonWriter.String("status");
    jsonWriter.String("success");
    jsonWriter.String("results");
    jsonWriter.StartArray();
    
    jsonWriter.EndArray();
    jsonWriter.EndObject();            
}

void SearcherController::concordiaSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter, std::string & pattern) {
    jsonWriter.StartObject();
    jsonWriter.String("status");
    jsonWriter.String("error");
    jsonWriter.String("data");
    jsonWriter.String("concordia searching not yet implemented");
    jsonWriter.EndObject();            
}

