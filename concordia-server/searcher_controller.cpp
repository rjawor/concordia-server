#include "searcher_controller.hpp"

#include <boost/foreach.hpp>
#include <vector>

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
        jsonWriter.StartObject();
        jsonWriter.String("id");
        jsonWriter.Int(result.getId());
        jsonWriter.String("matchedFragmentStart");
        jsonWriter.Int(result.getMatchedFragmentStart());
        jsonWriter.String("matchedFragmentEnd");
        jsonWriter.Int(result.getMatchedFragmentEnd());
        jsonWriter.String("sourceSegment");
        jsonWriter.String(result.getSourceSegment().c_str());
        jsonWriter.String("targetSegment");
        jsonWriter.String(result.getTargetSegment().c_str());                        
        jsonWriter.EndObject();            
    }    
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

