#include "index_controller.hpp"

#include <concordia/common/config.hpp>

#include "json_generator.hpp"

IndexController::IndexController(boost::shared_ptr<Concordia> concordia)
                                         throw(ConcordiaException):
                                         _concordia(concordia) {
}

IndexController::~IndexController() {
}


void IndexController::addSentence(
                     rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                     const std::string & sourceSentence,
                     const std::string & targetSentence,
                     const int tmId) {

    try {
        TokenizedSentence tokenizedSentence = _concordia->tokenize(sourceSentence);
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

void IndexController::addSentences(
                 rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                 const std::vector<std::string> & sourceSentences,
                 const std::vector<std::string> & targetSentences,
                 const std::vector<int> & tmIds) {
    try {
        std::vector<TokenizedSentence> tokenizedSentences = _concordia->tokenizeAll(sourceSentences);
        std::vector<SUFFIX_MARKER_TYPE> sentenceIds = _unitDAO.addSentences(tokenizedSentences, targetSentences, tmIds);     
        _concordia->addAllTokenizedExamples(tokenizedSentences, sentenceIds);
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

