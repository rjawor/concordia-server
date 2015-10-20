#include "searcher_controller.hpp"

#include <boost/foreach.hpp>
#include <vector>

#include "json_generator.hpp"

SearcherController::SearcherController(boost::shared_ptr<boost::ptr_map<int,Concordia> >concordiasMap)
                                                                     throw(ConcordiaException):
                                                                     _concordiasMap(concordiasMap) {
}

SearcherController::~SearcherController() {
}


void SearcherController::simpleSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                                      std::string & pattern,
                                      const int tmId) {
    boost::ptr_map<int,Concordia>::iterator it = _concordiasMap->find(tmId);
    if (it != _concordiasMap->end()) {
        std::vector<SimpleSearchResult> results = _unitDAO.getSearchResults((*_concordiasMap)[tmId].simpleSearch(pattern));

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
    } else {
        JsonGenerator::signalError(jsonWriter, "no such tm!");    
    }
}

void SearcherController::concordiaSearch(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                                         std::string & pattern,
                                         const int tmId) {

    boost::ptr_map<int,Concordia>::iterator it = _concordiasMap->find(tmId);
    if (it != _concordiasMap->end()) {
        CompleteConcordiaSearchResult result = _unitDAO.getConcordiaResult((*_concordiasMap)[tmId].concordiaSearch(pattern));
        
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
    } else {
        JsonGenerator::signalError(jsonWriter, "no such tm!");        
    }            
}

