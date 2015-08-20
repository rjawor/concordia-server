#include "logger.hpp"

#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"

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

void Logger::_initialize(log4cpp::Category & root) {
    log4cpp::Appender *appender = new log4cpp::FileAppender("default", "/tmp/concordia-server.log");
    appender->setLayout(new log4cpp::BasicLayout());

    root.setPriority(log4cpp::Priority::INFO);
    root.addAppender(appender);
    
    _initialized = 1;
}


