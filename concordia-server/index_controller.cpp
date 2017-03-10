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

IndexController::IndexController(boost::shared_ptr<boost::ptr_map<int,Concordia> >concordiasMap,
                                 boost::shared_ptr<LemmatizerFacade> lemmatizerFacade)
                                                                    throw(ConcordiaException):
                                                                     _concordiasMap(concordiasMap),
                                                                     _lemmatizerFacade(lemmatizerFacade) {
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
            TokenizedSentence tokenizedLemmatizedSentence = it->second->tokenize(_lemmatizerFacade->lemmatizeIfNeeded(sourceSentence, tmId));
            TokenizedSentence tokenizedSentence = it->second->tokenize(sourceSentence);
            int sentenceId = _unitDAO.addSentence(tokenizedSentence, targetSentence, tmId);
            it->second->addTokenizedExample(tokenizedLemmatizedSentence, sentenceId);
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
            std::vector<TokenizedSentence> tokenizedLemmatizedSentences = it->second->tokenizeAll(_lemmatizerFacade->lemmatizeSentencesIfNeeded(sourceSentences, tmId));
            std::vector<TokenizedSentence> tokenizedSentences = it->second->tokenizeAll(sourceSentences);
            std::vector<SUFFIX_MARKER_TYPE> sentenceIds = _unitDAO.addSentences(tokenizedSentences, targetSentences, tmId);
            it->second->addAllTokenizedExamples(tokenizedLemmatizedSentences, sentenceIds);

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
                 const std::vector<std::string> & rawSourceSentences,
                 const std::vector<std::string> & targetSentences,
                 const int tmId) {
    try {
        boost::ptr_map<int,Concordia>::iterator it = _concordiasMap->find(tmId);
        if (it != _concordiasMap->end()) {
            std::vector<std::string> sourceSentences;
            std::vector<std::vector<std::vector<int> > > allAlignments;
            _getSourceSentencesAndAlignments(sourceSentences, allAlignments, rawSourceSentences);

            std::vector<TokenizedSentence> tokenizedSourceSentences = it->second->tokenizeAll(sourceSentences, true, true);
            std::vector<TokenizedSentence> tokenizedTargetSentences = it->second->tokenizeAll(targetSentences, true, false);

            std::vector<SUFFIX_MARKER_TYPE> sentenceIds = _unitDAO.addAlignedSentences(tokenizedSourceSentences, tokenizedTargetSentences, allAlignments, tmId);
            for(int index = 0; index < tokenizedSourceSentences.size(); index++) {
                it->second->addTokenizedExample(tokenizedSourceSentences.at(index), sentenceIds.at(index));
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

void IndexController::addAlignedLemmatizedSentences(
                 rapidjson::Writer<rapidjson::StringBuffer> & jsonWriter,
                 const std::vector<std::string> & sourceSentences,
                 const std::vector<std::string> & targetSentences,
                 const std::vector<std::string> & alignmentStrings,
                 const int tmId) {
    try {
        boost::ptr_map<int,Concordia>::iterator it = _concordiasMap->find(tmId);
        if (it != _concordiasMap->end()) {
            std::vector<std::string> lemmatizedSourceSentences;
            std::vector<std::vector<std::vector<int> > > allAlignments;
            _getSourceSentencesAndAlignments(lemmatizedSourceSentences, allAlignments, alignmentStrings);

            std::vector<TokenizedSentence> tokenizedLemmatizedSourceSentences = it->second->tokenizeAll(lemmatizedSourceSentences, true, true);
            std::vector<TokenizedSentence> tokenizedSourceSentences = it->second->tokenizeAll(sourceSentences, true, false);
            std::vector<TokenizedSentence> tokenizedTargetSentences = it->second->tokenizeAll(targetSentences, true, false);

            std::vector<SUFFIX_MARKER_TYPE> sentenceIds =

            _unitDAO.addAlignedSentences(tokenizedSourceSentences, tokenizedTargetSentences, allAlignments, tmId);
            for(int index = 0; index < tokenizedLemmatizedSourceSentences.size(); index++) {
                it->second->addTokenizedExample(tokenizedLemmatizedSourceSentences.at(index), sentenceIds.at(index));
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

void IndexController::_getSourceSentencesAndAlignments(
                        std::vector<std::string> & sourceSentences,
                        std::vector<std::vector<std::vector<int> > > & allAlignments,
                        const std::vector<std::string> & rawSourceSentences) {

    for (int i = 0; i<rawSourceSentences.size(); i++) {
        std::string rawSourceSentence = rawSourceSentences[i];

        std::string sourceSentence = "";
        std::vector<std::vector<int> > alignments;

        UnicodeString s(rawSourceSentence.c_str());
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
                sourceSentence += token + " ";
            }
        }

        sourceSentence = sourceSentence.substr(0, sourceSentence.length()-1);

        sourceSentences.push_back(sourceSentence);
        allAlignments.push_back(alignments);
    }
}
