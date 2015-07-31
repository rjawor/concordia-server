#include "searcher_controller.hpp"

#include <concordia/substring_occurence.hpp>
#include <vector>

SearcherController::SearcherController(boost::shared_ptr<Concordia> concordia)
                                         throw(ConcordiaException):
                                         _concordia(concordia) {
}

SearcherController::~SearcherController() {
}


void SearcherController::simpleSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter, string & pattern) {
    vector<SubstringOccurence> result = _concordia->simpleSearch(pattern);

    jsonWriter.StartObject();
    jsonWriter.String("status");
    jsonWriter.String("success");
    jsonWriter.String("count");
    jsonWriter.Int(result.size());
    jsonWriter.String("firstId");
    jsonWriter.Int(result.at(0).getId());
    jsonWriter.String("firstOffset");
    jsonWriter.Int(result.at(0).getOffset());
    jsonWriter.EndObject();            
}

void SearcherController::concordiaSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter, string & pattern) {
    jsonWriter.StartObject();
    jsonWriter.String("status");
    jsonWriter.String("error");
    jsonWriter.String("data");
    jsonWriter.String("concordia searching not yet implemented");
    jsonWriter.EndObject();            
}

