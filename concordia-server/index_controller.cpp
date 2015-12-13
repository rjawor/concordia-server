#include "index_controller.hpp"

#include <concordia/common/config.hpp>

#include "json_generator.hpp"
#include "logger.hpp"

IndexController::IndexController(boost::shared_ptr<boost::ptr_map<int,Concordia> >concordiasMap)
                                                                    throw(ConcordiaException):
                                                                     _concordiasMap(concordiasMap) {
}

IndexController::~IndexController() {
}


void IndexController::addSentence(
                     rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                     const std::string & sourceSentence,
                     const std::string & targetSentence,
                     const int tmId) {

    try {
        boost::ptr_map<int,Concordia>::iterator it = _concordiasMap->find(tmId);
        if (it != _concordiasMap->end()) {
            TokenizedSentence tokenizedSentence = (*_concordiasMap)[tmId].tokenize(sourceSentence);
            int sentenceId = _unitDAO.addSentence(tokenizedSentence, targetSentence, tmId);     
            (*_concordiasMap)[tmId].addTokenizedExample(tokenizedSentence, sentenceId);
            (*_concordiasMap)[tmId].refreshSAfromRAM();

            jsonWriter.StartObject();
            jsonWriter.String("status");
            jsonWriter.String("success");
            jsonWriter.EndObject();
        } else {
            JsonGenerator::signalError(jsonWriter, "no such tm!");        
        }
    } catch (ConcordiaException & e) {
        std::stringstream errorstream;
        errorstream << "concordia error: " << e.what();
        JsonGenerator::signalError(jsonWriter, errorstream.str());        
    } catch (std::exception & e) {
        std::stringstream errorstream;
        errorstream << "general error: " << e.what();
        JsonGenerator::signalError(jsonWriter, errorstream.str());        
    } catch (...) {
        std::stringstream errorstream;
        errorstream << "unexpected error occurred";
        JsonGenerator::signalError(jsonWriter, errorstream.str());        
    }
}

void IndexController::addSentences(
                 rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                 const std::vector<std::string> & sourceSentences,
                 const std::vector<std::string> & targetSentences,
                 const int tmId) {
    try {
        boost::ptr_map<int,Concordia>::iterator it = _concordiasMap->find(tmId);
        if (it != _concordiasMap->end()) {
            std::vector<TokenizedSentence> tokenizedSentences = (*_concordiasMap)[tmId].tokenizeAll(sourceSentences);
            std::vector<SUFFIX_MARKER_TYPE> sentenceIds = _unitDAO.addSentences(tokenizedSentences, targetSentences, tmId);
            (*_concordiasMap)[tmId].addAllTokenizedExamples(tokenizedSentences, sentenceIds);

            jsonWriter.StartObject();
            jsonWriter.String("status");
            jsonWriter.String("success");
            jsonWriter.EndObject();
        } else {
            JsonGenerator::signalError(jsonWriter, "no such tm!");        
        }
    } catch (ConcordiaException & e) {
        std::stringstream errorstream;
        errorstream << "concordia error: " << e.what();
        JsonGenerator::signalError(jsonWriter, errorstream.str());        
    }
}

void IndexController::addAlignedSentences(
                 rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                 const std::vector<std::string> & sourceSentences,
                 const std::vector<std::string> & targetSentences,
                 const int tmId) {
    try {
        boost::ptr_map<int,Concordia>::iterator it = _concordiasMap->find(tmId);
        if (it != _concordiasMap->end()) {
            std::vector<AlignedUnit> alignedUnits = _getAlignedUnits(sourceSentences, targetSentences);
            std::vector<SUFFIX_MARKER_TYPE> sentenceIds = _unitDAO.addAlignedUnits(alignedUnits, tmId);
            int index = 0;
            for(std::vector<AlignedUnit>::iterator it = alignedUnits.begin(); it != alignedUnits.end(); ++it) {
                (*_concordiasMap)[tmId].addTokenizedExample(*(it->getSourceSentence()), sentenceIds.at(index));
                index++;
            }            

            jsonWriter.StartObject();
            jsonWriter.String("status");
            jsonWriter.String("success");
            jsonWriter.EndObject();
        } else {
            JsonGenerator::signalError(jsonWriter, "no such tm!");        
        }
    } catch (ConcordiaException & e) {
        std::stringstream errorstream;
        errorstream << "concordia error: " << e.what();
        JsonGenerator::signalError(jsonWriter, errorstream.str());        
    }
}

void IndexController::refreshIndexFromRAM(rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                                          const int tmId) {
    try {
        boost::ptr_map<int,Concordia>::iterator it = _concordiasMap->find(tmId);
        if (it != _concordiasMap->end()) {
            (*_concordiasMap)[tmId].refreshSAfromRAM();

            jsonWriter.StartObject();
            jsonWriter.String("status");
            jsonWriter.String("success");
            jsonWriter.EndObject();
        } else {
            JsonGenerator::signalError(jsonWriter, "no such tm!");        
        }
    } catch (ConcordiaException & e) {
        std::stringstream errorstream;
        errorstream << "concordia error: " << e.what();
        JsonGenerator::signalError(jsonWriter, errorstream.str());        
    }

}

std::vector<AlignedUnit> IndexController::_getAlignedUnits(const std::vector<std::string> & sourceSentences,
                                                           const std::vector<std::string> & targetSentences) {
    //TODO
}




