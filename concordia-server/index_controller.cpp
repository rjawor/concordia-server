#include "index_controller.hpp"

#include <concordia/common/config.hpp>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <boost/regex.hpp>
#include <boost/regex/icu.hpp>
#include <unicode/unistr.h>

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
            TokenizedSentence tokenizedSentence = it->second->tokenize(sourceSentence);
            int sentenceId = _unitDAO.addSentence(tokenizedSentence, targetSentence, tmId);     
            it->second->addTokenizedExample(tokenizedSentence, sentenceId);
            it->second->refreshSAfromRAM();

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
            std::vector<TokenizedSentence> tokenizedSentences = it->second->tokenizeAll(sourceSentences);
            std::vector<SUFFIX_MARKER_TYPE> sentenceIds = _unitDAO.addSentences(tokenizedSentences, targetSentences, tmId);
            it->second->addAllTokenizedExamples(tokenizedSentences, sentenceIds);

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
            std::vector<AlignedUnit> alignedUnits = _getAlignedUnits(sourceSentences, targetSentences, tmId);
            std::vector<SUFFIX_MARKER_TYPE> sentenceIds = _unitDAO.addAlignedUnits(alignedUnits, tmId);
            int index = 0;
            for(std::vector<AlignedUnit>::iterator ait = alignedUnits.begin(); ait != alignedUnits.end(); ++ait) {
                it->second->addTokenizedExample(ait->getSourceSentence(), sentenceIds.at(index));
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
            it->second->refreshSAfromRAM();

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
                                                           const std::vector<std::string> & targetSentences,
                                                           const int tmId) {
    std::vector<AlignedUnit> result;
    for (int i = 0; i<sourceSentences.size(); i++) {
        std::string sourceSentence = sourceSentences[i];
        std::string targetSentence = targetSentences[i];
        
        std::string rawSourceSentence;
        std::vector<TokenAnnotation> sourceTokens;
        std::vector<std::vector<int> > alignments;
        
        UnicodeString s(sourceSentence.c_str());
        boost::u32regex_iterator<const UChar*> begin(
                           boost::make_u32regex_iterator(
                               s,
                               boost::make_u32regex(UnicodeString("(\\S+) \\(\\{(( \\d+)*) \\}\\)"), boost::regex::icase)
                           )
                                               );
        boost::u32regex_iterator<const UChar*> end;
        
        for (; begin != end; ++begin) {
            UnicodeString tokenUTF8((*begin)[1].first, (*begin).length(1));
            std::string token;
            tokenUTF8.toUTF8String(token);

            if (token != "NULL") {
                std::string numbers((*begin)[2].first, (*begin)[2].second);            
                std::istringstream iss(numbers);
                std::vector<std::string> numberStrings;
                std::copy(std::istream_iterator<std::string>(iss),
                          std::istream_iterator<std::string>(),
                          std::back_inserter(numberStrings));

                std::vector<int> tokenAlignments;                
                for (int j=0;j<numberStrings.size();j++) {
                    int n = atoi(numberStrings[j].c_str()) - 1; //subtracting 1 as we want alignments to be 0-based
                    tokenAlignments.push_back(n);
                }
                alignments.push_back(tokenAlignments);
                rawSourceSentence += token + " ";
            }
        }
        
        rawSourceSentence = _trim(rawSourceSentence);
        
        
        boost::ptr_map<int,Concordia>::iterator it = _concordiasMap->find(tmId);
        if (it != _concordiasMap->end()) {
            TokenizedSentence sourceTS = it->second->tokenize(rawSourceSentence, true);
            TokenizedSentence targetTS = it->second->tokenize(targetSentence, true);
                    
            result.push_back(AlignedUnit(sourceTS, targetTS, alignments));
        }
    }
    return result;
}

std::string IndexController::_trim(std::string & str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
}

