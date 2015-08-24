#include "searcher_controller.hpp"

#include <boost/foreach.hpp>
#include <vector>

#include "json_generator.hpp"

SearcherController::SearcherController(boost::shared_ptr<Concordia> concordia)
                                         throw(ConcordiaException):
                                         _concordia(concordia) {
}

SearcherController::~SearcherController() {
}


void SearcherController::simpleSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter, std::string & pattern) {
    std::vector<SimpleSearchResult> results = _unitDAO.getSearchResults(_concordia->simpleSearch(pattern));

    jsonWriter.StartObject();
    jsonWriter.String("status");
    jsonWriter.String("success");
    jsonWriter.String("results");
    jsonWriter.StartArray();
    BOOST_FOREACH(SimpleSearchResult & result, results) {
        JsonGenerator::writeSearchResult(jsonWriter, result);        
    }    
    jsonWriter.EndArray();
    jsonWriter.EndObject();
}

void SearcherController::concordiaSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter, std::string & pattern) {

    CompleteConcordiaSearchResult result = _unitDAO.getConcordiaResult(_concordia->concordiaSearch(pattern));
    
    jsonWriter.StartObject();
    jsonWriter.String("status");
    jsonWriter.String("success");
    jsonWriter.String("result");
    jsonWriter.StartObject();
    jsonWriter.String("bestOverlayScore");
    jsonWriter.Double(result.getBestOverlayScore());
    jsonWriter.String("bestOverlay");
    jsonWriter.StartArray();
    BOOST_FOREACH(SimpleSearchResult & simpleResult, result.getBestOverlay()) {
        JsonGenerator::writeSearchResult(jsonWriter, simpleResult);        
    }    
    jsonWriter.EndArray();
    jsonWriter.EndObject();
    
    
    jsonWriter.EndObject();            
}

