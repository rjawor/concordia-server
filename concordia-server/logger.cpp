#include "logger.hpp"

#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/PatternLayout.hh"
#include "log4cpp/Priority.hh"

#include "config.hpp"
#include <sstream>

Logger::Logger() {
}

Logger::~Logger() {
}

int Logger::_initialized = 0;

void Logger::log(std::string message) {
    log4cpp::Category & root = log4cpp::Category::getRoot();
    if (_initialized == 0) {
        _initialize(root);
 	}
	root.info(message);
}

void Logger::logInt(std::string name, int value) {
    log4cpp::Category & root = log4cpp::Category::getRoot();
    if (_initialized == 0) {
        _initialize(root);
 	}
 	std::stringstream ss;
 	ss << "    " << name << ": " << value;
	root.info(ss.str());
}

void Logger::logString(std::string name, std::string value) {
    log4cpp::Category & root = log4cpp::Category::getRoot();
    if (_initialized == 0) {
        _initialize(root);
 	}
 	std::stringstream ss;
 	ss << "    " << name << ": " << value;
	root.info(ss.str());
}

void Logger::logFragment(const MatchedPatternFragment & fragment) {
    log4cpp::Category & root = log4cpp::Category::getRoot();
    if (_initialized == 0) {
        _initialize(root);
 	}
 	std::stringstream ss;
 	ss << fragment;
	root.info(ss.str());
}

void Logger::logConcordiaSearchResult(const ConcordiaSearchResult & result) {
    log4cpp::Category & root = log4cpp::Category::getRoot();
    if (_initialized == 0) {
        _initialize(root);
 	}
 	std::stringstream ss;
 	ss << result;
	root.info(ss.str());
}

void Logger::_initialize(log4cpp::Category & root) {
    log4cpp::Appender *appender = new log4cpp::FileAppender("default", LOG_FILE_PATH);
    log4cpp::PatternLayout *layout = new log4cpp::PatternLayout();
    layout->setConversionPattern("%d{%Y-%m-%d %H:%M:%S}%c %x: %m%n");
    appender->setLayout(layout);

    root.setPriority(log4cpp::Priority::INFO);
    root.addAppender(appender);

    _initialized = 1;
}
