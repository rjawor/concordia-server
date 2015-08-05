#include "index_controller.hpp"

#include "json_generator.hpp"

IndexController::IndexController(boost::shared_ptr<Concordia> concordia)
                                         throw(ConcordiaException):
                                         _concordia(concordia) {
}

IndexController::~IndexController() {
}


void IndexController::addSentence(
                     rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                     std::string & sourceSentence,
                     std::string & targetSentence,
                     int tmId) {

    try {
        boost::shared_ptr<TokenizedSentence> tokenizedSentence = _concordia->tokenize(sourceSentence);
        int sentenceId = _unitDAO.addSentence(tokenizedSentence, targetSentence, tmId);     
        _concordia->addTokenizedExample(tokenizedSentence, sentenceId);
        _concordia->refreshSAfromRAM();

        jsonWriter.StartObject();
        jsonWriter.String("status");
        jsonWriter.String("success");
        jsonWriter.EndObject();
    } catch (ConcordiaException & e) {
        std::stringstream errorstream;
        errorstream << "concordia error: " << e.what();
        JsonGenerator::signalError(jsonWriter, errorstream.str());        
    }
}

